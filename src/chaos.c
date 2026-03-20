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

#include "color_ops.h"
#include "xoro128.h"

static inline void	compute_rels(int rv[], t_relations *rels, t_game *r, long i)
{
	rels->prev = rv[(i - 1) % r->rv_len];
	rels->one_before_prev = rv[(i - 2) % r->rv_len];
	rels->left_neighbor_of_prev = (rels->prev - 1 + r->points) % r->points;
	rels->right_neighbor_of_prev = (rels->prev + 1) % r->points;
}

int	set_and_check(int rv[], int v, long i, t_game *r)
{

	t_relations	rels;
	rule_ft		*ftp;

	compute_rels(rv, &rels, r, i);
	ftp = (rule_ft *)r->active;
	//set up rule array of ft pointers and check
	while (*ftp != NULL)
	{
		if ((*ftp)(v, &rels))
			return (1);
		ftp++;			

	}
	return (0);
}
//--------------------------------------------------



static inline int	ft_r_2(int v, t_game *r, t_relations *rels, uint32_t m)
{
	rels->second_left_neigbor_of_prev = (rels->prev - 2 + r->points) % r->points;
	rels->second_right_neigbor_of_prev = (rels->prev + 2) % r->points;
	rels->third_left_neigbor_of_prev = (rels->prev - 3 + r->points) % r->points;
	rels->third_right_neigbor_of_prev = (rels->prev + 3) % r->points;
	rels->fourth_left_neigbor_of_prev = (rels->prev - 4 + r->points) % r->points;
	rels->fourth_right_neigbor_of_prev = (rels->prev + 4) % r->points;


    if (m & (1u << 5)) if (v == rels->second_left_neigbor_of_prev) return 1;//rule 6
    if (m & (1u << 6)) if (v == rels->second_right_neigbor_of_prev) return 1;
    if (m & (1u << 7)) if ((v == rels->prev) && (v == rels->one_before_prev)) return 1;
    if (m & (1u << 8)) if (v == rels->third_left_neigbor_of_prev) return 1;
    if (m & (1u << 9)) if (v == rels->third_right_neigbor_of_prev) return 1;
    if (m & (1u << 10)) if (v == rels->fourth_left_neigbor_of_prev) return 1;
    if (m & (1u << 11)) if (v == rels->fourth_right_neigbor_of_prev) return 1;
    if (m & (1u << 16)) 
	
	if (rels->one_before_prev == rels->prev)//if double
	{
		if (v == rels->fourth_left_neigbor_of_prev)
			return (1);
		if (v == rels->fourth_right_neigbor_of_prev)
			return (1);
	}

    if (m & (1u << 12)) 
		if (rels->one_before_prev == rels->prev)//if double
		{
			if (v == rels->second_left_neigbor_of_prev)
				return (1);
			if (v == rels->second_right_neigbor_of_prev)
				return (1);
		}
    
	if (m & (1u << 13)) 
		if (rels->one_before_prev == rels->prev)//if double
		{
			if (v == rels->third_left_neigbor_of_prev)
				return (1);
			if (v == rels->third_right_neigbor_of_prev)
				return (1);
		}
	return (0);
}

static inline int	ft_r_3(int v, t_game *r, t_relations *rels, uint32_t m)
{
	rels->left_neigbor_of_one_before_prev = (rels->one_before_prev - 1 + r->points) % r->points;
	rels->right_neigbor_of_one_before_prev = (rels->one_before_prev + 1) % r->points;
	rels->second_left_neigbor_of_one_before_prev = (rels->one_before_prev - 2 + r->points) % r->points;
	rels->second_right_neigbor_of_one_before_prev = (rels->one_before_prev + 2) % r->points;
	rels->third_left_neigbor_of_one_before_prev = (rels->one_before_prev - 3 + r->points) % r->points;
	rels->third_right_neigbor_of_one_before_prev = (rels->one_before_prev + 3) % r->points;
	rels->fourth_left_neigbor_of_one_before_prev = (rels->one_before_prev - 4 + r->points) % r->points;
	rels->fourth_right_neigbor_of_one_before_prev = (rels->one_before_prev + 4) % r->points;

    if (m & (1u << 14)) if (v == rels->left_neigbor_of_one_before_prev) return 1;//rule 15
    if (m & (1u << 15)) if (v == rels->right_neigbor_of_one_before_prev) return 1;
    if (m & (1u << 17)) if (v == rels->second_left_neigbor_of_one_before_prev) return 1;
    if (m & (1u << 18)) if (v == rels->second_right_neigbor_of_one_before_prev) return 1;
    if (m & (1u << 19)) if (v == rels->third_left_neigbor_of_one_before_prev) return 1;
    if (m & (1u << 20)) if (v == rels->third_right_neigbor_of_one_before_prev) return 1;
    if (m & (1u << 21)) if (v == rels->fourth_left_neigbor_of_one_before_prev) return 1;
    if (m & (1u << 22)) if (v == rels->fourth_right_neigbor_of_one_before_prev) return 1;

	return (0);
}

