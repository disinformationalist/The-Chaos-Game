#include "chaos.h"

void	zoom_iters_mouse_move(t_game *r)
{
	double rad_times_zoom;
	
	r->col_shift_x = ft_round((double)r->col_shift_x / r->con->old_zoom);
	r->col_shift_y = ft_round((double)r->col_shift_y / r->con->old_zoom);
	rad_times_zoom = r->r * r->zoom;
	if (r->dist_ratio <= .5)
		r->iters = .5 * (8 + r->iters_change) * SQ(r->dist_ratio * rad_times_zoom) * r->area_factor;
	else if (r->dist_ratio < 1.0 && r->dist_ratio > .5)
		r->iters = .5 * (8 + r->iters_change) * SQ((1 - r->dist_ratio) * rad_times_zoom) * r->area_factor;
	else //(r->dist_ratio > 1)
		r->iters = .5 * (8 + r->iters_change) * SQ(rad_times_zoom * r->ratio_change * (r->dist_ratio - 1)) * r->area_factor;
	r->col_shift_x = ft_round((double)r->col_shift_x * r->zoom);
	r->col_shift_y = ft_round((double)r->col_shift_y * r->zoom);
}

static inline Rect rect_union(Rect a, Rect b)
{
    int x1 = (a.x < b.x) ? a.x : b.x;
    int y1 = (a.y < b.y) ? a.y : b.y;

    int x2 = ((a.x + a.w) > (b.x + b.w)) ? (a.x + a.w) : (b.x + b.w);
    int y2 = ((a.y + a.h) > (b.y + b.h)) ? (a.y + a.h) : (b.y + b.h);

    Rect r;
    r.x = x1;
    r.y = y1;
    r.w = x2 - x1;
    r.h = y2 - y1;
    return r;
}

void	draw_knob(t_game *r, t_control *con, t_img *knob, int x, int y)
{
	int 			i, j;
	unsigned int	color;

	j = -1;
	while (++j < con->k_height)
	{
		i = -1;
		while (++i < con->k_width)
		{
			color = pixel_color_get3(i, j , knob);
			if (color != 0xFF202020)//later if uint8_t trans = (color >> 24), if (trans == 255), else if (trans != 0) blend with lerp
				my_pixel_put(x + i, y + j, &r->img, color);
		}
	}
}

// takes the smallest pos. union rect from knobs before and afer pos, then draws bottom image that size and draws knob on top.
void	reset_track(t_game *r, t_control *con, t_img *panel, t_img *knob, int x1, int y1, int x2, int y2)
{
	int i, j;
	Rect a, b;

	//start knob pos
	a.x = x1;
	a.y = y1;
	a.w = 17;
	a.h = 17;
	//end knob pos
	b.x = x2;
	b.y = y2;
	b.w = 17;
	b.h = 17;

	Rect Union = rect_union(a, b);
	j = Union.y - 1;
	while (++j < Union.y + Union.h)
	{
		i = Union.x - 1;
		while (++i < Union.x + Union.w)
			my_pixel_put(i , j,  &r->img, pixel_color_get3(i , j , panel));
	}
	//redraw at new pos
	draw_knob(r, con, knob, x2, y2);
	mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);
}

double	get_delta_angle(double y, double x, double start)
{
	double	angle;
	double	delta_angle;

	angle = atan2(y, x);
	delta_angle = angle - start;
	if (delta_angle > M_PI)
		delta_angle -= TWO_PI;
    else if (delta_angle < -M_PI)
		delta_angle += TWO_PI;
	return (delta_angle);
}

void	set_color_chan(t_colors *colors, t_4color *curr_col, int num_col, int chan, int new_val)
{
	unsigned int color_out;

	if (chan == 0) curr_col->r = new_val;
	else if (chan == 1)curr_col->g = new_val;
	else if (chan == 2)curr_col->b = new_val;
	else if (chan == 3)curr_col->t = new_val;
	color_out = (curr_col->t << 24) | (curr_col->r << 16) | (curr_col->g << 8) | curr_col->b;

	if (num_col == 1)
		colors->color_1 = color_out;
	else if (num_col == 2)
		colors->color_2 = color_out;
	else if (num_col == 3)
		colors->color_3 = color_out;
	else if (num_col == 4)
		colors->color_4 = color_out;
	else //(num_col == 5)
		colors->background = color_out;
}

int	mouse_release(int button, int x, int y, t_game *game)
{
	t_control *con;

	(void)x;
	(void)y;
	con = game->con;
	if (button == 1 && con->dragging)
	{
		con->dragging = false;
			con->knob = -1;
		render(game);
	}
	return (0);
}

double get_knob_pos(double max_d, double base_d)
{
	return (127.0 + ((max_d / base_d - 1.0) / 6.0) * 144.0);
}

double get_max_d(double new_val, double base_d)
{
	return (((new_val / 144) * 6 + 1) * base_d);
}

