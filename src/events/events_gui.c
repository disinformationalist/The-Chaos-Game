#include "chaos.h"

static inline bool	in_circle(int x, int y, int cx, int cy, int rad)
{	
	double	dx;
	double	dy;
	double	dist2;

	dx = x - cx;
	dy = y - cy;
	dist2 = SQ(dx) + SQ(dy);
	return (dist2 <= SQ(rad));
}

static inline void	set_drag(t_game *game, int x, int knob)
{
	t_control *con;

	con = game->con;
	con->start_x = x;
	con->dragging = true;
	con->knob = knob;
}

static inline void	set_dial_knob(t_game *r, int knob)
{
	r->con->dragging = true;
	r->con->knob = knob;
}


int check_color_knobs(int x, int y, t_game *r)
{
	t_4color	color;
	
	color = r->curr_col;
	if (r->color_op != 1)
	{
		if (in_circle(x, y, 23 + color.r, 112, 8))
			return (set_drag(r, x, 0), 1);
		else if (in_circle(x, y, 23 + color.g, 137, 8))
			return (set_drag(r, x, 1), 1);
		else if (in_circle(x, y, 23 + color.b, 162, 8))
			return (set_drag(r, x, 2), 1);
		else if (in_circle(x, y, 23 + color.t, 187, 8))
			return (set_drag(r, x, 3), 1);
	}
	else 
	{
		if (in_circle(x, y, 150 + ft_round(r->wheel->saturation * 100), 112, 8))
			return (set_drag(r, x, 14), 1);
		else if (in_circle(x, y, 150 + ft_round(r->wheel->lightness * 100), 137, 8))
			return (set_drag(r, x, 15), 1);
		else if (in_circle(x, y, 23 + r->wheel->trans, 187, 8))
			return (set_drag(r, x, 16), 1);
	}
	return 0;
}



int	check_knobs(int x, int y, t_game *game)
{
	t_control	control = *game->con;

	if (check_color_knobs(x, y, game))
		return (1);
	else if (in_circle(x, y,  127 + (control.max_d - control.base_d) / (6 * control.base_d) * 144, 251, 8))//max_d
		set_drag(game, x, 4);
	else if (in_circle(x, y,  149 + control.vert_d * 100, 283, 8))//vert_d
		set_drag(game, x, 5);
	else if (in_circle(x, y, control.knobs.iter.posx + 9, control.knobs.iter.posy + 9, 8))//iters dial knob
		set_dial_knob(game, 6);
	if (game->con->knob == -1)
		return (1);
	return (0);
}


//Check if a nav click is on a knob and set for adjusting

void	check_nav_knobs(int x, int y, t_game *r, t_control *con)//set all with set dial knob and see if can cache val sooner
{
	if (con->nav_open)
	{
		if (in_circle(x, y, con->knobs.posx.posx + 9, con->knobs.posx.posy + 9, 8))
			set_dial_knob(r, 7);
		if (in_circle(x, y, con->knobs.posy.posx + 9, con->knobs.posy.posy + 9, 8))
			set_dial_knob(r, 8);
		if (in_circle(x, y, con->knobs.rot.posx + 9, con->knobs.rot.posy + 9, 8))
			set_dial_knob(r, 9);
	}
	else if (con->nav_w_open)
	{
		if (in_circle(x, y, con->knobs.w_posx.posx + 9, con->knobs.w_posx.posy + 9, 8))
			set_dial_knob(r, 11);
		if (in_circle(x, y, con->knobs.w_posy.posx + 9, con->knobs.w_posy.posy + 9, 8))
			set_dial_knob(r, 12);
		if (in_circle(x, y, con->knobs.w_rot.posx + 9, con->knobs.w_rot.posy + 9, 8))
			set_dial_knob(r, 13);
	}
	if (in_circle(x, y, con->knobs.zoom.posx + 9, con->knobs.zoom.posy + 9, 8))
			set_dial_knob(r, 10);
}
//switch between color tabs

void	switch_tab(t_img *img, t_img *whole, int start, int prev_start)
{
	int	end;

	end = start + 55;
	while (++start < end)
	{
		my_pixel_put(start, 92, img, 0x202020);//open
		my_pixel_put(prev_start, 92, img, 0x90C4FF);//close
		my_pixel_put(start, 92, whole, 0x202020);//open
		my_pixel_put(prev_start, 92, whole, 0x90C4FF);//close
		prev_start++;
	}
}

//manages tab click, switches if tab clicked is not already the open one

