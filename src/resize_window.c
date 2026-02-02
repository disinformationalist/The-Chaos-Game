#include "chaos.h"

// FT MODES NEED MORE ITERS.

static void	pre_adjust_params(t_game *r, int new_width, int new_height)
{	
	r->move_x *= (r->zoom * 15) / (double)r->width;
	r->move_y *= (r->zoom * 15) / (double)r->height;
	r->col_shift_x = ft_round((double)r->col_shift_x / ((double)r->height / 8));//needs super fix?
	r->col_shift_y = ft_round((double)r->col_shift_y / ((double)r->height / 8));

	double old_hyp = sqrt((SQ(r->width) + SQ(r->height)) / 4);
	r->iters = round(4 * ((double)r->iters / (SQ((double)r->r * r->zoom) * r->area_factor) - 1));
	r->width = new_width;
	r->height = new_height;
	r->width_orig = new_width;
	r->height_orig = new_height;
	double new_hyp = sqrt((SQ(r->width) + SQ(r->height)) / 4);
	if (!r->supersample)
	{
		r->max_distance *= (new_hyp / old_hyp);
		r->start_maxd *= (new_hyp / old_hyp);
	}
	else
	{
		r->max_distance *= (new_hyp / old_hyp) * r->s_kernel;
		r->start_maxd *= (new_hyp / old_hyp)  * r->s_kernel;
	}
}

static void post_adjust_params(t_game *r)
{
	//ADJUST ALL H AND W PARAMS
	r->r = r->height *.4;
	r->iters = round(SQ((double)r->r * r->zoom) * r->area_factor * (1 + (double)r->iters / 4));
	r->move_x *= (double)(r->width) / (15 * r->zoom);
	r->move_y *= (double)(r->height) / (15 * r->zoom);
	r->col_shift_x = ft_round((double)r->col_shift_x * ((double)r->height / 8));//needs super fix?
	r->col_shift_y = ft_round((double)r->col_shift_y * ((double)r->height / 8));
	printf("col s x: %d\n", r->col_shift_x);
	r->resize = false;
	r->win_change_x = 1;
	r->win_change_y = 1;
}

void	resize_window(t_game *r, int new_width, int new_height)//knobs not working right on resize
{
	mlx_destroy_image(r->mlx_connect, r->img.img_ptr);
	mlx_destroy_image(r->mlx_connect, r->cmyk.img_ptr);
	mlx_destroy_window(r->mlx_connect, r->mlx_win);

/* 	mlx_destroy_display(r->mlx_connect);
	free(r->mlx_connect);
	r->mlx_connect = mlx_init();
	if (r->mlx_connect == NULL)
		exit(EXIT_FAILURE);//pro */

	pre_adjust_params(r, new_width, new_height);	
	r->mlx_win = mlx_new_window(r->mlx_connect, r->width, \
		r->height, "******* THE  CHAOS  GAME *******");
	if (r->mlx_win == NULL)
		clear_all(r);
	if (new_img_init(r->mlx_connect, &r->img, r->width, r->height) == -1)
		clear_all(r);
	if (new_img_init(r->mlx_connect, &r->cmyk, r->width, r->height) == -1)
		clear_all(r);
	mlx_hook(r->mlx_win, KeyPress, KeyPressMask, key_handler, r);
	mlx_hook(r->mlx_win, DestroyNotify, StructureNotifyMask, close_screen, r);
	mlx_hook(r->mlx_win, ButtonPress, ButtonPressMask, mouse_handler, r);
	mlx_hook(r->mlx_win, MotionNotify, PointerMotionMask, mouse_move, r);
	mlx_hook(r->mlx_win, ButtonRelease, ButtonReleaseMask, mouse_release, r);
	if (r->supersample)
	{
		r->width *= r->s_kernel;
		r->height *= r->s_kernel;
	}
	post_adjust_params(r);
}