static inline double	move_knob(int knob_min, int knob_max, int current_position, int delta_x)
{
	int		new_position;
	double	new_value;
	
	new_position = fmax(knob_min, fmin(knob_max, current_position + delta_x));
    new_value = new_position - knob_min;
	return (new_value);
}

//replaces minimum to reset using the union of knob pos rectangles(before + after locations)

t_knob	update_dial_knob(t_game *game, t_control *con, t_img *panel, t_knob knob, double pos, int flag, double rad, double angle)
{
	int oldx, oldy, newx, newy;
	
	
	if (flag == 1)
	{
		if (game->supersample)
			pos /= game->s_kernel;
		pos = fmod(pos, 360) * DEG_TO_RAD;
	}
	else if (flag == 2)
		pos = fmod(pos, 3) * (TWO_PI_THIRDS);
	else if (flag == 3)
	{
		if (game->supersample)
			pos /= SQ(game->s_kernel);
		pos = fmod(pos, 336708) * ITER_FACTOR;
	}
	if (flag == 4)
		pos = fmod(pos, 360) * DEG_TO_RAD;
	knob.angle = pos;
	oldx = knob.posx;
	oldy = knob.posy;
	newx = ft_round(rad * cos(pos + angle)) + knob.cx;
	newy = knob.cy - ft_round(rad * sin(pos + angle));
	knob.posx = newx;
	knob.posy = newy;
	reset_track(game, con, panel, knob.img, oldx, oldy, newx, newy);
	return (knob);
}

static inline void		adjust_nav(t_game *game, int knob, int x, int y, double delta_angle, t_control *con, void *mlx, void *win)
{
	if (knob == 7)//posx
	{
		delta_angle = get_delta_angle(199 - y, x - 131, con->knobs.posx.angle);
		game->move_x += (delta_angle - PI_HALVES) * (RAD_TO_DEG);
		game->move_x = ft_round(game->move_x);
		con->knobs.posx = update_dial_knob(game, con, con->nav, con->knobs.posx, game->move_x, 1, 60, PI_HALVES);
	}
	else if (knob == 8)//posy
	{
		delta_angle = get_delta_angle(199 - y, x - 270, con->knobs.posy.angle);
		game->move_y += (delta_angle - PI_HALVES) * (RAD_TO_DEG);
		game->move_y = ft_round(game->move_y);
		con->knobs.posy = update_dial_knob(game, con, con->nav, con->knobs.posy, game->move_y, 1, 60, PI_HALVES);
	}
	else if (knob == 9)//rot
	{
		delta_angle = get_delta_angle(319 - y, x - 200, game->rotate);
		rotate(game, (delta_angle - PI_HALVES));
		con->knobs.rot = update_dial_knob(game, con, con->nav, con->knobs.rot, game->rotate, 0, 60, PI_HALVES);
	}
	else if (knob == 10)//zoom must adjust iters
	{
		delta_angle = get_delta_angle(239 - y, x - 200, con->knobs.zoom.angle);
		con->old_zoom = game->zoom;
		zoom_iters_mouse_move(game);
		game->zoom += (delta_angle + PI_SIXTHS) * THREE_OVER_2PI;
		con->knobs.zoom = update_dial_knob(game, con, con->nav, con->knobs.zoom, game->zoom, 2, 161, -PI_SIXTHS);
	}
	set_nav_vals(mlx, win, game, 0);
}

static inline void		adjust_w_nav(t_game *game, int knob, int x, int y, double delta_angle, t_control *con, void *mlx, void *win)
{
	if (knob == 11)//w_posx
	{
		delta_angle = get_delta_angle(199 - y, x - 131, con->knobs.w_posx.angle);
		game->col_shift_x += ft_round((delta_angle - PI_HALVES) * (RAD_TO_DEG));
		con->knobs.w_posx = update_dial_knob(game, con, con->w_nav, con->knobs.w_posx, game->col_shift_x, 1, 60, PI_HALVES);
	}
	else if (knob == 12)//w_posy
	{
		delta_angle = get_delta_angle(199 - y, x - 270, con->knobs.w_posy.angle);
		game->col_shift_y += ft_round((delta_angle - PI_HALVES) * (RAD_TO_DEG));
		con->knobs.w_posy = update_dial_knob(game, con, con->w_nav, con->knobs.w_posy, game->col_shift_y, 1, 60, PI_HALVES);
	}
	else if (knob == 13)//w_rot
	{
		delta_angle = get_delta_angle(319 - y, x - 200, con->knobs.w_rot.angle);
		game->color_rot += ft_round((delta_angle - PI_HALVES) * (RAD_TO_DEG));
		game->color_rot %= 360;
		con->knobs.w_rot = update_dial_knob(game, con, con->w_nav, con->knobs.w_rot, game->color_rot, 4, 60, PI_HALVES);
	}
	else if (knob == 10)//zoom must adjust iters knob
	{
		delta_angle = get_delta_angle(239 - y, x - 200, con->knobs.zoom.angle);
		con->old_zoom = game->zoom;
		game->zoom += (delta_angle + PI_SIXTHS) * THREE_OVER_2PI;
		zoom_iters_mouse_move(game);
		con->knobs.zoom = update_dial_knob(game, con, con->w_nav, con->knobs.zoom, game->zoom, 2, 161, -PI_SIXTHS);
	}
	set_nav_vals(mlx, win, game, 1);
}
void	set_maxd(t_game *game, double new_val, double base_d)
{
	double scaled_max_d = get_max_d(new_val, base_d);
	if (game->supersample)
		game->max_distance = scaled_max_d * game->s_kernel;
	else	
		game->max_distance = scaled_max_d;
}

