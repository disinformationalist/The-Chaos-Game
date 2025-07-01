#include "chaos.h"

/* void	reset_track(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	j = -1;
	while (++j < control.ct_height)
	{
		i = -1;
		while (++i < control.ct_width)
		{
			color = pixel_color_get3(i, j , control.ct);
				my_pixel_put(9 + i, move_y + j, img, color);
		}
	}
} */


/* void	set_rknob(t_img *img, t_control control, t_3color color)
{
	int				i;
	int				j;
	unsigned int	k_color;
	int				x_startr;
	int				y_startr;

	x_startr = 14 + color.r;
	y_startr = 103;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			k_color = pixel_color_get3(i, j , control.r);
			if (k_color != 0xFF202020)
				my_pixel_put(x_startr + i, y_startr + j, img, k_color);
		}
	}
} */


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
	else //(num_col == 4)
		colors->color_4 = color_out;
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


int mouse_move(int x, int y, t_game *game)
{
	double			delta_x;
//	double			delta_y;
//	t_vec3			move;
	t_3color		color;
	int				current_knob_pos = 0;
	int 			new_knob_pos;
	double			new_val;
	int				knob;
/* 	double			angle;
	double			delta_angle; */
	void			*connect = game->mlx_connect;
	void			*win = game->mlx_win;
	t_control		*con = game->con;
	//t_control		cont = *trace->obj_control;
	//t_img			*img = con->color_con;
	
	(void)y;
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
		set_controls(game);
		mlx_put_image_to_window(connect, win, game->img.img_ptr, 0, 0);
		set_color_vals(connect, win, game);
	}
	con->start_x = x;
	return (0);
}