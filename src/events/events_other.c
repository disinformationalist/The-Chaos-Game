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
const t_colors DUSK = {0xFF9253FF, 0xFFFFC64F, 0xFFE7F2FF, 0xFF878EFF, BLACK1};

const t_colors HIGH_SUMMER = {TOMATO_RED, GOLD_2, LIME_GREEN, STEEL_BLUE, BLACK1};
const t_colors WAXING_AUTUMN = {CHOCOLATE, ORANGE_RED, ORANGE, SADDLE_BROWN, BLACK1};
const t_colors DEEP_WINTER = {SKY_BLUE, STEEL_BLUE, COL_WHITE, SLATE_GRAY, BLACK1};
const t_colors PSYCHEDELIC = {COL_CYAN, 0xFF5948A6, 0xFFFFB400, 0xFFFFC800, BLACK1};
//const t_colors UNNAMED_2 = {EMERALD, COPPER, INDIGO, GUNMETAL, BLACK1};//replaced by psych
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
	else if (keysym == KEY_B)
		r->colors = PSYCHEDELIC;
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
	else if (keysym == PAD_3)
	{
		adjust_ratio(r, 2);
	}
	else
		key_handler_other_4(keysym, r);
	return (0);
}

static int	key_handler_other_3layer(int keysym, t_game *r)//avail 2nd layer
{
	if (keysym == COMMA)
		r->max_distance += r->height * .1;
	else if (keysym == PERIOD)
		r->max_distance -= r->height * .1;
	else if (keysym == PAD_3)
		r->dist_ratio = 2;
	else
		key_handler_other_4layer(keysym, r);
	return (0);
}

//change the number of sides and reset iters for new sides

static inline void change_sides(t_game *r, int sides)
{
	r->sides = sides;//may need to pass in old sides.. or old ITERS	, adjust below ft
	r->area_factor = .25 * (double)r->sides * sin(2 * M_PI / (double)r->sides);
	//r->area_factor = ((double)r->sides / 4.0) * sin(2 * M_PI / (double)r->sides);
	reset_iters(r);
}

static int	key_handler_other_2(int keysym, t_game *r)
{
	if (keysym == XK_F5)
		change_sides(r, 5);
	else if (keysym == XK_F6)
		change_sides(r, 6);
	else if (keysym == XK_F7)
		change_sides(r, 7);
	else if (keysym == XK_F12)
		change_sides(r, 12);
	else if (keysym == XK_Control_L)
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
		change_sides(r, 4);
	else if (keysym == XK_F5)
		change_sides(r, 5);
	else if (keysym == XK_F6)
		change_sides(r, 6);
	else if (keysym == XK_F7)
		change_sides(r, 7);
	else if (keysym == XK_F12)
		change_sides(r, 12);
	else if (keysym == XK_Control_L)
	{
		r_init(r);
		info_init(r);
	}
	else
		key_handler_other_3layer(keysym, r);
	return (0);
}




void	set_home(t_game *r)
{
	rotate(r, -r->rotate);
	r->col_shift_x = ft_round((double)r->col_shift_x / r->zoom);
	r->col_shift_y = ft_round((double)r->col_shift_y / r->zoom);
	r->r = r->height *.4;
	if (r->dist_ratio > 1 && r->dist_ratio <= 2)
		r->zoom = (1 / r->ratio_change);
	else
		r->zoom = 1;
	r->iters_change = 0;
	r->move_x = 0.0;
	r->move_y = 0.0;
	reset_iters(r);
}

int	key_handler_other(int keysym, t_game *r)
{
	if (keysym == MULT || keysym == DIV)
		zoom_iters_key(keysym, r);
	else if (keysym == PAD_0)
		set_home(r);
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