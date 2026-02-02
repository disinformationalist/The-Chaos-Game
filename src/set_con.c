#include "chaos.h"


t_4color	set_color(t_colors colors, int col_num)
{
	int			num_color;
	t_4color	color;

	if (col_num == 1)
		num_color = colors.color_1;
	else if (col_num == 2)
		num_color = colors.color_2;
	else if (col_num == 3)
		num_color = colors.color_3;
	else if (col_num == 4)
		num_color = colors.color_4;
	else //(col_num == 5)
		num_color = colors.background;
	//unpack
	color.t = (num_color >> 24);
	color.r = (num_color >> 16) & 0xFF;
	color.g = (num_color >> 8) & 0xFF;
	color.b = num_color & 0xFF;
	return (color);
}

void	set_color_vals(void *mlx_con, void *mlx_win, t_game *game)
{
	unsigned int	hex_col;
	char 			r[10];
	char 			g[10];
	char 			b[10];
	char 			t[10];

	char			max[20];
	char			iters[20];
	char 			depth[5];
	char			hex[13];
	int				y_s;
	unsigned int	val_col;
	t_4color		color;

	color = game->curr_col;
	val_col = 0x90C4FF;
	hex_col = (unsigned int)((color.t << 24) | (color.r << 16) | (color.g << 8) | (color.b));
	sprintf(hex, "0x%x", hex_col);
	sprintf(r, "%d", color.r);
	sprintf(g, "%d", color.g);
	sprintf(b, "%d", color.b);
	sprintf(t, "%d", color.t);

	mlx_string_put(mlx_con, mlx_win, 315, 87, val_col, hex);
	y_s = 117;
	mlx_string_put(mlx_con, mlx_win, 327, y_s, val_col, r);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, val_col, g);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, val_col, b);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, val_col, t);

	//depth setting
	sprintf(depth, "%d", game->color_depth);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 36, val_col, depth);

	//iters //max dist

	double dist = game->con->max_d;
	double vert = game->con->vert_d;
	long it = game->con->iters;
	
	sprintf(max, "%.3f", dist);
	mlx_string_put(mlx_con, mlx_win, 315, y_s += 30, val_col, max);
	sprintf(max, "%.2f", vert);
	mlx_string_put(mlx_con, mlx_win, 315, y_s += 30, val_col, max);
	sprintf(iters, "%ld", it);
	mlx_string_put(mlx_con, mlx_win, 311, y_s += 30, val_col, iters);
	return ;
}

//put nav panel values to the screen, xpos, ypos, rotation in xy, 1d zoom

void	set_nav_vals(void *mlx_con, void *mlx_win, t_game *game, int nav)
{
	char			zoom[20];
	char			movex[20];
	char			movey[20];
	double			movx, movy, rotv;
	char			rot[20];
	int				ys, xs;
	unsigned int	val_col;

	val_col = 0x90C4FF;
	
	xs = 110;//X
	ys = 203;
		
	if (nav == 0)
		movx = game->move_x;
	else
		movx = game->col_shift_x;
	if (game->supersample)
	{
		movx /= game->s_kernel;
		movx = ft_round(movx);
	}
	
	sprintf(movex, "%.3f", movx);
	mlx_string_put(mlx_con, mlx_win, xs, ys, val_col, movex);
	
	xs = 250;//Y
	ys = 203;
	if (nav == 0)
		movy = game->move_y;
	else
		movy = game->col_shift_y;
	if (game->supersample)
	{
		movy /= game->s_kernel;
		movy = ft_round(movy);
	}
	sprintf(movey, "%.3f", movy);
	mlx_string_put(mlx_con, mlx_win, xs, ys, val_col, movey);

	xs = 180;//ROT
	ys = 324;
	if (nav == 0)
		rotv = game->rotate * (RAD_TO_DEG);
	else
		rotv = game->color_rot;
	rotv = fmod(rotv, 360);
	sprintf(rot, "%.3f", rotv);
	mlx_string_put(mlx_con, mlx_win, xs, ys, val_col, rot);
	
	xs = 180;//ZOOM
	ys = 455;
	
	sprintf(zoom, "%.6f", game->zoom);
	mlx_string_put(mlx_con, mlx_win, xs, ys, val_col, zoom);

	return ;
}

unsigned int pixel_color_get3(int x, int y, t_img *img)
{
	unsigned int	pixel_color;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixel_color = *(unsigned int *)(img->pixels_ptr + offset);
	return (pixel_color);
}

