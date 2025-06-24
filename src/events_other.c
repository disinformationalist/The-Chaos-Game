/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_other.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:14:30 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:14:32 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "chaos.h"

const t_colors FIRE = {DEEP_RED, ORANGE, COL_YELLOW, INTENSE_PINK};
const t_colors ICE = {DEEP_PURPLE, ICY_BLUE, LIGHT_CYAN, SILVER};
const t_colors CANDY = {BUBBLEGUM_PINK, LEMON_YELLOW, MINT_GREEN, SKY_BLUE};
const t_colors SLIME = {TOXIC_GREEN, RADIOACTIVE_YELLOW, ALIEN_BLUE, SLIMY_PURPLE};
const t_colors UNNAMED_1 = {COL_CYAN, MALACHITE_GREEN, GOLD, SINISTER_RED};
const t_colors ORIGINAL = {COL_CYAN, COL_MAGENTA, COL_YELLOW, COL_BLUE};
const t_colors NEBULA = {INDIGO_PURPLE, DARK_ORCHID, GOLD_2, ORANGE_RED};
const t_colors ETERNAL_SPRING = {CHARTREUSE, HOT_PINK, DARK_TURQUOISE, GOLD_2};
const t_colors HIGH_SUMMER = {TOMATO_RED, GOLD_2, LIME_GREEN, STEEL_BLUE};
const t_colors WAXING_AUTUMN = {CHOCOLATE, ORANGE_RED, ORANGE, SADDLE_BROWN};
const t_colors DEEP_WINTER = {SKY_BLUE, STEEL_BLUE, COL_WHITE, SLATE_GRAY};
const t_colors UNNAMED_2 = {EMERALD, COPPER, INDIGO, GUNMETAL};
const t_colors UNNAMED_3 = {TEAL, PLUM, AMBER, CHARCOAL};
const t_colors GUN = {STEEL_GRAY, SILVER, BRONZE, GUNMETAL_2};

static int	key_handler_other_4layer(int keysym, t_game *r)
{
	if (keysym == Y)
		r->colors = ETERNAL_SPRING;
	else if (keysym == X)
		r->colors = HIGH_SUMMER;
	else if (keysym == C)
		r->colors = WAXING_AUTUMN;
	else if (keysym == V)
		r->colors = DEEP_WINTER;
	else if (keysym == _B)//UN
		r->colors = UNNAMED_2;
	else if (keysym == N)//UN
		r->colors = UNNAMED_3;
	else if (keysym == M)
		r->colors = GUN;
	else
		supersample_handler(keysym, r);
	return (0);
}

static int	key_handler_other_4(int keysym, t_game *r)
{
	if (keysym == Y)
		r->colors = FIRE;
	else if (keysym == X)
		r->colors = ICE;
	else if (keysym == C)
		r->colors = CANDY;
	else if (keysym == V)
		r->colors = SLIME;
	else if (keysym == _B)//UNNAMED
		r->colors = UNNAMED_1;
	else if (keysym == N)
		r->colors = ORIGINAL;
	else if (keysym == M)
		r->colors = NEBULA;
	else
		supersample_handler(keysym, r);
	return (0);
}

static int	key_handler_other_3(int keysym, t_game *r)
{
	if (keysym == COMMA)
	{
		if (r->supersample)//make these height and width based...
			r->max_distance += 100 * r->s_kernel;
		else
			r->max_distance += 100;
	}
	else if (keysym == PERIOD)
	{
		if (r->supersample)
			r->max_distance -= 100 * r->s_kernel;
		else
			r->max_distance -= 100;
	}
	else if (keysym == PAD_3)//need to make special handling for monster
	{
		r->dist_ratio = 2;
		r->zoom /= 2187;//729;
		r->iters = 1000000;
		//adjust_ratio(r, 2);
	}
	else
		key_handler_other_4(keysym, r);
	return (0);
}

static int	key_handler_other_3layer(int keysym, t_game *r)//avail 2nd layer
{
	if (keysym == COMMA)
	{
		if (r->supersample)//make these height and width based...
			r->max_distance += 100 * r->s_kernel;
		else
			r->max_distance += 100;
	}
	else if (keysym == PERIOD)
	{
		if (r->supersample)
			r->max_distance -= 100 * r->s_kernel;
		else
			r->max_distance -= 100;
	}
	else if (keysym == PAD_3)
		r->dist_ratio = 2;
	else
		key_handler_other_4layer(keysym, r);
	return (0);
}

static int	key_handler_other_2(int keysym, t_game *r)
{
	if (keysym == F4)
		r->sides = 4;
	else if (keysym == F5)
		r->sides = 5;
	else if (keysym == F6)
		r->sides = 6;
	else if (keysym == F7)
		r->sides = 7;
	else if (keysym == F12)
		r->sides = 12;
	else if (keysym == STRG)//lft strg here...
	{
		r_init(r);
		info_init(r);
	}
	else
		key_handler_other_3(keysym, r);
	return (0);
}

static int	key_handler_other_2layer(int keysym, t_game *r)//avail 2nd layer
{
	if (keysym == F4)
		r->sides = 4;
	else if (keysym == F5)
		r->sides = 5;
	else if (keysym == F6)
		r->sides = 6;
	else if (keysym == F7)
		r->sides = 7;
	else if (keysym == F12)
		r->sides = 12;
	else if (keysym == STRG)//lft strg here...
	{
		r_init(r);
		info_init(r);
	}
	else
		key_handler_other_3layer(keysym, r);
	return (0);
}

int	key_handler_other(int keysym, t_game *r)
{
	if (keysym == MULT || keysym == DIV)
		zoom_iters_key(keysym, r);
	else if (keysym == PAD_0)
	{
		r->r = r->height / 2 - r->height / 10;
		r->iters = 4 * r->dist_ratio * r->dist_ratio * r->r * r->r * sqrt(3);
		r->zoom = 1.0;
		r->iters_change = 0;
		r->move_x = 0.0;
		r->move_y = 0.0;
		r->rotate = 0;
	}
	else
		key_handler_other_2(keysym, r);
	return (0);
}

int	key_handler_otherlayer(int keysym, t_game *r)
{
	if (keysym == MULT || keysym == DIV)
		zoom_iters_key(keysym, r);
	else if (keysym == PAD_0)
	{
		r->col_shift_x = 0;
		r->col_shift_y = 0;
	}
	else if (keysym == RGHT_STRG)
		r->color_rot += 7;
	else
		key_handler_other_2layer(keysym, r);
	return (0);
}