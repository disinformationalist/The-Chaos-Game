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

//NOTE: 'v' can be center point, verts in the rv[] array cannot.

unsigned int	color_depth_option(t_game *r, int v, double x, double y, int **vertices)
{
	unsigned int 	color;
	double			distance;
	(void)v;
		//distance = sqrt((x - r->center[0]) * (x - r->center[0]) + (y - r->center[1]) * (y - r->center[1]));//maybe make from center TRY prob with boundries...
	if (r->color_depth == 0)
		distance = sqrt((x - r->width / 2) * (x - r->width / 2) + (y - r->height / 2) * (y - r->height / 2));
	else if (r->color_depth == 1)
	{
		distance = get_distance(x, y, vertices[r->rv[(r->i) % r->rv_len]][0], vertices[r->rv[(r->i) % r->rv_len]][1]);
		
		/* if (r->jump_to_center && (v == r->points - 1))
			distance = sqrt((x - r->center[0]) * (x - r->center[0]) + (y - r->center[1]) * (y - r->center[1]));
		else
			distance = get_distance(x, y, vertices[v][0], vertices[v][1]);//dist from vert, NOT BAD TRY VARIATION LIKE DIST */ //FROM SIDES of screen!!--GReaT WITH FT MODES!!!!!!	
	}
	else if (r->color_depth == 2)
		distance = get_distance(x, y, vertices[r->rv[(r->i - 1) % r->rv_len]][0], vertices[r->rv[(r->i - 1) % r->rv_len]][1]);//may need to modify better with similar 4 cols
	else if (r->color_depth == 3)
		distance = get_distance(x, y, vertices[r->rv[(r->i - 2) % r->rv_len]][0], vertices[r->rv[(r->i - 2) % r->rv_len]][1]);
	else
		distance = r->max_distance;//just a placholder for no compil err
	if (r->color_depth == 0)
		color = calc_color_4(distance, r->max_distance, &r->colors);
	else
		color = calc_color_4(distance ,  r->max_distance * 1.3 , &r->colors);// dist form verts, max radius, then zoom, was 1.3 * max_d
	//color = calc_color_4(distance, r->r * r->zoom, &r->colors);// dist form verts, max radius, then zoom, this is the correct inversion. sometimes above looks better.
	
	return (color);
}

void	color_option(t_game *r, int v, double x, double y, int **vertices)
{
	unsigned int		color;
	
	if (!r->color_op)
		color = color_depth_option(r, v, x, y, vertices);
	else//color scource and other ops...
	{
		double col_x;
		double col_y;

		if (r->supersample)
		{
			col_x = ((x - r->width / 2) + r->col_shift_x * r->s_kernel);
			col_y = ((y - r->height / 2) + r->col_shift_y * r->s_kernel);
		}
		else
		{
			col_x = ((x - r->width / 2) + r->col_shift_x);
			col_y = ((y - r->height / 2) + r->col_shift_y);
		}
		color = get_color_source( col_x, col_y, r->w_colors, 360, r->color_rot);
	
	/* 	bv = r->rv[(r->i - 1) % r->rv_len];
	//	distance = get_distance(x, y, vertices[bv][0], vertices[bv][1]);//previous vert
		distance = get_distance(vertices[bv][0], vertices[bv][1], vertices[v][0], vertices[v][1]);//previous vert
	
	unsigned int color1 = r->w_colors[r->rv[(r->i - 1) % r->rv_len]];
	unsigned int color2 = r->w_colors[v];
	color = interpolate_two_colors(assign_color_rgb(color1), assign_color_rgb(color2), (int)(255 * (distance / ( 2 * r->r)))); */
		

	//color = r->w_colors[r->rv[(r->i - depth) % r->rv_len]];	//diff palates for this...vertice depth orig
	}
	
	put_pixel((int)x, (int)y, r, color); 
}
	//		distance_to_vert = get_distance(x, y, vertices[v][0], vertices[v][1]);// current vert

	//log_density = (log_density / log(highest)) * 255.0; //USING INTERPOLATE HERE. VAL = distance to vert/ max distance

	//color = interpolate_two_colors(color1, color2, (int)log_density);


void	put_pixel(int x, int y, t_game *r, unsigned int color)
{
 	if (!r->supersample)
		my_pixel_put(x, y, &r->img, color);
	else
		r->pixels_xl[y][x] = color;
}

static void	assign_colors(int colors_arr[4], t_colors *colors)
{
	colors_arr[0] = colors->color_1;
	colors_arr[1] = colors->color_2;
	colors_arr[2] = colors->color_3;
	colors_arr[3] = colors->color_4;
}

static int	calculate_color_component(int color_low, \
int color_high, double t, int shift)
{
	return ((int)((1 - t) * ((color_low >> shift) & 0xFF) + t * \
	((color_high >> shift) & 0xFF)) << shift);
}

static int	interpolate_colors(int color_low, int color_high, double t)
{
	int	red;
	int	green;
	int	blue;

	red = calculate_color_component(color_low, color_high, t, 16);
	green = calculate_color_component(color_low, color_high, t, 8);
	blue = calculate_color_component(color_low, color_high, t, 0);
	return (red | green | blue);
}

int	calc_color_4(double distance, double max_distance, t_colors *colors)
{
	t_color_vars	vars;
	double			norm_distance;
	double			color_index_d;
	double			t;
	int				colors_arr[4];

	vars.num_colors = 4;
	norm_distance = distance / max_distance;
	color_index_d = norm_distance * (vars.num_colors - 1);
	vars.color_index_low = (int)color_index_d;
	vars.color_index_high = vars.color_index_low + 1;
	assign_colors(colors_arr, colors);
	t = color_index_d - vars.color_index_low;
	vars.color_low = colors_arr[vars.color_index_low];
	vars.color_high = colors_arr[vars.color_index_high];
	vars.color = interpolate_colors(vars.color_low, vars.color_high, t);
	return (vars.color);
}