t_4color	set_color_knobs(t_img *img, t_control *cont, int k_width, t_colors colors)
{
	int				i;
	int				j;
	unsigned int	color;
	int				x_startr;
	int				x_startg;
	int				x_startb;
	int				x_startt;

	int				y_startr;
	int				y_startg;
	int				y_startb;
	int				y_startt;

	int				y_starts;
	int				x_starts;

	int				y_startdist;
	int				x_startdist;

	int				x_startv;
	int				y_startv;

	int				start;
	t_4color		num_color;
	t_control		control = *cont;

	num_color = set_color(colors, control.col_num);
	start = 22 - k_width / 2;

	x_startr = start + num_color.r;
	x_startg = start + num_color.g;
	x_startb = start + num_color.b;
	x_startt = start + num_color.t;

	y_startr = 103;
	y_startg = y_startr + 25;
	y_startb = y_startg + 25;
	y_startt = y_startb + 25;


	//iters to angle
	double iters = (double)(cont->iters % 336708) * ITER_FACTOR;//ITER_fact = PI / 168354.0
	
	x_starts = ft_round(120 * cos(iters)) + cont->knobs.iter.cx;
	y_starts = cont->knobs.iter.cy - ft_round(120 * sin(iters));
	cont->knobs.iter.angle = iters;
	cont->knobs.iter.posx = x_starts;
	cont->knobs.iter.posy = y_starts;

	//dist, len 144 in center of slider
	x_startdist = 119 + ft_round((cont->max_d - cont->base_d) / (6 * cont->base_d) * 144);
	y_startdist = 243;
	x_startv = 141 + ft_round(cont->vert_d * 100);
	y_startv = 273;
	j = -1;
	while (++j < k_width)
	{
		i = -1;
		while (++i < k_width)
		{
			color = pixel_color_get3(i, j , control.r);
			if (color != 0xFF202020)
				my_pixel_put(x_startr + i, y_startr + j, img, color);
			color = pixel_color_get3(i, j , control.g);
			if (color != 0xFF202020)
				my_pixel_put(x_startg + i, y_startg + j, img, color);
			color = pixel_color_get3(i, j , control.b);
			if (color != 0xFF202020)
				my_pixel_put(x_startb + i, y_startb + j, img, color);
			color = pixel_color_get3(i, j , control.s);
			if (color != 0xFF202020)
			{
				my_pixel_put(x_startt + i, y_startt + j, img, color);//color transparency

				my_pixel_put(x_starts + i, y_starts + j, img, color);//set iter knob
				my_pixel_put(x_startdist + i, y_startdist + j, img, color);//set dist knob
				my_pixel_put(x_startv + i, y_startv + j, img, color);//set vert_d knob
			}
		}
	}
	return (num_color);
}

void	set_color_con(t_game *r, t_control con)
{
	int i, j;
	t_img *img = &r->img;
	
	j = -1;
	while(++j < con.m_height)
	{
		i = -1;
		while (++i < con.m_width)
			my_pixel_put(i, j, img, pixel_color_get3(i, j, con.color_con));
	}
}

void	set_controls(t_game *r)
{
	t_control	con;

	con = *r->con;
	set_color_con(r, con);
	if (r->supersample)
	{
		r->con->iters = r->iters / (r->s_kernel * r->s_kernel);	
		r->con->max_d = r->max_distance / r->s_kernel;
	}
	else
	{
		r->con->iters = r->iters;
		r->con->max_d = r->max_distance;
	}
	r->con->vert_d = r->vert_dist;
	if (!r->con->nav_open && !r->con->nav_w_open)
	{
		r->curr_col = set_color_knobs(&r->img, r->con, con.k_width, r->colors);
		mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);
		set_color_vals(r->mlx_connect, r->mlx_win, r);
	}
	else if (r->con->nav_open)
		set_nav(r);
	else
		set_w_nav(r);
}


//if open putstrs after put to win
//menu 400 by 570

void	controls(t_game *r)
{
	if (r->height_orig < 570 || r->width_orig < 400)
		return ;
	r->con_open = !r->con_open;
	if (r->con_open)
		set_controls(r);
	else //f (!r->con_open)
	{
		r->on_con = false;
		r->con->nav_open = false;
		r->con->nav_w_open = false;
		r->con->on_nav = false;
		r->con->knob = -1;
		render(r);
	}
}