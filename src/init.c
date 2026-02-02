/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:58:47 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 17:47:33 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"


	/*-------RULES---------*/

	/* MOD => MODIFIES OTHER RULES OR RULE SET */
	
	/*---1 ===== NO DOUBLES(SAME AS PREV)
	-----2 ===== NO LEFT NEIGHBOR OF PREVIOUS
	-----3 ===== NO RIGHT NEIGHBOR OF PREVIOUS
    -----4 ===== IF DOUBLE, NO NEIGHBOR 1 AWAY
    -----5 ===== NOT SAME AS 1 BEFORE PREVIOUS
    -----6 ===== NO 2 AWAY LEFT
    -----7 ===== NO 2 AWAY RIGHT
    -----8 ===== NO TRIPLES
    -----9 ===== NO 3 AWAY LEFT
    -----10 ==== NO 3 AWAY RIGHT
    -----11 ==== NO 4 AWAY LEFT
    -----12 ==== NO 4 AWAY RIGHT
    -----13 ==== IF DOUBLE NO 2 AWAY
    -----14 ==== IF DOUBLE NO 3 AWAY
    -----15 ==== NO LEFT NEIGHBOR OF ONE BEFORE PREV
    -----16 ==== NO RIGHT NEIGHBOR OF ONE BEFORE PREV
    -----17 ==== IF DOUBLE NO 4 AWAY
    -----18 ==== NO 2 AWAY OF 1 BEFORE PREV, LEFT
    -----19 ==== NO 2 AWAY OF 1 BEFORE PREV, RIGHT
    -----20 ==== NO 3 AWAY OF 1 BEFORE PREV, LEFT
    -----21 ==== NO 3 AWAY OF 1 BEFORE PREV, RIGHT
    -----22 ==== NO 4 AWAY OF 1 BEFORE PREV, LEFT
    -----23 ==== NO 4 AWAY OF 1 BEFORE PREV, RIGHT
    -----24 ==== IF PREV IS NEIGH OF 1 BEFORE PREV, NO NEIGH OF 1 BEFORE PREV
    -----25 ==== IF PREV IS NEIGH OF 1 BERORE PREV, NO 2 AWAY OF 1 BEFORE PREV
    
	***26 MODIFIES 24, 25, 27, 28, AND 29 WHENEVER ANY COMBO OF THESE IS ACTIVE
	
	-MOD-26 ==== NO SAME AS 1 BEFORE PREV WHEN FOLLOWING ARE ON: 24, 25, 27, 28, 29

	-----27 ==== IF PREV IS A NEIGH OF 1 BEFORE PREV, NO NEIGH OF 2 BEFORE PREV
	-----28 ==== IF PREV IS A NEIGH OF 1 BEFORE PREV, NO 2 AWAY FROM PREV
	-----29 ==== IF PREV IS A NEIGH OF 1 BEFORE PREV, NO 3 AWAY FROM ONE BEFORE PREV  
	-----30 ==== NOT SET

	***THESE TWO MODIFY CURRENT ACTIVE RULE SET***
	
	-MOD-DISINFO_1 ==== DISINFORM RECENT VERTS 1 BACK
    -MOD-DISINFO_2 ==== DISINFORM RECENT VERTS 2 BACK
	*/

void	clear_all(t_game *r)
{
	if (r->texture)
		free(r->texture);
	if (r->con)
		destroy_controls(r->mlx_connect, r->con);
	if (r->w_colors)
		free(r->w_colors);
	if (r->pixels_xl)
		free_ui_matrix(r->pixels_xl, r->height_orig * r->s_kernel);
	if (r->img.img_ptr)
		mlx_destroy_image(r->mlx_connect, r->img.img_ptr);
	if (r->cmyk.img_ptr)
		mlx_destroy_image(r->mlx_connect, r->cmyk.img_ptr);
	if (r->mlx_win)
		mlx_destroy_window(r->mlx_connect, r->mlx_win);
	mlx_destroy_display(r->mlx_connect);
	free(r->mlx_connect);
	exit(EXIT_FAILURE);
}