static inline void	select_tab(int num, t_game *r, int tab, int val)
{
	if (num != tab)
	{
		switch_tab(r->con->color_con, &r->img, val, 7 + (num - 1) * 55);
		r->con->col_num = tab;
	}
}

static inline void	set_knob_vals(t_knob *knob, double rad, double angle, double shift)
{
	knob->angle = angle;
	angle += shift;
	knob->posx = ft_round(rad * cos(angle)) + knob->cx;
	knob->posy = knob->cy - ft_round(rad * sin(angle));
}

void	set_nav_knob_pos(t_game *r, t_control *con)
{
	double		posx, posy, zoom, sup;
	int			desired_zoom = 3;

	//set rot
	set_knob_vals(&(con->knobs.rot), 60, r->rotate, PI_HALVES);
	//set xy
	posx = r->move_x;
	posy = r->move_y;
	if (r->supersample)
	{
		sup = 1 / (double)r->s_kernel;
		posx *= sup;
		posy *= sup;
	}
	//becomes angle x, y
	//angle = fmod(posx, 360) * DEG_TO_RAD;//value % desired_len_of circumference * (2 * pi / desired_len)[conversion factor]
	set_knob_vals(&(con->knobs.posx), 60, fmod(posx, 360) * DEG_TO_RAD, PI_HALVES);
	set_knob_vals(&(con->knobs.posy), 60, fmod(posy, 360) * DEG_TO_RAD, PI_HALVES);

	//set the zoom, = 1 is the starting pos at the top.
	zoom = r->zoom;
	set_knob_vals(&(con->knobs.zoom), 161, fmod(zoom, desired_zoom) * (TWO_PI_THIRDS), -PI_SIXTHS);
}

void	set_nav_knobs(t_game *r, t_control *con)
{
	int				i;
	int				j;
	int				rotx;
	int				roty;
	int				posxx;
	int				posxy;
	int				posyx;
	int				posyy;
	int				zoomx;
	int				zoomy;
	unsigned int	color;

	rotx = con->knobs.rot.posx;
	roty = con->knobs.rot.posy;
	posxx = con->knobs.posx.posx;
	posxy = con->knobs.posx.posy;
	posyx = con->knobs.posy.posx;
	posyy = con->knobs.posy.posy;
	zoomx = con->knobs.zoom.posx;
	zoomy = con->knobs.zoom.posy;
	j = -1;
	while(++j < con->k_height)
	{
		i = -1;
		while (++i < con->k_width)
		{
			color = pixel_color_get3(i, j, con->r);
			if (color != 0xFF202020)
				my_pixel_put(rotx + i, roty + j, &r->img, color);

			color = pixel_color_get3(i, j, con->g);
			if (color != 0xFF202020)
				my_pixel_put(posxx + i, posxy + j, &r->img, color);

			color = pixel_color_get3(i, j, con->b);
			if (color != 0xFF202020)
				my_pixel_put(posyx + i, posyy + j, &r->img, color);

			color = pixel_color_get3(i, j, con->s);//zoom
			if (color != 0xFF202020)
				my_pixel_put(zoomx + i, zoomy + j, &r->img, color);
	
		}
	}
}


//set the navigation window

void	set_nav(t_game *r)
{
	t_control		control;
	int				i;
	int				j;
	int				k;

	control = *r->con;
	j = -1;
	k = 0;
	while(++j < control.nav_h)
	{
		i = -1;
		while (++i < control.nav_w)
			my_pixel_put(i, j, &r->img, pixel_color_get3(i, k, control.nav));
		k++;
	}
	// set the dials knobs
	set_nav_knob_pos(r, r->con);
	set_nav_knobs(r, r->con);
	mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);
	set_nav_vals(r->mlx_connect, r->mlx_win, r, 0);
}

void	set_nav_w_knob_pos(t_game *r, t_control *con)
{
	double		posx, posy, zoom, sup;
	int			desired_zoom = 3;
	
	set_knob_vals(&(con->knobs.w_rot), 60, fmod((double)r->color_rot, 360) * DEG_TO_RAD, PI_HALVES);
	posx = (double)r->col_shift_x;
	posy = (double)r->col_shift_y;
	if (r->supersample)
	{
		sup = 1 / (double)r->s_kernel;
		posx *= sup;
		posy *= sup;
	}
	set_knob_vals(&(con->knobs.w_posx), 60, fmod(posx, 360) * DEG_TO_RAD, PI_HALVES);
	set_knob_vals(&(con->knobs.w_posy), 60, fmod(posy, 360) * DEG_TO_RAD, PI_HALVES);

	zoom = r->zoom;
	set_knob_vals(&(con->knobs.zoom), 161, fmod(zoom, desired_zoom) * (TWO_PI_THIRDS), -PI_SIXTHS);
}

