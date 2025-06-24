/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:07:25 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 15:09:25 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

//2 back color sometimes more clear looking on details function mode maybe other



static inline void	intermed_init(t_game *r, int ***vertices, double *x, double *y)//CONSIDER MOVIN POLYGON HERE FOR MULIT THREAD...
{	
	if (r->resize)//maybe change to cycle through desired presets.
		resize_window(r, r->width_orig + r->win_change_x, r->height_orig + r->win_change_y);
//------------------	works..
	if (r->supersample)
	{
		r->pixels_xl = malloc_ui_matrix(r->width_orig * r->s_kernel, r->height_orig * r->s_kernel);
		if (!r->pixels_xl)
		{
			printf(RED"The super malloc has failed\n"RESET);
			close_screen(r);
		}
		zero_ui_matrix(r->pixels_xl, r->width, r->height);
	}
//----------------
	if (r->jump_to_sides)
		r->points = r->sides * 2;
	else
		r->points = r->sides;
	if (r->jump_to_center)
		r->points++;
	r->w_colors = set_color_wheel(360, 1.0, 0.5, 202);//second layer arrow keys move and recenter with ^pad_0
	//r->w_colors = set_color_wheel(r->points * 50, 1.0, 0.5, 0xFF00FF);

	if (!r->w_colors)
		clear_all(r);
	*x = r->width / 2 + r->move_x;
	*y = r->height / 2 - r->move_y; 
	*vertices = NULL;
}

static inline void	reset_img_memory(t_game *r)
{
	memset(r->img.pixels_ptr, 0, r->width_orig * r->height_orig * \
		(r->img.bpp / 8));
}

static inline void	run_game(t_game *r, int **vertices, double x, double y)
{
	if (r->curved)
		chaos_game_curved(r, vertices, x, y);
	else if (r->mode == 1)
		chaos_game(r, vertices, x, y);
	else if (r->mode == 2 || r->mode == 3 || r->mode == 4)
		chaos_game_apply_function(r, vertices, x,  y);
}

static inline void	end_intermed(t_game *r)
{
	if (r->supersample)
	{	
		//mean_convo_filter(r->mlx_connect, &r->img_new, r->width, r->height, 9);//seems to help. smooth here then UNsharp mask!? monte carlo denoising? non local means!
		
		//---------perfom circle reflect in xl_size before downsample to make nice-----------this works!!!!!
		/* unsigned int **new = pm_circle_matrix(r->pixels_xl, r->width, r->height, r->height / 8, r->height / 2 - r->height / 8);
		free(r->pixels_xl);
		r->pixels_xl = new; */
		//-----------------------------------------------------------------------------------
		downsample_xl(r->width, r->height, &r->img, r->pixels_xl, r->s_kernel);///
		//zero_ui_matrix(r->pixels_xl, r->width, r->height);///do i need this?
		free_ui_matrix(r->pixels_xl, r->height);
		
		
		//mean_convo_filter(r->mlx_connect, &r->img, r->width_orig, r->height_orig, 5);
		//gaussian_convo_filter(r->mlx_connect, &r->img, r->width_orig, r->height_orig, 7, 1.5);//testing.....

		ft_putstr_color("...SUPERSAMPLE COMPLETE\n", BOLD_BRIGHT_BLUE);
	}
	free(r->w_colors);
	r->w_colors = NULL;	
}

void	intermed(t_game *r)
{
	long	start;
	long	end;
	int		**vertices;
	double		x;
	double		y;


	


	uint64_t	glob_seed;

	glob_seed = 0xFABDECAF;//secret number fAb DeCaF
	sxoro128(&r->rng, glob_seed);// * 142857);

	start = get_time();
	intermed_init(r, &vertices, &x, &y);
	reset_img_memory(r);
	run_game(r, vertices, x, y);
	end_intermed(r);
	mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);
	end = get_time();
	print_time(start, end, "RENDER TIME: %f seconds\n");
}
