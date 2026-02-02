/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:22:48 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 14:57:15 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"


//--------------------function pointer active method.......

//***setup individ rule fts.***// test with first 5

//1) not same as previous

/* static bool rule_1(int v, t_relations *rels)
{
	return (v == rels->prev);
}

//2) not same as left neighbor of prev

static bool rule_2(int v, t_relations *rels)
{
	return (v == rels->left_neighbor_of_prev);
}

//3) not same as right neighbor of prev

static bool rule_3(int v, t_relations *rels)
{
	return (v == rels->right_neighbor_of_prev);
}

//4) if double, no left or right neighbor of prev

static bool rule_4(int v, t_relations *rels)
{

	if (rels->one_before_prev == rels->prev)
	{
		if (v == rels->left_neighbor_of_prev)
			return (1);
		if (v == rels->right_neighbor_of_prev)
			return (1);
	}
	return (0);
}

//5) not same as one before prev

static bool rule_5(int v, t_relations *rels)
{
	return (v == rels->one_before_prev);
} */

//6) not the second to the left of prev
/* 
static bool rule_6(int v, t_relations *rels)
{
	return (v == rels->second_left_neigbor_of_prev);
} */



/* static const rule_ft RULE_FTS[MAX_R + 1] = 
{
	rule_1,
    rule_2,
    rule_3,
	rule_4,
	rule_5,
}; */
//	BUILD FUNCION THAT MAKES AN ARRAY OF TURNED ON RULE FT POINTERS NULL TERM AND TEST POINTS AGAINST. MUCH SPEED.
//NEED TO MAKE FTS FOR EACH RULE.

/* void	build_rules_active(const bool *rules, rule_ft active[MAX_R + 1])
{
	int i;
	int k;

	i = -1;
	k = 0;
	while (++i < MAX_R)
	{
		if (rules[i] && RULE_FTS[i])
			active[k++] = RULE_FTS[i];
	}
	active[k] = NULL;
} */

void	compute_rels(int rv[], t_relations *rels, t_game *r, long i)
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

int	ft_r(int rv[], int v, long i, t_game *r)
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

