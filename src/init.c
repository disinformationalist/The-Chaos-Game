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
	/*---1 ===== NO DOUBLES
	-----2 ===== NO LEFT NEIGHBOR OF PREVIOUS
	-----3 ===== NO RIGHT NEIGHBOR OF PREVIOUS
    -----4 ===== IF SAME TWICE(DOUBLE), NO NEIGHBOR 1 AWAY
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
    -----26 ==== TURNS ON NO SAME AS 1 BEFORE PREV FOR 24 AND 25, 27, 28
	-----27 ==== IF PREV IS A NEIGH OF 1 BEFORE PREV, NO NEIGH OF 2 BEFORE PREV
	-----28 ==== IF PREV IS A NEIGH OF 1 BEFORE PREV, NO 2 AWAY FROM PREV
	-----29
    -----DISINFO_1 ==== DISINFORM RECENT VERTS 1 BACK
    -----DISINFO_2 ==== DISINFORM RECENT VERTS 2 BACK*/
void	clear_all(t_game *r)
{
	if (r->w_colors)
		free(r->w_colors);
	if (r->pixels_xl)
		free_ui_matrix(r->pixels_xl, r->height_orig * r->s_kernel);
	if (r->img.img_ptr)
		mlx_destroy_image(r->mlx_connect, r->img.img_ptr);
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
	
	//back compat
	//r->colors = ORIGINAL;
/* 	r->color_depth = 0;
	r->color_op = 0; 
	r->col_shift_x = 0;
	r->col_shift_y = 0;
	r->color_rot = 0; */
	
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
	}
	r->resize = false;
}

void	init_rv(t_game *r)
{
	int i;

	i = -1;
	while (++i < r->rv_len)
		r->rv[i] = i;
}

void	info_init(t_game *r)
{
	
		//r->iters_change = 0;//for back compat
	if (!r->god)
	{
		r->iters_change = 0;//just added can use in adjust window to simplify current... is num times iters key change ,1, 2 etc
		
		r->rotate = 0;
		r->ratio_start = 0.5;
		r->move_x = 0.0;
		r->move_y = 0.0;
		r->zoom = 1.0;
		r->sides = 3;///changed to 3 for now
		r->dist_ratio = .5;
		//height and width based params
		r->r = r->height / 2 - r->height / 10;
		//if (!r->supersample)
		r->iters = (r->r * r->zoom) * (r->r * r->zoom) * sqrt(3);//may adjust but seems good for now..iters = area of hexagon.
		r->max_distance = sqrt(((r->width * r->width) + (r->height * r->height)) / 4);//changed from width/2 to dist to corner.
	}
	r->rv_len = 3;//change here and in header struct
	
	init_rv(r);
}

void	r_init(t_game *r)
{
	int	i;

	i = -1;
	r->num_rules = 30;
	while(++i < r->num_rules)
		r->rules[i] = 0;
	r->disinfo_1 = 0;
	r->disinfo_2 = 0;
	r->win_change_x = 0;
	r->win_change_y = 0;
}

void	game_init(t_game *r)
{
	r->mlx_connect = mlx_init();
	if (r->mlx_connect == NULL)
		exit(EXIT_FAILURE);
	r->mlx_win = mlx_new_window(r->mlx_connect, r->width, \
		r->height, "******* THE  CHAOS  GAME *******");
	//ft_putstr_color("----------NOT YET....\n", BLUE);
	
	if (r->mlx_win == NULL)
	{
		mlx_destroy_display(r->mlx_connect);
		free(r->mlx_connect);
		exit(EXIT_FAILURE);
	}
	if (new_img_init(r->mlx_connect, &r->img, r->width, r->height) == -1)
		clear_all(r);
	events_init(r);
	info_init(r);
}