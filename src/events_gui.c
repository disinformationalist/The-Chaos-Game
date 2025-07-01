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

void	check_knobs(int x, int y, t_game *game)
{
	t_3color	color;
	
	color = game->curr_col;
	//color
	if (in_circle(x, y, 23 + color.r, 112, 8))
		set_drag(game, x, 0);
	else if (in_circle(x, y, 23 + color.g, 137, 8))
		set_drag(game, x, 1);
	else if (in_circle(x, y, 23 + color.b, 162, 8))
		set_drag(game, x, 2);
}

void	switch_tab(t_img *img, int start, int prev_start)
{
	int	end;

	end = start + 69;
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
	//(void)con;
	game->on_con = true;
	if ((x >= 7 && x <= 76) && (y > 70 && y <= 92)) // start 6, 70 ;  70x23
	{
		switch_tab(game->con->color_con, 6, 7 + (con->col_num - 1) * 69);
		con->col_num = 1;
		//update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if ((x >= 77 && x <= 146) && (y > 70 && y <= 92))
	{
		switch_tab(game->con->color_con, 75, 7 + (con->col_num - 1) * 69);
		con->col_num = 2;
	}
	else if ((x >= 147 && x <= 216) && (y > 70 && y <= 92))
	{
		switch_tab(game->con->color_con, 144, 7 + (con->col_num - 1) * 69);
		con->col_num = 3;
	}
	else if ((x >= 217 && x <= 286) && (y > 70 && y <= 92))
	{
		switch_tab(game->con->color_con, 213, 7 + (con->col_num - 1) * 69);
		con->col_num = 4;
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
