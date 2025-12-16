/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chaos.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 13:05:45 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 13:56:05 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

//consider different method of updating rv array so that rv[2] is most recent, rv[1] = rv[2], rv[0] = rv[1] save separate and try reformat.
void	information(t_game *r, int v, long i)
{
	/* int position1, position2;
	
	position1 = (i - 1) % r->rv_len;
	r->prev = r->rv[position1];//save actual for depth color
	position2 = (i - 2) % r->rv_len;
	r->obp = r->rv[position2]; */
	if (r->jump_to_center && (v == r->points - 1))
		return ;
	if (r->disinfo_1)
	{
		r->rv[(i - 1) % r->rv_len] = v;
		//r->rv[position1] = v;
	}
	if (r->disinfo_2)
	{
		r->rv[(i - 2) % r->rv_len] = v;
		//r->rv[position2] = v;
	}
	else if (!r->disinfo_1)
	{
		r->rv[i % r->rv_len] = v;
	}

/* 	(void)i;
	if (r->disinfo_1)
	{
		r->tbp = r->obp;
		r->obp = v;
		//r->prev = r->tbp;
	}
	if (r->disinfo_2)
	{
		//this is making d2 + d1, others dont work..
		r->tbp = r->prev;
		r->prev = r->obp;
		r->obp = v;
	}
	else if (!r->disinfo_1)
	{
		r->tbp = r->obp;
		r->obp = r->prev;
		r->prev = v;
	} */
}

int	r_loop(t_game *r, Xoro128 *rng)
{
	int	v;
	int	tries;
	int p;

	tries = 0;
	p = r->points;
	v = xoro128(rng) % p;
	if (r->jump_to_center && (v == p - 1))// implement center rules here!
		return (v);
		//while (set_and_check(r->rv, v, r->i, r))
	while (ft_r(r->rv, v, r->i, r))//orig
	{
		v = xoro128(rng) % p;
		tries++;
		if (tries >= 250)
			return (-1);
	}
	return (v);
}

//test print
void	print_rv(int v, int iter, int rv[], int rv_len)
{
	int i;

	printf("current vert: %d\n", v);
	printf("iter: %d i_mod_rvlen: %d     (i - 1)mod_rvlen: %d     (i - 2)mod_rvlen: %d\n", iter, rv[iter % rv_len], rv[(iter - 1) % rv_len], rv[(iter - 2) % rv_len]);
	i = -1;
	while (++i < rv_len)
	{
		printf("index: %d vert: %d\n", i, rv[i]);
	}
	printf("\n");

}

void	chaos_game(t_game *r, int **vertices, double x, double y)
{
	int		v;
	Xoro128 *rng = &r->rng;


	//test for new radius
	/* double big = 0.0;//
	double dist = 0.0;// */

//	int d = r->dist_ratio;
	r->i = 1;
	polygon(&vertices, r);
	while (++r->i <= r->iters + 1)
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		//print_rv(v, r->i, r->rv, r->rv_len);
		
		if (r->jump_to_center && (v == r->points - 1))//handle center
		{
			x = x + (r->center[0] - x) * r->dist_ratio;
			y = y + (r->center[1] - y) * r->dist_ratio;
		}
		else
		{	
			x = x + (vertices[v][0] - x) * r->dist_ratio; //normal
			y = y + (vertices[v][1] - y) * r->dist_ratio;
		}	 
	/* if(v % 2 == 0) //for alternating vertex ratios
		{
		x = x + (vertices[v][0] - x) * r->dist_ratio; 
		y = y + (vertices[v][1] - y) * r->dist_ratio;
		}
		else
		{
		x = x + (vertices[v][0] - x) * r->dist_ratio * 2 / 3; //.. *2, *2/3 ...etc
		y = y + (vertices[v][1] - y) * r->dist_ratio * 2 / 3;
		} */

		if ((r->i > 10) && (x > 0 && x < r->width) && (y > 0 && y < r->height))
			color_option(r, v, x, y, vertices);

		//test big
		/* dist = sqrt((x - r->center[0]) * (x - r->center[0]) + (y - r->center[1]) * (y - r->center[1]));
		if (dist > big)
			big = dist; */
		//information(r, v, r->i);

	}
	//print test new rad
	//printf("amount dist ratio is over 1: %f radius increase factor: %f\n", r->dist_ratio - 1, big / r->r);


	free_poly(&vertices, r->points - r->jump_to_center);
	free_poly(&r->vertices2, r->points - r->jump_to_center);

}

void	chaos_game_curved(t_game *r, int **vertices, double x, double y)
{
	int		v;
	Xoro128	*rng = &r->rng;

	double ratio_end = r->dist_ratio;
	double shift = (ratio_end - r->ratio_start) / (double)r->iters;
	
	r->dist_ratio = r->ratio_start;
	//adjust_ratio(r, r->ratio_start);
	r->i = 1;
	polygon(&vertices, r);
	while (++r->i <= r->iters + 2)
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		//adjust_ratio(r, r->dist_ratio + shift);
		r->dist_ratio += shift;
		if (r->jump_to_center && (v == r->points - 1))//handle center
		{
			x = x + (r->center[0] - x) * r->dist_ratio;
			y = y + (r->center[1] - y) * r->dist_ratio;
		}
		else
		{	
			x = x + (vertices[v][0] - x) * r->dist_ratio; //other verts
			y = y + (vertices[v][1] - y) * r->dist_ratio;
		}	 
		/* if(v % 2 == 0)
		{
		x = x + (vertices[v][0] - x) * r->dist_ratio; 
		y = y + (vertices[v][1] - y) * r->dist_ratio;
		}
		else
		{
		x = x + (vertices[v][0] - x) * r->dist_ratio * 2 / 3; //.. *2, *2/3 ...etc
		y = y + (vertices[v][1] - y) * r->dist_ratio * 2 / 3;
		} */
		if ((r->i > 10) && (x > 0 && x < r->width) && (y > 0 && y < r->height))
			color_option(r, v, x, y, vertices);
	}
	free_poly(&vertices, r->points - r->jump_to_center);
	free_poly(&r->vertices2, r->points - r->jump_to_center);

	r->dist_ratio = ratio_end;
}