static inline int	ft_r_4(int v, t_relations *rels, uint32_t m)
{
    if (m & (1u << 23)) 
		if (rels->prev == rels->right_neigbor_of_one_before_prev 
			|| rels->prev == rels->left_neigbor_of_one_before_prev)//rule 24
		{
			if (v == rels->left_neigbor_of_one_before_prev)
				return (1);
			if (v == rels->right_neigbor_of_one_before_prev)
				return (1);
			if (m & (1u << 25) && v == rels->one_before_prev)
				return (1);
		}
	
	if (m & (1u << 24) && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == rels->second_left_neigbor_of_one_before_prev)
			return (1);
		if (v == rels->second_right_neigbor_of_one_before_prev)
			return (1);
		if (m & (1u << 25) && v == rels->one_before_prev)
			return (1);
	}
	return (0);
}

static inline int	ft_r_5(int rv[], int v, long i, t_game *r, t_relations *rels, uint32_t m)
{
	if (m & (1u << 26) && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == (rv[(i - 3) % r->rv_len] - 1 + r->points) % r->points)
			return (1);
		if (v == (rv[(i - 3) % r->rv_len] + 1) % r->points)
			return (1);
		if (m & (1u << 25) && v == rv[(i - 3) % r->rv_len])
			return (1);
	}
	if (m & (1u << 27) && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{							
		if (v == rels->second_left_neigbor_of_prev)
			return (1);
		if (v == rels->second_right_neigbor_of_prev)
			return (1);
		if (m & (1u << 25) && v == rels->prev)
			return (1);
	}
	if (m & (1u << 28) && (rels->prev == rels->right_neigbor_of_one_before_prev//29
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == rels->third_left_neigbor_of_one_before_prev)
			return (1);
		if (v == rels->third_right_neigbor_of_one_before_prev)
			return (1);
		if (m & (1u << 25) && v == rels->one_before_prev)
			return (1);
	}
	return (0);
}

static inline int	ft_r(int rv[], int v, long i, t_game *r)
{
	t_relations			rels;
	//const bool			*rules = r->rules;
	
	//try keeping prev, obp, tbp in rolling list
	/* rels.prev = r->prev;
	rels.one_before_prev = r->obp; */

	rels.prev = rv[(i - 1) % r->rv_len];
	rels.one_before_prev = rv[(i - 2) % r->rv_len];

	rels.left_neighbor_of_prev = (rels.prev - 1 + r->points) % r->points;
	rels.right_neighbor_of_prev = (rels.prev + 1) % r->points;
	
	uint32_t m = r->rules_mask;

    if (m & (1u << 0)) if (v == rels.prev) return 1;//rule 1
    if (m & (1u << 1)) if (v == rels.left_neighbor_of_prev) return 1;
    if (m & (1u << 2)) if (v == rels.right_neighbor_of_prev) return 1;
    if (m & (1u << 3))
	{
        if (rels.one_before_prev == rels.prev) 
		{
            if (v == rels.left_neighbor_of_prev || v == rels.right_neighbor_of_prev) 
				return 1;
        }
    }
    // Rule 5
    if (m & (1u << 4)) if (v == rels.one_before_prev) return 1;

	if (ft_r_2(v, r, &rels, m) || ft_r_3(v, r, &rels, m))
		return (1);
	if (ft_r_4(v, &rels, m) || ft_r_5(rv, v, i, r, &rels, m))
		return (1);
	return (0);
}

//consider different method of updating rv array so that rv[2] is most recent, rv[1] = rv[2], rv[0] = rv[1], or use saved POTT
void	information(t_game *r, int v, long i)
{

	if (r->jump_to_center && (v == r->points - 1))
		return ;
	if (r->disinfo_1)
	{
		r->rv[(i - 1) % r->rv_len] = v;
	}
	if (r->disinfo_2)
	{
		r->rv[(i - 2) % r->rv_len] = v;
	}
	else if (!r->disinfo_1)
	{
		r->rv[i % r->rv_len] = v;
	}
}

