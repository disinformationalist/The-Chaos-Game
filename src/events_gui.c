#include "chaos.h"

static inline bool	in_circle(int x, int y, int cx, int cy, int rad)
{	
	double	dx;
	double	dy;
	double	dist2;

	dx = x - cx;
	dy = y - cy;
	dist2 = dx * dx + dy *dy;
	return (dist2 <= rad * rad);
}

static inline void	set_drag(t_game *game, int x, int knob)
{
	t_control *con;

	con = game->con;
	con->start_x = x;
	con->dragging = true;
	con->knob = knob;
}

static inline void	set_iter_knob(t_game *r, int knob, double *start)//, double angle)//TODO THESE 2 	
{
	r->con->dragging = true;
	*start = (double)(r->iters % 336708) * (M_PI / (double)168354);
	//*start = fmod(angle, 10) * (M_PI / 5);
	r->con->knob = knob;
}
/* x_startdist = 119 + (cont->max_d - cont->base_d) / (6 * cont->base_d) * 144;
	y_startdist = 218; */
void	check_knobs(int x, int y, t_game *game)//need a knob 3 for the dist
{
	t_3color	color;
	t_control	control = *game->con;
	
	color = game->curr_col;
	//color
	if (in_circle(x, y, 23 + color.r, 112, 8))
		set_drag(game, x, 0);
	else if (in_circle(x, y, 23 + color.g, 137, 8))
		set_drag(game, x, 1);
	else if (in_circle(x, y, 23 + color.b, 162, 8))
		set_drag(game, x, 2);
	else if (in_circle(x, y,  127 + (control.max_d - control.base_d) / (6 * control.base_d) * 144, 226, 8))//max_d
		set_drag(game, x, 3);
	else if (in_circle(x, y, control.iters_i + 8, control.iters_j + 8, 8))//iters dial knob
		set_iter_knob(game, 4, &game->con->start_angle);//, &trace->start_xangle, control.poss.x );

}

//switch between color tabs

void	switch_tab(t_img *img, int start, int prev_start)
{
	int	end;

	end = start + 55;
	while (++start < end)
	{
		my_pixel_put(start, 92, img, 0x202020);
		my_pixel_put(prev_start, 92, img, 0x90C4FF);
		prev_start++;
	}
}

int con_press(int x, int y, t_game *game, t_control *con)
{
	void			*connect = game->mlx_connect;
	void			*win = game->mlx_win;
	int				num;

	game->on_con = true;
	num = con->col_num;
	if ((x >= 7 && x <= 62) && (y > 70 && y <= 92)) // start 6, 70 ;  70x23
	{
		if (num != 1)
			switch_tab(game->con->color_con, 6, 7 + (num - 1) * 55);
		con->col_num = 1;
	}
	else if ((x >= 63 && x <= 118) && (y > 70 && y <= 92))
	{
		if (num != 2)
			switch_tab(game->con->color_con, 61, 7 + (num - 1) * 55);
		con->col_num = 2;
	}
	else if ((x >= 119 && x <= 174) && (y > 70 && y <= 92))
	{
		if (num != 3)
			switch_tab(game->con->color_con, 116, 7 + (num - 1) * 55);
		con->col_num = 3;
	}
	else if ((x >= 175 && x <= 230) && (y > 70 && y <= 92))
	{
		if (num != 4)
			switch_tab(game->con->color_con, 171, 7 + (num - 1) * 55);
		con->col_num = 4;
	}
	else if ((x >= 231 && x <= 286) && (y > 70 && y <= 92))
	{
		if (num != 5)
			switch_tab(game->con->color_con, 226, 7 + (num - 1) * 55);
		con->col_num = 5;
	}
	else if ((x >= 127 && x <= 196) && (y > 186 && y <= 209))
	{
		game->color_depth++;
		if (game->color_depth > 3)
			game->color_depth = 0;
		intermed(game);
	}
	else if ((x >= 203 && x <= 272) && (y > 186 && y <= 209))
	{
		game->color_depth--;
		if (game->color_depth < 0)
			game->color_depth = 3;
		intermed(game);
	}
	else
		check_knobs(x, y, game);
	set_controls(game);
	mlx_put_image_to_window(connect, win, game->img.img_ptr, 0, 0);
	set_color_vals(connect, win, game);
	
	/* //TODO Depth up down
	else if ((x >= 172 && x <= 274) && (y > 390 && y <= 413)) 
	{
		toggle_shadow(trace, trace->on);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	} */
	return (0);
}
