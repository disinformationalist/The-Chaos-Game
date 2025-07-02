#include "chaos.h"


t_3color	set_color(t_colors colors, int col_num)
{
	int			num_color;
	t_3color	color;

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

	char			max[20];
	char			iters[20];
	char 			depth[5];
	char			hex[10];
	int				y_s;
	unsigned int	val_col;
	t_3color		color;

	color = game->curr_col;
	val_col = 0x90C4FF;
	hex_col = (unsigned int)((color.r << 16) | (color.g << 8) | (color.b));
	sprintf(hex, "0x%x", hex_col);
	sprintf(r, "%d", color.r);
	sprintf(g, "%d", color.g);
	sprintf(b, "%d", color.b);
	mlx_string_put(mlx_con, mlx_win, 315, 87, val_col, hex);
	y_s = 117;
	mlx_string_put(mlx_con, mlx_win, 327, y_s, val_col, r);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, val_col, g);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, val_col, b);

	//depth setting
	sprintf(depth, "%d", game->color_depth);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 36, val_col, depth);

	
	//iters //max dist
	double dist = game->con->max_d;
	long it = game->con->iters;
	
	sprintf(max, "%.3f", dist);
	mlx_string_put(mlx_con, mlx_win, 315, y_s += 30, val_col, max);
	sprintf(iters, "%ld", it);
	mlx_string_put(mlx_con, mlx_win, 311, y_s += 60, val_col, iters);
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

t_3color	set_color_knobs(t_img *img, t_control *cont, int k_width, t_colors colors)
{
	int				i;
	int				j;
	unsigned int	color;
	int				x_startr;
	int				x_startg;
	int				x_startb;
	int				y_startr;
	int				y_startg;
	int				y_startb;

	int				y_starts;
	int				x_starts;

	int				y_startdist;
	int				x_startdist;
	int				start;
	t_3color		num_color;
	t_control		control = *cont;

	num_color = set_color(colors, control.col_num);
	start = 22 - k_width / 2;

	x_startr = start + num_color.r;
	x_startg = start + num_color.g;
	x_startb = start + num_color.b;
	y_startr = start + 89;
	y_startg = y_startr + 25;
	y_startb = y_startg + 25;

	//iters
	double iters = (double)(control.iters % 336708) * (M_PI / (double)168354);// * PI_FIFTHS;setting like pos on rt 

	x_starts = (int)round(120 * cos(iters)) + 192;
	y_starts = -(int)round(120 * sin(iters)) + 424;
	cont->iters_i = x_starts;
	cont->iters_j = y_starts;
	cont->start_angle = iters;

	//dist, len 144 in center of slider
	x_startdist = 119 + (cont->max_d - cont->base_d) / (6 * cont->base_d) * 144;
	y_startdist = 218;

/* 	j = -1;
	while(++j < 570)//centerline vert check
	{
		my_pixel_put(200, j, img, 0xFFFFFF);
	} */

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
				my_pixel_put(x_starts + i, y_starts + j, img, color);//iter
				my_pixel_put(x_startdist + i, y_startdist + j, img, color);//dist
			}
		}
	}
	return (num_color);
}

void	set_controls(t_game *r)
{
	t_control	control;
	int			i;
	int			j;
	t_img		*img;
	t_img		*con;

	control = *r->con;
	con = control.color_con;
	img = &r->img;
	if (r->supersample)
	{
		r->con->max_d = r->max_distance / r->s_kernel;
		r->con->iters = r->iters / (r->s_kernel * r->s_kernel);
	}
	else
	{
		r->con->max_d = r->max_distance;
		r->con->iters = r->iters;
	}
	j = -1;
	while(++j < control.m_height)
	{
		i = -1;
		while (++i < control.m_width)
			my_pixel_put(i, j, img, pixel_color_get3(i, j, con));
	}
	r->curr_col = set_color_knobs(img, r->con, control.k_width, r->colors);
}

//if open putstrs after put to win
//menu 400 by 570

void	controls(t_game *r)
{
	if (!r->con_open)
		set_controls(r);
	r->con_open = !r->con_open;
	if (!r->con_open)
	{
		r->on_con = false;
		r->con->knob = -1;
		intermed(r);
	}
	else
	{
		mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);
		set_color_vals(r->mlx_connect, r->mlx_win, r);
	}
}