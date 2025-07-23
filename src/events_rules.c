/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_rules.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 13:57:07 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:12:01 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"


void	zoom_iters_key(int keysym, t_game *r)
{
	if (keysym == MULT)
		r->zoom *= 3;
	else if (keysym == DIV)
		r->zoom /= 3;
	r->iters = (4 + r->iters_change) * r->dist_ratio * r->dist_ratio * (r->r * r->zoom) * (r->r * r->zoom) * sqrt(3);
}

static int	key_handler_r_3(int keysym, t_game *r)
{
	if (keysym == KEY_S)
		r->rules[21] = !r->rules[21];
	else if (keysym == KEY_D)
		r->rules[22] = !r->rules[22];
	else if (keysym == KEY_F)
		r->rules[23] = !r->rules[23];
	else if (keysym == KEY_G)
		r->rules[24] = !r->rules[24];
	else if (keysym == KEY_H)
		r->rules[25] = !r->rules[25];
	else if (keysym == KEY_Ö)
		r->rules[26] = !r->rules[26];
	else if (keysym == KEY_Ä)
		r->rules[27] = !r->rules[27];
	else if (keysym == KEY_Ü)
		r->rules[28] = !r->rules[28];
	else if (keysym == KEY_J)
		r->disinfo_1 = !r->disinfo_1;
	else if (keysym == KEY_K)
		r->disinfo_2 = !r->disinfo_2;
	else
		key_handler_other(keysym, r);
	return (0);
}

static int	key_handler_r_2(int keysym, t_game *r)
{
	if (keysym == KEY_Q)
		r->rules[10] = !r->rules[10];
	else if (keysym == KEY_W)
		r->rules[11] = !r->rules[11];
	else if (keysym == KEY_E)
		r->rules[12] = !r->rules[12];
	else if (keysym == KEY_R)
		r->rules[13] = !r->rules[13];
	else if (keysym == KEY_T)
		r->rules[14] = !r->rules[14];
	else if (keysym == KEY_Z)
		r->rules[15] = !r->rules[15];
	else if (keysym == KEY_U)
		r->rules[16] = !r->rules[16];
	else if (keysym == KEY_I)
		r->rules[17] = !r->rules[17];
	else if (keysym == KEY_O)
		r->rules[18] = !r->rules[18];
	else if (keysym == KEY_P)
		r->rules[19] = !r->rules[19];
	else if (keysym == KEY_A)
		r->rules[20] = !r->rules[20];
	else
		key_handler_r_3(keysym, r);
	return (0);
}

int	key_handler_r(int keysym, t_game *r)
{
	if (keysym == N_1)
		r->rules[0] = !r->rules[0];
	else if (keysym == N_2)
		r->rules[1] = !r->rules[1];
	else if (keysym == N_3)
		r->rules[2] = !r->rules[2];
	else if (keysym == N_4)
		r->rules[3] = !r->rules[3];
	else if (keysym == N_5)
		r->rules[4] = !r->rules[4];
	else if (keysym == N_6)
		r->rules[5] = !r->rules[5];
	else if (keysym == N_7)
		r->rules[6] = !r->rules[6];
	else if (keysym == N_8)
		r->rules[7] = !r->rules[7];
	else if (keysym == N_9)
		r->rules[8] = !r->rules[8];
	else if (keysym == N_0)
		r->rules[9] = !r->rules[9];
	else
		key_handler_r_2(keysym, r);
	return (0);
}


static int	key_handler_rlayer_3(int keysym, t_game *r)
{
	if (keysym == KEY_S)
		r->function_id = 12;
	else if (keysym == KEY_D)
		r->function_id = 13;
	else if (keysym == KEY_F)
		r->function_id = 14;
	else if (keysym == KEY_G)
		r->function_id = 15;
	else if (keysym == KEY_H)
		r->function_id = 16;
	else if (keysym == KEY_J)
		r->function_id = 17;
	else if (keysym == KEY_K)
		r->function_id = 18;
	else if (keysym == KEY_L)
		r->function_id = 19;
	else if (keysym == KEY_Ö)
		r->function_id = 20;
	/* else if (keysym == Ä)
		r->function_id = 18;
	else if (keysym == Ü)
		r->rules[28] = !r->rules[28]; */
	else
		key_handler_otherlayer(keysym, r);
	return (0);
}

static int	key_handler_rlayer_2(int keysym, t_game *r)
{
	if (keysym == KEY_Q)
		r->function_id = 1;
	else if (keysym == KEY_W)
		r->function_id = 2;
	else if (keysym == KEY_E)
		r->function_id = 3;
	else if (keysym == KEY_R)
		r->function_id = 4;
	else if (keysym == KEY_T)
		r->function_id = 5;
	else if (keysym == KEY_Z)
		r->function_id = 6;
	else if (keysym == KEY_U)
		r->function_id = 7;
	else if (keysym == KEY_I)
		r->function_id = 8;
	else if (keysym == KEY_O)
		r->function_id = 9;
	else if (keysym == KEY_P)
		r->function_id = 10;
	else if (keysym == KEY_A)
		r->function_id = 11;
	else
		key_handler_rlayer_3(keysym, r);
	return (0);
}

int	key_handler_rlayer(int keysym, t_game *r)
{
	if (keysym == N_1)
		r->rules[0] = !r->rules[0];
	else if (keysym == N_2)
		r->rules[1] = !r->rules[1];
	else if (keysym == N_3)
		r->rules[2] = !r->rules[2];
	else if (keysym == N_4)
		r->rules[3] = !r->rules[3];
	else if (keysym == N_5)
		r->rules[4] = !r->rules[4];
	else if (keysym == N_6)
		r->rules[5] = !r->rules[5];
	else if (keysym == N_7)
		r->rules[6] = !r->rules[6];
	else if (keysym == N_8)
		r->rules[7] = !r->rules[7];
	else if (keysym == N_9)
		r->rules[8] = !r->rules[8];
	else if (keysym == N_0)
		r->rules[9] = !r->rules[9];
	else
		key_handler_rlayer_2(keysym, r);
	return (0);
}