void	events_init(t_game *r) //for things not to reset upon strg press!
{
	mlx_hook(r->mlx_win, KeyPress, KeyPressMask, key_handler, r);
	mlx_hook(r->mlx_win, DestroyNotify, StructureNotifyMask, close_screen, r);
	mlx_hook(r->mlx_win, ButtonPress, ButtonPressMask, mouse_handler, r);
	mlx_hook(r->mlx_win, MotionNotify, PointerMotionMask, mouse_move, r);
	mlx_hook(r->mlx_win, ButtonRelease, ButtonReleaseMask, mouse_release, r);
	

	//this sets off transpar on old images, maybe fix in the importer when detecting Rgb type
	if (r->god)
	{
		r->colors.background |= 0xFF000000;
		r->colors.color_1 |= 0xFF000000;
		r->colors.color_2 |= 0xFF000000;
		r->colors.color_3 |= 0xFF000000;
		r->colors.color_4 |= 0xFF000000;
	}

	if (!r->god)//must place these outside of this for backward compat.
	{
		r->colors = ORIGINAL;
		r->color_depth = 0;
		r->color_op = 0; 
		r->col_shift_x = 0;
		r->col_shift_y = 0;
		r->color_rot = 0;
	}
	if (!r->god)
	{
		r->mouse_zoom = 0;
		r->layer = 0;
		r->curved = 0;
		r->jump_to_sides = 0;
		r->jump_to_center = 0;
		r->mode = 1;
		r->function_id = 0;
		r->colors.background = 0xFF000000;
	}
	r->resize = false;
}

void	init_rv(t_game *r)
{
	int i;

	i = -1;
	while (++i < r->rv_len)
		r->rv[i] = i;
	
	//can try a second rv here that updated after and uses a fixed slot system instead of rolling
	r->prev = 1;
	r->obp = 0;
	r->tbp = 2;
	r->thbp = 1;
}

void	info_init(t_game *r)
{
	r->con_open = false;
	r->on_con = false;
	r->con->knob = 0;
	r->con->base_d = sqrt((double)(SQ(r->width) + SQ(r->height)) / 4);//dist to corner of screen
	r->win_change_x = 1;
	r->win_change_y = 1;

	if (!r->god)
	{
		r->iters_change = 0;//num times iters key change +- ,1, 2 etc, used for window resize, iters set
		r->rotate = 0;
		r->ratio_start = 0.5;
		r->move_x = 0.0;
		r->move_y = 0.0;
		r->zoom = 1.0;
		r->dist_ratio = .5;
		r->r = r->height * .4;
		//printf("R: %lu\n", r->r);
		r->sides = 6;
		r->area_factor = .25 * (double)r->sides * sin(2 * M_PI / (double)r->sides);//Area * (1 / 2), Area: Area(sides) = (sides / 2) * sin(2 * M_PI / sides) * r * r 

		r->iters = (long)(SQ(r->r * r->zoom) * r->area_factor);
		r->vert_dist = 1;
		r->max_distance = r->con->base_d;
	}
	
	//stores starting maxd as base and maxd change
	if (r->god && r->supersample)
		r->start_maxd = r->con->base_d * r->s_kernel;
	else
		r->start_maxd = r->con->base_d;

	//these are correct
	r->con->base_d *= r->vert_dist;
	r->con->max_d = r->con->base_d;

	r->rv_len = 3;//change here and in header struct
	init_rv(r);
	r->vertices2 = NULL;
	r->ratio_change = r->dist_ratio / (2 - r->dist_ratio);
	r->jump_to_center_col = 0;
	r->jump_to_sides_col = false;
	r->ghost2 = 0;
	r->ghost = false;

}

void	r_init(t_game *r)
{
	int	i;

	i = -1;
	r->num_rules = 30;
	while(++i < r->num_rules)
		r->rules[i] = 0;
	r->disinfo_1 = false;
	r->disinfo_2 = false;
}

static inline void init_textures(t_game *r)
{
	r->texture = import_png(r->mlx_connect, "textures/copper1.png", &r->width_tex, &r->height_tex);
	if (!r->texture)
		clear_all(r);
}

void	game_init(t_game *r)
{
	r->con = NULL;
	r->texture = NULL;
	r->w_colors = NULL;
	r->pixels_xl = NULL;

	r->width_tex = 0;
	r->height_tex = 0;

	r->mlx_connect = mlx_init();
	if (r->mlx_connect == NULL)
		exit(EXIT_FAILURE);
	r->mlx_win = mlx_new_window(r->mlx_connect, r->width, \
		r->height, "******* THE  CHAOS  GAME *******");	
	if (r->mlx_win == NULL)
	{
		mlx_destroy_display(r->mlx_connect);
		free(r->mlx_connect);
		exit(EXIT_FAILURE);
	}
	if (new_img_init(r->mlx_connect, &r->img, r->width, r->height) == -1)
		clear_all(r);
	//init cmyk image
	if (new_img_init(r->mlx_connect, &r->cmyk, r->width, r->height) == -1)
		clear_all(r);
	events_init(r);
	r->con = make_controls(r->mlx_connect);
	if (!r->con)
		clear_all(r);
	init_textures(r);
	info_init(r);
}