static inline int	r_loop(t_game *r, Xoro128 *rng)
{
	int	v;
	int	tries;
	int p;

	tries = 0;
	p = r->points;
	v = xoro128(rng) % p;
	if (r->jump_to_center && (v == p - 1))// implement center rules here
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
	printf("iter: %d i_mod_rvlen: %d     (i - 1)mod_rvlen: %d     (i - 2)mod_rvlen: %d\n", \
		iter, rv[iter % rv_len], rv[(iter - 1) % rv_len], rv[(iter - 2) % rv_len]);
	i = -1;
	while (++i < rv_len)
	{
		printf("index: %d vert: %d\n", i, rv[i]);
	}
	printf("\n");

}

static inline void update_pott(t_game *r, int v)//Prev,Onebeforeprev, Twobefore, Threebef...POTT
{
	r->thbp = r->tbp;
	r->tbp = r->obp;
	r->obp = r->prev;
	r->prev = v;
}

void	chaos_game(t_game *r, int **vertices, double x, double y)
{
	int		v;
	Xoro128 *rng = &r->rng;

	r->i = 1;
	polygon(&vertices, r);
	while (++r->i <= 15)//dont plot first few to let convergence begin
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		x += (vertices[v][0] - x) * r->dist_ratio;
		y += (vertices[v][1] - y) * r->dist_ratio; 
	}
	while (++r->i <= r->iters + 1)
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		//print_rv(v, r->i, r->rv, r->rv_len);
		x += ((double)vertices[v][0] - x) * r->dist_ratio;
		y += ((double)vertices[v][1] - y) * r->dist_ratio; 
		
	/* 	if (v % 2 == 0) //for alternating vertex ratios
		{
			x = x + ((double)vertices[v][0] - x) * r->dist_ratio; 
			y = y + ((double)vertices[v][1] - y) * r->dist_ratio;
		}
		else
		{
			x += ((double)vertices[v][0] - x) * r->dist_ratio * 2 / 3; //.. *2, *2/3 ...etc
			y += ((double)vertices[v][1] - y) * r->dist_ratio * 2 / 3;
		} */
		if ((x > 0 && x < r->width) && (y > 0 && y < r->height))
			//if (v == 0) 
				color_option(r, v, x, y, vertices);
		update_pott(r, v);
	}
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
	while (++r->i <= 15)//dont plot first few to let convergence begin
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		x += (vertices[v][0] - x) * r->dist_ratio;
		y += (vertices[v][1] - y) * r->dist_ratio; 
	}
	while (++r->i <= r->iters + 2)
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		//adjust_ratio(r, r->dist_ratio + shift);
		r->dist_ratio += shift;	
		x += (vertices[v][0] - x) * r->dist_ratio; //other verts
		y += (vertices[v][1] - y) * r->dist_ratio;
		//test color center no jump.. use points that cross over center, when moving toward opposite point and is within dist


		/* if(v % 2 == 0)
		{
		x += (vertices[v][0] - x) * r->dist_ratio; 
		y += (vertices[v][1] - y) * r->dist_ratio;
		}
		else
		{
		x += (vertices[v][0] - x) * r->dist_ratio * 2 / 3; //.. *2, *2/3 ...etc
		y += (vertices[v][1] - y) * r->dist_ratio * 2 / 3;
		} */
		if ((x > 0 && x < r->width) && (y > 0 && y < r->height))
			color_option(r, v, x, y, vertices);
		update_pott(r, v);
	}
	free_poly(&vertices, r->points - r->jump_to_center);
	free_poly(&r->vertices2, r->points - r->jump_to_center);

	r->dist_ratio = ratio_end;
}


double submode(t_game *r, int v, double ft_result)
{
	if (r->mode == 2)
		return (ft_result);	
	else if (r->mode == 3)
	{
		if (v % 2 == 0)
			return (r->dist_ratio);
		else
			return (ft_result);
	}
	else
	{
		if (v % 2 != 0)
			return (r->dist_ratio);
		else
			return (ft_result);
	}
}
	//	return (r->ratio_start * 2 * sin(log(drz)) * sin(log(drz) * M_PI));
	//	return (submode(r, v, r->ratio_start * 2 * cos(cos(log(drz) * M_PI)))); eh not bad..