//printf(BOLD_BLUE"here\n"RESET);
///old rule functions setup----------------------------
/* static int	ft_r_2(int rv[], int v, long i, t_game *r)
{
	if ((v == rv[(i - 1) % 3]) && (v == rv[(i - 2) % 3]) && rules[7])
		return (1);
	if (v == (rv[(i - 1) % 3] - 3 + r->sides) % r->sides && rules[8])
		return (1);
	if (v == (rv[(i - 1) % 3] + 3) % r->sides && rules[9])
		return (1);
	if (v == (rv[(i - 1) % 3] - 4 + r->sides) % r->sides && rules[10])
		return (1);
	if (v == (rv[(i - 1) % 3] + 4) % r->sides && rules[11])
		return (1);
	if (rv[(i - 2) % 3] == rv[(i - 1) % 3] && rules[12])
	{
		if (v == (rv[(i - 1) % 3] - 2 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 1) % 3] + 2) % r->sides)
			return (1);
	}
	if (rv[(i - 2) % 3] == rv[(i - 1) % 3] && rules[13])
	{
		if (v == (rv[(i - 1) % 3] - 3 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 1) % 3] + 3) % r->sides)
			return (1);
	}
	return (0);
}

static int	ft_r_3(int rv[], int v, long i, t_game *r)
{
	if (v == (rv[(i - 2) % 3] - 1 + r->sides) % r->sides && rules[14])
		return (1);
	if (v == (rv[(i - 2) % 3] + 1) % r->sides && rules[15])
		return (1);
	if (rv[(i - 2) % 3] == rv[(i - 1) % 3] && rules[16])
	{
		if (v == (rv[(i - 1) % 3] - 4 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 1) % 3] + 4) % r->sides)
			return (1);
	}
	if (v == (rv[(i - 2) % 3] - 2 + r->sides) % r->sides && rules[17])
		return (1);
	if (v == (rv[(i - 2) % 3] + 2) % r->sides && rules[18])
		return (1);
	if (v == (rv[(i - 2) % 3] - 3 + r->sides) % r->sides && rules[19])
		return (1);
	if (v == (rv[(i - 2) % 3] + 3) % r->sides && rules[20])
		return (1);
	if (v == (rv[(i - 2) % 3] - 4 + r->sides) % r->sides && rules[21])
		return (1);
	if (v == (rv[(i - 2) % 3] + 4) % r->sides && rules[22])
		return (1);
	return (0);
}

static int	ft_r_4(int rv[], int v, long i, t_game *r)
{
	if ((rv[(i - 1) % 3] == (rv[(i - 2) % 3] + 1) % r->sides
			|| rv[(i - 1) % 3] == (rv[(i - 2) % 3] - 1 + r->sides)
			% r->sides) && rules[23])
	{
		if (v == (rv[(i - 2) % 3] - 1 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 2) % 3] + 1) % r->sides)
			return (1);
		if (v == rv[(i - 2) % 3] && rules[25])
			return (1);
	}
	if ((rv[(i - 1) % 3] == (rv[(i - 2) % 3] + 1) % r->sides
			|| rv[(i - 1) % 3] == (rv[(i - 2) % 3] - 1 + r->sides)
			% r->sides) && rules[24])
	{
		if (v == (rv[(i - 2) % 3] - 2 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 2) % 3] + 2) % r->sides)
			return (1);
		if (v == rv[(i - 2) % 3] && rules[25])
			return (1);
	}
	return (0);
}

static int	ft_r_5(int rv[], int v, long i, t_game *r)
{
	if ((rv[(i - 1) % 3] == (rv[(i - 2) % 3] + 1) % r->sides
			|| rv[(i - 1) % 3] == (rv[(i - 2) % 3] - 1 + r->sides)
			% r->sides) && rules[26])
	{
		if (v == (rv[(i - 3) % 3] - 1 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 3) % 3] + 1) % r->sides)
			return (1);
		if (v == rv[(i - 3) % 3] && rules[25])
			return (1);
	}
	if ((rv[(i - 1) % 3] == (rv[(i - 2) % 3] + 1) % r->sides
			|| rv[(i - 1) % 3] == (rv[(i - 2) % 3] - 1 + r->sides)
			% r->sides) && rules[27])
	{
		if (v == (rv[(i - 1) % 3] - 2 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 1) % 3] + 2) % r->sides)
			return (1);
		if (v == rv[(i - 1) % 3] && rules[25])
			return (1);
	}
	return (0);
}

int	ft_r(int rv[], int v, long i, t_game *r)
{
	//------new rules-----
	if ((rv[(i - 1) % 3] == (rv[(i - 2) % 3] + 1) % r->sides//29
			|| rv[(i - 1) % 3] == (rv[(i - 2) % 3] - 1 + r->sides)
			% r->sides) && rules[28])
	{
		if (v == (rv[(i - 2) % 3] - 3 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 2) % 3] + 3) % r->sides)
			return (1);
		if (v == rv[(i - 2) % 3] && rules[25])
			return (1);
	}
	//---------
	if ((v == rv[(i - 1) % 3]) && rules[0])
		return (1);
	if (v == (rv[(i - 1) % 3] - 1 + r->sides) % r->sides && rules[1])
		return (1);
	if (v == (rv[(i - 1) % 3] + 1) % r->sides && rules[2])
		return (1);
	if (rv[(i - 2) % 3] == rv[(i - 1) % 3] && rules[3])
	{
		if (v == (rv[(i - 1) % 3] - 1 + r->sides) % r->sides)
			return (1);
		if (v == (rv[(i - 1) % 3] + 1) % r->sides)
			return (1);
	}
	if (v == rv[(i - 2) % 3] && rules[4])
		return (1);
	if (v == (rv[(i - 1) % 3] - 2 + r->sides) % r->sides && rules[5])
		return (1);
	if (v == (rv[(i - 1) % 3] + 2) % r->sides && rules[6])
		return (1);
	if (ft_r_2(rv, v, i, r) || ft_r_3(rv, v, i, r))
		return (1);
	if (ft_r_4(rv, v, i, r) || ft_r_5(rv, v, i, r))
		return (1);
	return (0);
} */