void	set_nav_w_knobs(t_game *r, t_control *con)
{
	int				i;
	int				j;
	int				rotx;
	int				roty;
	int				posxx;
	int				posxy;
	int				posyx;
	int				posyy;
	int				zoomx;
	int				zoomy;
	unsigned int	color;

	rotx = con->knobs.w_rot.posx;
	roty = con->knobs.w_rot.posy;
	posxx = con->knobs.w_posx.posx;
	posxy = con->knobs.w_posx.posy;
	posyx = con->knobs.w_posy.posx;
	posyy = con->knobs.w_posy.posy;
	zoomx = con->knobs.zoom.posx;
	zoomy = con->knobs.zoom.posy;
	j = -1;
	while(++j < con->k_height)
	{
		i = -1;
		while (++i < con->k_width)
		{
			color = pixel_color_get3(i, j, con->r);
			if (color != 0xFF202020)
				my_pixel_put(rotx + i, roty + j, &r->img, color);

			color = pixel_color_get3(i, j, con->g);
			if (color != 0xFF202020)
				my_pixel_put(posxx + i, posxy + j, &r->img, color);

			color = pixel_color_get3(i, j, con->b);
			if (color != 0xFF202020)
				my_pixel_put(posyx + i, posyy + j, &r->img, color);

			color = pixel_color_get3(i, j, con->s);
			if (color != 0xFF202020)
				my_pixel_put(zoomx + i, zoomy + j, &r->img, color);
	
		}
	}
}


void	set_w_nav(t_game *r)
{
	t_control		control;
	int				i;
	int				j;
	int				k;

	control = *r->con;
	j = -1;
	k = 0;
	while(++j < control.nav_h)
	{
		i = -1;
		while (++i < control.nav_w)
			my_pixel_put(i, j, &r->img, pixel_color_get3(i, k, control.w_nav));
		k++;
	}
	// set the dials knobs
	set_nav_w_knob_pos(r, r->con);
	set_nav_w_knobs(r, r->con);
	mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);
	set_nav_vals(r->mlx_connect, r->mlx_win, r, 1);
}

void	open_panel(t_game *r, bool *open_panel, void (*set_panel)(t_game *r))
{
	*open_panel = true;
	set_panel(r);
}


int	check_top_buttons(t_game *r, int x, int y, t_control *con)
{
	if ((y > 41 && y <= 64) && (x >= 52 && x <= 140))//prev panel, col nav
	{
		open_panel(r, &con->nav_w_open, set_w_nav);
		con->nav_open = false;
		return (1);
	}
	else if ((y > 41 && y <= 64) && (x >= 153 && x <= 247))//to colorcon
	{
		con->nav_w_open = false;
		con->nav_open = false;
		open_panel(r, &r->con_open, set_controls);
		return (1);
	}
	else if ((y > 41 && y <= 64) && (x >= 257 && x <= 355))//next panel, nav
	{
		open_panel(r, &con->nav_open, set_nav);
		con->nav_w_open = false;
		return (1);
	}
	return (0);
}

int con_press(int x, int y, t_game *game, t_control *con)
{
	/* void			*connect = game->mlx_connect;
	void			*win = game->mlx_win; */
	int				num;

	num = con->col_num;
	if (check_top_buttons(game, x, y, con))
		return (0);
	else if ((x >= 7 && x <= 62) && (y > 70 && y <= 92)) // startx: 6, starty: 70 ; tab size: 70x23
		select_tab(num, game, 1, 6);
	else if ((x >= 63 && x <= 118) && (y > 70 && y <= 92))
		select_tab(num, game, 2, 61);
	else if ((x >= 119 && x <= 174) && (y > 70 && y <= 92))
		select_tab(num, game, 3, 116);
	else if ((x >= 175 && x <= 230) && (y > 70 && y <= 92))
		select_tab(num, game, 4, 171);
	else if ((x >= 231 && x <= 286) && (y > 70 && y <= 92))
		select_tab(num, game, 5, 226);
	else if ((x >= 127 && x <= 196) && (y > 211 && y <= 234))
	{
		game->color_depth++;
		if (game->color_depth > 4)
			game->color_depth = 0;
		render(game);
		//return (0);
		//make better button and add press

	}
	else if ((x >= 203 && x <= 272) && (y > 211 && y <= 234))
	{
		game->color_depth--;
		if (game->color_depth < 0)
			game->color_depth = 4;
		render(game);
		//return (0);
	}
	else
		check_knobs(x, y, game);
	set_controls(game);
	
	return (0);
}