double	apply_function(t_game *r, double d, int **vertices, int v)//, generic function change with keys recieves distance to vertex and applies ft.
{
	(void)vertices;
	double	drz = d * r->rz;//rz = (1 / ((double)r->r * r->zoom));
	if (r->function_id == 1)
		return (submode(r, v, r->ratio_start * 2 * sin(drz * M_PI)));
	else if (r->function_id == 2)
		return (submode(r, v, r->ratio_start * 2 * cos(drz * M_PI)));
	else if (r->function_id == 3)
		return (submode(r, v, r->ratio_start * 2 * cos(cos(drz) * M_PI)));
	else if (r->function_id == 4)
		return (submode(r, v, r->ratio_start * 2 * sin(sin(drz) * M_PI)));
	else if (r->function_id == 5)
		return(submode(r, v, r->ratio_start * 2 * cos(log(drz) * M_PI)));
	else if (r->function_id == 6)
		return(submode(r, v, r->ratio_start * 2 * cos(d / (double)r->r * M_PI) * cos(drz * M_PI)));
	else if (r->function_id == 7)
		return (submode(r, v, r->ratio_start * 2 * sin(cos(drz) * cos(drz) * M_PI)));				
	else if (r->function_id == 8)
		return (submode(r, v, r->ratio_start * 2 * cos(sin(drz) * sin(drz) * M_PI)));	
	else if (r->function_id == 9)
		return(submode(r, v, r->ratio_start * 2 * sin(log(drz) * M_PI)));
	else if (r->function_id == 10)
		return (submode(r, v, r->ratio_start * 2 * cos(log(drz) * log(drz) * M_PI)));			
	else
		return (r->dist_ratio);

			//return (r->dist_ratio + (r->dist_ratio - r->ratio_start) * cos(log(drz) * M_PI));
		
			
			//return (r->dist_ratio);
//return (r->ratio_start + shift);
//return (r->ratio_start + shift);// + shift);//try diff function to apply here..
		

	//double val = (pow(cos(d), 2));

	
	//return(r->ratio_start * (2  * cos(x * M_PI) + 2 * sin(x * M_PI)));//some intersting stuff here.. try diff combos
	
	//return(r->ratio_start * ( 2 * cos(x) * cos (x) + 3 * sin(x) * sin(x)));//some intersting stuff here.. try diff combos
}

void	chaos_game_apply_function(t_game *r, int **vertices, double x, double y)//try to app function and change ratio here
{
	int		v;
	double	distance_to_vert;
	double curr_ratio;
	Xoro128 *rng = &r->rng;
	//int		bv;
	
	r->i = 1;
	polygon(&vertices, r);
	while (++r->i <= 10)//dont plot first few to let convergence begin
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		x += (vertices[v][0] - x) * r->dist_ratio;
		y += (vertices[v][1] - y) * r->dist_ratio; 
	}
	while (++r->i <= r->iters + 1)
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		
		/* if (r->sides > 1) doesnt seem to make difference
			bv = (r->i - 2) % r->rv_len;
		else
			bv = 0;
		distance_to_vert = get_dist(x, y, vertices[r->rv[bv]][0], vertices[r->rv[bv]][1]);//previous vert
		 */
		/* if (r->jump_to_center && (v == r->points - 1))
			distance_to_vert = get_dist(x, y, r->center[0], r->center[1]);// current vert set as center...//can try not applying function if center
		else */
			distance_to_vert = get_dist(x, y, vertices[v][0], vertices[v][1]);// current vert
		curr_ratio = apply_function(r, distance_to_vert, vertices, v);// get ratio based on function
		x += (vertices[v][0] - x) * curr_ratio; //normal
		y += (vertices[v][1] - y) * curr_ratio;
		
		if ((x > 0 && x < r->width) && (y > 0 && y < r->height))
			color_option(r, v, x, y, vertices);
		update_pott(r, v);
	}
	free_poly(&vertices, r->points - r->jump_to_center);
	free_poly(&r->vertices2, r->points - r->jump_to_center);
}

//maybe separate for center rules handle?
/* if (r->jump_to_center && (v == r->points - 1))//handle center
		{
			x += (r->center[0] - x) * r->dist_ratio;
			y += (r->center[1] - y) * r->dist_ratio;
		}
		else
		{	
			x += (vertices[v][0] - x) * r->dist_ratio; //other verts
			y += (vertices[v][1] - y) * r->dist_ratio;
		}	  */