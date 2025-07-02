#include "chaos.h"

double	get_delta_angle(double y, double x, double *start)
{
	double	angle;
	double	delta_angle;

	angle = atan2(y, x);
	
	(void)start, (void)angle;
	delta_angle = angle - *start;
	if (delta_angle > M_PI)
		delta_angle -= TWO_PI;
    else if (delta_angle < -M_PI)
		delta_angle += TWO_PI;
	//*start = angle;
	return (delta_angle);
}


void	set_color_chan(t_colors *colors, t_3color *curr_col, int num_col, int chan, int new_val)
{
	unsigned int color_out;

	if (chan == 0) curr_col->r = new_val;
	else if (chan == 1)curr_col->g = new_val;
	else if (chan == 2)curr_col->b = new_val;
	color_out = (curr_col->r << 16) | (curr_col->g << 8) | curr_col->b; 
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
		/* if (trace->low_flag)
			trace->low_inc = trace->m_lowinc;
		else
			trace->low_res = false;
		if (trace->menu_open && !trace->stash)
			reset_con(trace);
		else if (trace->menu_open)
			reset_con_non(trace);
		else if (!trace->stash) */
		intermed(game);
	}
	return (0);
}

double get_knob_pos(double max_d, double base_d)
{
	return 127.0 + ((max_d / base_d - 1.0) / 6.0) * 144.0;
}

double get_max_d(double new_val, double base_d)
{
	return ((new_val / 144) * 6 + 1) * base_d;
}

int mouse_move(int x, int y, t_game *game)
{
	double			delta_x;
	t_3color		color;
	int				current_knob_pos = 0;
	int 			new_knob_pos;
	double			new_val;
	int				knob;
	double			delta_angle;
	void			*connect = game->mlx_connect;
	void			*win = game->mlx_win;
	t_control		*con = game->con;

	knob = con->knob;
	if (con->dragging && game->con_open && knob != -1)
	{	
		delta_x = x - con->start_x;
		//handle color knobs
		if (knob >= 0 && knob < 3)
		{
			color = game->curr_col;
			if (knob == 0) current_knob_pos = 16 + color.r;
			else if (knob == 1) current_knob_pos = 16 + color.g;
			else if (knob == 2) current_knob_pos = 16 + color.b;
     		new_knob_pos= fmax(16, fmin(271, current_knob_pos + delta_x));
     		new_val = new_knob_pos - 16;
			 //change color with ft here...
			 set_color_chan(&game->colors, &color, con->col_num, con->knob, new_val);
		}
		if (knob == 3)//max_d
		{
			current_knob_pos = get_knob_pos(con->max_d, con->base_d);
			new_knob_pos = fmax(127, fmin(270, current_knob_pos + delta_x));
     		new_val = new_knob_pos - 127;
			double scaled_max_d = get_max_d(new_val, con->base_d);
			if (game->supersample)
				game->max_distance = scaled_max_d * game->s_kernel;
			else	
				game->max_distance = scaled_max_d;//((new_val / 144) * 6 + 1) * con->base_d;
			//con->max_d = game->max_distance;
		}
		if (knob == 4)//iter
		{
			delta_angle = get_delta_angle(432 - y, x - 200, &con->start_angle);//424 + 8 -y, x - 8 - 192, min the 8s for the rad of knob
			game->iters += delta_angle * (168354.0 / M_PI);
		}
		set_controls(game);
		mlx_put_image_to_window(connect, win, game->img.img_ptr, 0, 0);
		set_color_vals(connect, win, game);
	}
	con->start_x = x;
	return (0);
}