#include "chaos.h"

// FT MODES NEED MORE ITERS.

static void	pre_adjust_params(t_game *r, int new_width, int new_height)
{	
	r->move_x *= (r->zoom * 15) / r->width;
	r->move_y *= (r->zoom * 15) / r->height;
	//r->max_distance = r->max_distance - sqrt(((r->width * r->width) + (r->height * r->height)) / 4);
	
	double old_hyp = sqrt(((r->width * r->width) + (r->height * r->height)) / 4);
	double max_change = r->max_distance - old_hyp;
	
	
	r->iters = round(4 * ((double)r->iters / (((double)r->r * r->zoom) * ((double)r->r * r->zoom) * sqrt(3)) - 1));
	r->width = new_width;
	r->height = new_height;
	r->width_orig = new_width;
	r->height_orig = new_height;
	
	double new_hyp = sqrt(((r->width * r->width) + (r->height * r->height)) / 4);
	r->max_distance =  (new_hyp / old_hyp) * (r->max_distance + max_change);
}

static void post_adjust_params(t_game *r)
{
	//ADJUST ALL H AND W PARAMS
	r->r = (r->height / 2 - r->height / 10);
	r->iters = round(((double)r->r * r->zoom) * ((double)r->r * r->zoom) * sqrt(3) * (1 + (double)r->iters / 4));
	//r->iters = round(((double)r->r * r->zoom) * ((double)r->r * r->zoom) * sqrt(3) * (1 + (double)r->iters_change / 4));

	r->move_x *= (double)(r->width) / (15 * r->zoom);
	r->move_y *= (double)(r->height) / (15 * r->zoom);
	//r->max_distance += sqrt(((r->width * r->width) + (r->height * r->height)) / 4);//changed from width/2 to dist to corner.
	r->resize = false;
	r->win_change_x = 0;
	r->win_change_y = 0;
}

void	resize_window(t_game *r, int new_width, int new_height)
{
	mlx_destroy_image(r->mlx_connect, r->img.img_ptr);
	mlx_destroy_window(r->mlx_connect, r->mlx_win);
	pre_adjust_params(r, new_width, new_height);	
	r->mlx_win = mlx_new_window(r->mlx_connect, r->width, \
		r->height, "******* THE  CHAOS  GAME *******");
	if (r->mlx_win == NULL)
		clear_all(r);
	if (new_img_init(r->mlx_connect, &r->img, r->width, r->height) == -1)
		clear_all(r);
	mlx_hook(r->mlx_win, KeyPress, KeyPressMask, key_handler, r);
	mlx_hook(r->mlx_win, DestroyNotify, StructureNotifyMask, close_screen, r);
	mlx_hook(r->mlx_win, ButtonPress, ButtonPressMask, mouse_handler, r);
	if (r->supersample)
	{
		r->width = r->width * r->s_kernel;
		r->height = r->height * r->s_kernel;
	}
	post_adjust_params(r);
}
