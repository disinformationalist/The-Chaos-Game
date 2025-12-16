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

const t_colors FIRE = {DEEP_RED, ORANGE, COL_YELLOW, INTENSE_PINK, BLACK1};
const t_colors ICE = {DEEP_PURPLE, ICY_BLUE, LIGHT_CYAN, SILVER, BLACK1};
const t_colors CANDY = {BUBBLEGUM_PINK, LEMON_YELLOW, MINT_GREEN, SKY_BLUE, BLACK1};
const t_colors SLIME = {TOXIC_GREEN, RADIOACTIVE_YELLOW, ALIEN_BLUE, SLIMY_PURPLE, BLACK1};
const t_colors UNNAMED_1 = {COL_CYAN, MALACHITE_GREEN, GOLD, SINISTER_RED, BLACK1};
const t_colors ORIGINAL = {COL_CYAN, COL_MAGENTA, COL_YELLOW, COL_BLUE, BLACK1};
const t_colors NEBULA = {INDIGO_PURPLE, DARK_ORCHID, GOLD_2, ORANGE_RED, BLACK1};
const t_colors ETERNAL_SPRING = {CHARTREUSE, HOT_PINK, DARK_TURQUOISE, GOLD_2, BLACK1};
const t_colors DUSK = {0x9253FF, 0xFFC64F, 0xE7F2FF, 0x878EFF, BLACK1};

const t_colors HIGH_SUMMER = {TOMATO_RED, GOLD_2, LIME_GREEN, STEEL_BLUE, BLACK1};
const t_colors WAXING_AUTUMN = {CHOCOLATE, ORANGE_RED, ORANGE, SADDLE_BROWN, BLACK1};
const t_colors DEEP_WINTER = {SKY_BLUE, STEEL_BLUE, COL_WHITE, SLATE_GRAY, BLACK1};
const t_colors UNNAMED_2 = {EMERALD, COPPER, INDIGO, GUNMETAL, BLACK1};
const t_colors UNNAMED_3 = {TEAL, PLUM, AMBER, CHARCOAL, BLACK1};
const t_colors GUN = {STEEL_GRAY, SILVER, BRONZE, GUNMETAL_2, BLACK1};

static int	key_handler_other_4layer(int keysym, t_game *r)
{
	if (keysym == KEY_Y)
		r->colors = DUSK;
	else if (keysym == KEY_X)
		r->colors = HIGH_SUMMER;
	else if (keysym == KEY_C)
		r->colors = WAXING_AUTUMN;
	else if (keysym == KEY_V)
		r->colors = DEEP_WINTER;
	else if (keysym == KEY_B)//UN
		r->colors = UNNAMED_2;
	else if (keysym == KEY_N)//UN
		r->colors = UNNAMED_3;
	else if (keysym == KEY_M)
		r->colors = GUN;
	else
		supersample_handler(keysym, r);
	return (0);
}

static int	key_handler_other_4(int keysym, t_game *r)
{
	if (keysym == KEY_Y)
		r->colors = FIRE;
	else if (keysym == KEY_X)
		r->colors = ICE;
	else if (keysym == KEY_C)
		r->colors = CANDY;
	else if (keysym == KEY_V)
		r->colors = SLIME;
	else if (keysym == KEY_B)//UNNAMED
		r->colors = UNNAMED_1;
	else if (keysym == KEY_N)
		r->colors = ORIGINAL;
	else if (keysym == KEY_M)
		r->colors = NEBULA;
	else
		supersample_handler(keysym, r);
	return (0);
}

static int	key_handler_other_3(int keysym, t_game *r)
{
	if (keysym == COMMA)
			r->max_distance += r->height / 10;
	else if (keysym == PERIOD)
			r->max_distance -= r->height / 10;
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
			r->max_distance += r->height / 10;
	else if (keysym == PERIOD)
			r->max_distance -= r->height / 10;
	else if (keysym == PAD_3)
		r->dist_ratio = 2;
	else
		key_handler_other_4layer(keysym, r);
	return (0);
}

static int	key_handler_other_2(int keysym, t_game *r)
{
	if (keysym == XK_F5)
		r->sides = 5;
	else if (keysym == XK_F6)
		r->sides = 6;
	else if (keysym == XK_F7)
		r->sides = 7;
	else if (keysym == XK_F12)
		r->sides = 12;
	else if (keysym == XK_Control_L)//lft strg here...
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
	if (keysym == XK_F4)
		r->sides = 4;
	else if (keysym == XK_F5)
		r->sides = 5;
	else if (keysym == XK_F6)
		r->sides = 6;
	else if (keysym == XK_F7)
		r->sides = 7;
	else if (keysym == XK_F12)
		r->sides = 12;
	else if (keysym == XK_Control_L)
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
	else if (keysym == XK_Control_R)
		r->color_rot += 7;
	else
		key_handler_other_2layer(keysym, r);
	return (0);
}