/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:12:58 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 17:28:57 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

//local pixel_put for inline

static inline void	my_pixel_put2(int x, int y, t_img *img, unsigned int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

static inline void	put_pixel(int x, int y, t_game *r, unsigned int color)
{
 	if (!r->supersample)
		my_pixel_put2(x, y, &r->img, color);
	else
		r->pixels_xl[y][x] = color;
}

static inline double get_dist(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

static inline double get_dist2(double x1, double y1, double x2, double y2)
{
	return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

static inline void	assign_colors(int colors_arr[4], t_colors *colors)
{
	colors_arr[0] = colors->color_1;
	colors_arr[1] = colors->color_2;
	colors_arr[2] = colors->color_3;
	colors_arr[3] = colors->color_4;
}

static inline int	calculate_color_component(int color_low, \
int color_high, double t, int shift)
{
	return ((int)((1 - t) * ((color_low >> shift) & 0xFF) + t * \
	((color_high >> shift) & 0xFF)) << shift);
}

static inline int	interpolate_colors(int color_low, int color_high, double t)
{
	int	red;
	int	green;
	int	blue;
	int alpha;

	alpha = calculate_color_component(color_low, color_high, t, 24);
	red = calculate_color_component(color_low, color_high, t, 16);
	green = calculate_color_component(color_low, color_high, t, 8);
	blue = calculate_color_component(color_low, color_high, t, 0);
	return (alpha | red | green | blue);
}

static inline int	calc_color_4(double distance, double max_distance, t_colors *colors)
{
	t_color_vars	vars;
	double			norm_distance;
	double			color_index_d;
	double			t;
	int				colors_arr[4];

	vars.num_colors = 4;//adustable to higher if more than 4 cols are added at some point
	norm_distance = distance / max_distance;
	color_index_d = norm_distance * (vars.num_colors - 1);

	//ifs are special handling for loop around cases
	if (color_index_d > vars.num_colors - 1)
		color_index_d = fmod(color_index_d, vars.num_colors);

	vars.color_index_low = (int)color_index_d;

	//ifs are special handling for loop around cases
	if (vars.color_index_low == vars.num_colors - 1)
		vars.color_index_high = 0;
	else
		vars.color_index_high = vars.color_index_low + 1;

	assign_colors(colors_arr, colors);
	t = color_index_d - vars.color_index_low;
	vars.color_low = colors_arr[vars.color_index_low];
	vars.color_high = colors_arr[vars.color_index_high];
	vars.color = interpolate_colors(vars.color_low, vars.color_high, t);
	return (vars.color);
}

/* checks if a point passes nearly through center for color(ghost center vert)
c_depth_ = 1 requires pass over(near) center(moving toward direct opposite)
c_depth_ = 2 goes tighter, requiring 2 passes consecutive
c_depth_ = 3 goes even tighter, requiring 3 passes */

static inline bool condition(t_game *r, int prev, int curr, int ph1, int ph2)
{
	bool	normal;	
	int 	c_depth;
	
	c_depth = r->jump_to_center_col;//set depth here, make control, god save
	normal = (r->dist_ratio <= 1);//normal or inverted
	if (c_depth == 1)
	{
		if (normal)
			return (ph1 == curr || ph2 == curr);
		else 
			return (prev == curr);
	}
	int sides = r->sides;
	int half = sides / 2;
	int obp = r->obp;
	int oh1 = (obp + half) % sides;
	int oh2 = (obp - half + sides) % sides;
	if (c_depth == 2)
	{
		if (normal)
			return (ph1 == curr && oh2 == ph1) || (ph2 == curr && oh1 == ph2);
		else
			return (curr == prev && (obp == ph1 || obp == ph2));
	}
	int tbp = r->tbp;
	int th1 = (tbp + half) % sides;
	int th2 = (tbp - half + sides) % sides;
	if (normal)
		return  (ph1 == curr && oh2 == ph1 && (th2 == oh2)) || 
				(ph2 == curr && oh1 == ph2 && (th1 == oh1));
	else
		return (prev == curr && (obp == ph1 || obp == ph2) && tbp == curr);//maybe tighter for odds?
	//	norm depth == 2 => (ph1 == curr && oh2 == ph1 && (th1 == oh1)) || (ph2 == curr && oh1 == ph2 && (th2 == oh2));//also good?
	//make 4th level for zooms, tofin
	/* int thbp = r->thbp;
	int thh1 = (thbp + half) % sides;
	//int thh2 = (thbp - half + sides) % sides;
	if (normal)
		return  (ph1 == curr && oh2 == ph1 && th2 == oh2 && thh1 == th2);// || 
				//(ph2 == curr && oh1 == ph2 && (th1 == oh1));
	else
		return (prev == curr && (obp == ph1 || obp == ph2) && tbp == curr);//maybe tighter for odds? */
	
}
//checks if going between two neighbors

static inline int condition2(t_game *r, int prev, int curr, int half)
{
	if (r->dist_ratio < 1)
		return ((prev + 1) % r->sides == curr || (prev - 1 + r->sides) % r->sides == curr);
	else if ((prev + half + 1) % r->sides == curr)
		return (1);
	else if (((prev - half - 1 + r->sides) % r->sides == curr))
		return (2);
	else
		return (0);
}

static inline double ghost_sides(t_game *r, int **vertices, double x, double y, int curr, int prev, int cond2, int ph1, int ph2)
{
	double distance;

	if (r->dist_ratio < 1)
		distance = get_dist(x, y, (vertices[curr][0] + vertices[prev][0]) * .5, \
		(vertices[curr][1] + vertices[prev][1]) * .5);
	else if (cond2 == 1)
	{
		distance = get_dist(x, y, (vertices[curr][0] + vertices[ph1][0]) * .5, \
		(vertices[curr][1] + vertices[ph1][1]) * .5);
	}
	else//cond2 == 2
	{
		distance = get_dist(x, y, (vertices[curr][0] + vertices[ph2][0]) * .5, \
		(vertices[curr][1] + vertices[ph2][1]) * .5);
	}
	//r->ghost = true;
	return (distance);
}

//recursively searches distance from nearest center, base depth on the zoom level?
//doesnt really do much of value for now, needs rework

static inline double recursive_center_ghost(int x, int y, int px, int py, int cx, int cy, int limit)
{
	int ncx, ncy;
	double d1, d2;

	if (limit == 0)
		return (get_dist(x, y, cx, cy));
		
	/* d1 = get_dist2(px, py, x, y);// doesnt seem to matter which is used
	d2 = get_dist2(px, py, cx, cy); */
	d1 = get_dist2(x, y,(cx + px) / 2, (cy + py) / 2);
	d2 = get_dist2(x, y, (3 * cx - px) / 2, (3 * cy - py) / 2);
    if (d1 <= d2)
	{// move center toward px
    	ncx = (cx + px) / 2;
		ncy = (cy + py) / 2;
		return (recursive_center_ghost(x, y, px, py, ncx, ncy, limit - 1));

    } 
	else 
	{// move center away from px
		ncx = (3 * cx - px) / 2;
		ncy = (3 * cy - py) / 2;
		return (recursive_center_ghost(x, y, cx, cy, ncx, ncy, limit - 1));//changed from px, py to cx, cy
	}
	//return (recursive_center_ghost(x, y, px, py, ncx, ncy, limit - 1));//old
}

static inline double ghost_center(t_game *r, int x, int y, int **verts, int prev, bool prev_ghost, int curr)// make a ghost center control op 1,2,3 etc
{
	int on = 0;//0 = no recur, 1 = recur, 2= use dist from prev //(1,2 when prev_ghost)
	int limit = 1;//> 1 is not very intersting
	int		cx, cy;
	double	dist;

	cx = r->center[0];
	cy = r->center[1];
	//(void)prev_ghost;
	int	px, py;
	px = verts[prev][0];
	py = verts[prev][1];

	(void)curr;

	//simple generalize to lands near or far on axis.
	//zoom is impacting?
	//sides on also activates pg, so with side and cen ghost on, 
	//with side ghost come on also, interesting to use without the  if else
	if (prev_ghost && on == 1)
		dist = recursive_center_ghost(x, y, px, py, cx, cy, limit);
	else if (prev_ghost && on == 2)
		dist = get_dist(x, y, px, py);//dist from prev, interesting, could try curr
	else
		dist = get_dist(x, y, cx, cy);//orig ft, basic center ghost
		
	r->ghost = true;//orig ft
	return (dist);
}

static inline bool condition3(t_game *r, int prev, int curr, int ph11, int ph21)
{
	if (r->dist_ratio < 1)
		return (ph11 == curr || ph21 == curr);
	else
		return (prev == curr);
}

static inline double get_depth_dist(t_game *r, double x, double y, int **vertices, int prev, int curr)
{
	double			distance;
	unsigned int 	color;
	int				cond1, cond2;
	int 			half, sides, ph1, ph2;
	bool			prev_ghost;

	prev_ghost = r->ghost;
	r->ghost = false;
	sides = r->sides;
	half = sides / 2;
	ph1 = (prev + half) % sides;
	ph2 = (prev - half + sides) % sides;
	cond1 = condition(r, prev, curr, ph1, ph2);
	cond2 = condition2(r, prev, curr, half);
	
	//how to switch on off? + or - key layer2? or just use col==4
	if (prev_ghost && r->ghost2)//works for in between center and vert. , try something for which side its on, close to vert, or center?
	{
		//needs to adjust with vertd
		distance = get_dist(x, y, (vertices[curr][0] + r->center[0]) * .5, (vertices[curr][1] + r->center[1]) * .5);
	}
	else if (cond1 && r->jump_to_center_col)//ghost center
		distance = ghost_center(r, x, y, vertices, prev, prev_ghost, curr);
	else if (cond2 && r->jump_to_sides_col)//ghost sides
		distance = ghost_sides(r, vertices, x, y, curr, prev, cond2, ph1, ph2);
	else
		distance = get_dist(x, y, vertices[curr][0], vertices[curr][1]);
	double rad = (double)r->r;
	if (r->dist_ratio > 1 && r->dist_ratio < 2)
		rad *= r->ratio_change;
	color = calc_color_4(distance, (r->max_distance / r->start_maxd) * rad * r->zoom, &r->colors);
	return (color);
}

unsigned int	color_depth_option(t_game *r, int v, double x, double y, int **vertices)
{
	unsigned int 	color;
	double			distance;

	if (r->color_depth == 0)
	{
		distance = get_dist(x, y, r->center[0], r->center[1]);//from center of shape
		//distance = get_dist(x, y, r->width / 2, r->height / 2);//from center of screen
		color = calc_color_4(distance, r->max_distance, &r->colors);
	}
	else if (r->color_depth == 1)
		color = get_depth_dist(r, x, y, vertices, r->prev, v);
	else if (r->color_depth == 2)
		color = get_depth_dist(r, x, y, vertices, r->obp, r->prev);
	else if(r->color_depth == 3)
		color = get_depth_dist(r, x, y, vertices, r->tbp, r->obp);
	else//(r->color_depth == 4)
		color = get_depth_dist(r, x, y, vertices, r->thbp, r->tbp);
	
	return (color);
}

static inline unsigned int set_wheel_center(t_game *r, int x, int y, int curr, int cenx, int ceny, bool g)
{
	double			col_x;
	double			col_y;
	unsigned int	color;
	
	col_x = ((x - cenx) + r->col_shift_x);//shape center
	col_y = ((y - ceny) + r->col_shift_y);
	r->ghost = g;
	//(void)curr;
	
	int				rot_adjust;
	rot_adjust = -(360 / r->sides) * (curr + 1) + 720;// rots each vert(i) * 2pi / sides;//all face in
	//rot_adjust = -(360 / (double)r->sides) * ((double)curr + curr % 2 + 1) + 720;//alternate 3 in 3 out
	

	//radial
	//color = get_color_source(col_x, col_y, r->w_colors, 360, r->color_rot + rot_adjust, 1);
	
	double 			factor;
	(!r->supersample) ? (factor = 1): (factor = 1 / (double)r->s_kernel);
	//spiral
	color = get_color_source_spiral(col_x, col_y, r->w_colors, 360, r->color_rot + rot_adjust, .0066 * factor);
	
	return (color);
}

//color wheel(s)

static inline unsigned int set_col_sources(t_game *r, double x, double y, int **vertices, int prev, int curr)
{
	
	unsigned int	color;
	int 			half, ph1, ph2, cond2;
	//bool			prev_ghost;

	//prev_ghost = r->ghost;
	r->ghost = false;
	half = r->sides / 2;
	ph1 = (prev + half) % r->sides;
	ph2 = (prev - half + r->sides) % r->sides;
	cond2 = condition2(r, prev, curr, half);
	if (r->jump_to_center_col && condition(r, prev, curr, ph1, ph2))
		color = set_wheel_center(r, x, y, curr, r->center[0], r->center[1], 1);
	else if (cond2 && r->jump_to_sides_col)//ghost sides
	{
		if (r->dist_ratio < 1)
			color = set_wheel_center(r, x, y, curr, (vertices[curr][0] + vertices[prev][0]) * .5, (vertices[curr][1] + vertices[prev][1]) * .5, 1);
		else if (cond2 == 1)
			color = set_wheel_center(r, x, y, curr, (vertices[curr][0] + vertices[ph1][0]) * .5, (vertices[curr][1] + vertices[ph1][1]) * .5, 1);
		else//cond2 == 2
			color = set_wheel_center(r, x, y, curr, (vertices[curr][0] + vertices[ph2][0]) * .5, (vertices[curr][1] + vertices[ph2][1]) * .5, 1);
	}
	else
		color = set_wheel_center(r, x, y, curr, vertices[curr][0], vertices[curr][1], 0);
	return (color);
	
}

void	color_option(t_game *r, int v, double x, double y, int **vertices)
{
	unsigned int		color;
	
	(void)vertices;
	
	if (!r->color_op)
		color = color_depth_option(r, v, x, y, r->vertices2);
	else if (r->color_op == 1)//color scource(wheel(s))
	{
		if (r->color_depth == 0)
		{
			double col_x;
			double col_y;
			/* double 	factor;
			(!r->supersample) ? (factor = 1): (factor = 1 / (double)r->s_kernel); */
			col_x = ((x - r->center[0]) + r->col_shift_x);//shape center
			col_y = ((y - r->center[1]) + r->col_shift_y);
		//	color = get_color_source_spiral(col_x, col_y, r->w_colors, 360, r->color_rot, .01 * factor);
			color = get_color_source(col_x, col_y, r->w_colors, 360, r->color_rot, 1);
		}
		else if (r->color_depth == 1)
			color = set_col_sources(r, x, y, r->vertices2, r->prev, v);
		else if (r->color_depth == 2)
			color = set_col_sources(r, x, y, r->vertices2, r->obp, r->prev);
		else if (r->color_depth == 3)
			color = set_col_sources(r, x, y, r->vertices2, r->tbp, r->obp);
		else//(r->color_depth == 4)
			color = set_col_sources(r, x, y, r->vertices2, r->thbp, r->tbp);
		r->ghost = false;
	}
	else//color_op = 2, texture, needs full build out mult txs to rotate between, gui
		color =	map_color(r->texture, r, x, y);//should be able to bring in 4 txs and do all interpolate and vertex color combos and depths.
	put_pixel((int)x, (int)y, r, color);		//maybe use range of txt and cubic interp closer to edges.
}