int mouse_move(int x, int y, t_game *game)
{
	void			*connect = game->mlx_connect;
	void			*win = game->mlx_win;
	t_control		*con = game->con;
	t_4color		color;
	t_img			*k_img;
	double			delta_angle;
	double			delta_x;
	double			new_val;
	int				current_knob_pos = 0;
	int				knob, new, old, yval;

	knob = con->knob;
	if (con->dragging && game->con_open && knob != -1)//These only actually need to change value on mouse release unless realtime render
	{	
		delta_x = x - con->start_x;
		//handle color knobs
		if (knob >= 0 && knob < 4)
		{
			color = game->curr_col;
			if (knob == 0)
				current_knob_pos = 14 + color.r, yval = 103, k_img = con->r;
			else if (knob == 1)
				current_knob_pos = 14 + color.g, yval = 128, k_img = con->g;
			else if (knob == 2)
				current_knob_pos = 14 + color.b, yval = 153, k_img = con->b;
			else if (knob == 3)
				current_knob_pos = 14 + color.t, yval = 178, k_img = con->s;

			new_val = move_knob(14, 269, current_knob_pos, delta_x);
			new = ft_round(new_val);
			set_color_chan(&game->colors, &game->curr_col, con->col_num, con->knob, new);
			reset_track(game, con, con->color_con, k_img, current_knob_pos, yval, new + 14, yval);
		}
		else if (knob == 4)//max_d
		{
			old = ft_round((con->max_d - con->base_d) / (6 * con->base_d) * 144) + 119;
			new_val = move_knob(127, 271, get_knob_pos(game->con->max_d, game->con->base_d), delta_x);
			set_maxd(game, new_val, game->con->base_d);
			if (game->supersample)
				game->con->max_d = game->max_distance / game->s_kernel;
			else
				game->con->max_d = game->max_distance;
			new = ft_round((game->con->max_d - game->con->base_d) / (6 * game->con->base_d) * 144) + 119;
			yval = 243;
			reset_track(game, con, con->color_con, con->s, old , yval, new, yval);
		}
		else if (knob == 5)//vert_d
		{
     		old = 141 + ft_round(game->con->vert_d * 100);
			new_val = move_knob(149, 249, old + 8, delta_x);
			new = ft_round(new_val) + 141, yval = 273;
			new_val *= .01;
			if (fabs(new_val) < 1e-6)//protect against 0 vert dist
				new_val += 1e-6;
			
			game->vert_dist = new_val;
			game->con->vert_d = new_val;

			current_knob_pos = get_knob_pos(game->con->max_d, game->con->base_d);//max_distance knob pos

			if (game->supersample)
				game->con->base_d = game->start_maxd * new_val / game->s_kernel;
			else	
				game->con->base_d = game->start_maxd * new_val;
				
			set_maxd(game, current_knob_pos - 127, con->base_d);
			if (game->supersample)
				game->con->max_d = game->max_distance / game->s_kernel;
			else
				game->con->max_d = game->max_distance;

			reset_track(game, con, con->color_con, con->s, old, yval, new, yval);
		}
		else if (knob == 6)//iter
		{
			delta_angle = get_delta_angle(432 - y, x - 200, con->knobs.iter.angle);
			game->iters += ft_round(delta_angle * 53588.74258);//(168354.0 / M_PI)
			con->knobs.iter = update_dial_knob(game, con, con->color_con, con->knobs.iter, game->iters, 3, 120, 0);
			if (game->supersample)
				game->con->iters = game->iters / (game->s_kernel * game->s_kernel);	
			else
				game->con->iters = game->iters;
		}
		// Navigation knobs start here
		if (con->nav_open)
			adjust_nav(game, knob, x, y, delta_angle, con, connect, win);
		else if (con->nav_w_open)
			adjust_w_nav(game, knob, x, y, delta_angle, con, connect, win);
		else
			set_color_vals(connect, win, game);
	}
	con->start_x = x;
	return (0);
}
