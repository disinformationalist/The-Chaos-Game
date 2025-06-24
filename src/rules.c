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

/* 	r->rules_mask = 0;
	for (int i = 0; i < 30; i++) {
		if (r->rules[i])
			r->rules_mask |= (1u << i);
	} */

/* int ft_r(int rv[], int v, long i, t_game *r) {
	t_relations rels;
	uint32_t rules = r->rules_mask; // new uint32_t representation

	int points = r->points;
	long rv_len = r->rv_len;

	rels.prev = rv[(i - 1 + rv_len) % rv_len];
	rels.one_before_prev = rv[(i - 2 + rv_len) % rv_len];

	int prev = rels.prev;
	int obp = rels.one_before_prev;

	int left = wrap_index(prev - 1, points);
	int right = wrap_index(prev + 1, points);

	// Rule 0: cannot be same as prev
	if (RULE(rules, 0) && v == prev) return 1;
	if (RULE(rules, 1) && v == left) return 1;
	if (RULE(rules, 2) && v == right) return 1;

	if (RULE(rules, 3) && obp == prev) {
		if (v == left || v == right) return 1;
	}
	if (RULE(rules, 4) && v == obp) return 1;

	// Secondary rules
	if (RULE(rules, 5) && v == wrap_index(prev - 2, points)) return 1;
	if (RULE(rules, 6) && v == wrap_index(prev + 2, points)) return 1;
	if (RULE(rules, 7) && v == prev && obp == prev) return 1;
	if (RULE(rules, 8) && v == wrap_index(prev - 3, points)) return 1;
	if (RULE(rules, 9) && v == wrap_index(prev + 3, points)) return 1;
	if (RULE(rules, 10) && v == wrap_index(prev - 4, points)) return 1;
	if (RULE(rules, 11) && v == wrap_index(prev + 4, points)) return 1;

	// Rule 16: if same, forbid 4-away neighbors
	if (RULE(rules, 16) && obp == prev) {
		if (v == wrap_index(prev - 4, points) || v == wrap_index(prev + 4, points)) return 1;
	}
	if (RULE(rules, 12) && obp == prev) {
		if (v == wrap_index(prev - 2, points) || v == wrap_index(prev + 2, points)) return 1;
	}
	if (RULE(rules, 13) && obp == prev) {
		if (v == wrap_index(prev - 3, points) || v == wrap_index(prev + 3, points)) return 1;
	}

	// From one_before_prev
	int obp_left = wrap_index(obp - 1, points);
	int obp_right = wrap_index(obp + 1, points);
	if (RULE(rules, 14) && v == obp_left) return 1;
	if (RULE(rules, 15) && v == obp_right) return 1;
	if (RULE(rules, 17) && v == wrap_index(obp - 2, points)) return 1;
	if (RULE(rules, 18) && v == wrap_index(obp + 2, points)) return 1;
	if (RULE(rules, 19) && v == wrap_index(obp - 3, points)) return 1;
	if (RULE(rules, 20) && v == wrap_index(obp + 3, points)) return 1;
	if (RULE(rules, 21) && v == wrap_index(obp - 4, points)) return 1;
	if (RULE(rules, 22) && v == wrap_index(obp + 4, points)) return 1;

	// Rule 23 and 24: if prev is neighbor of obp
	if ((RULE(rules, 23) || RULE(rules, 24)) &&
	    (prev == obp_left || prev == obp_right)) {
		if (RULE(rules, 23)) {
			if (v == obp_left || v == obp_right) return 1;
			if (RULE(rules, 25) && v == obp) return 1;
		}
		if (RULE(rules, 24)) {
			if (v == wrap_index(obp - 2, points) || v == wrap_index(obp + 2, points)) return 1;
			if (RULE(rules, 25) && v == obp) return 1;
		}
	}

	// Rule 26 and 27: special logic with 3rd last val
	int third = rv[(i - 3 + rv_len) % rv_len];
	if ((RULE(rules, 26) || RULE(rules, 27)) && (prev == obp_left || prev == obp_right)) {
		if (RULE(rules, 26)) {
			if (v == wrap_index(third - 1, points) || v == wrap_index(third + 1, points)) return 1;
			if (RULE(rules, 25) && v == third) return 1;
		}
		if (RULE(rules, 27)) {
			if (v == wrap_index(prev - 2, points) || v == wrap_index(prev + 2, points)) return 1;
			if (RULE(rules, 25) && v == prev) return 1;
		}
	}

	// Rule 28: new triple-step logic
	if (RULE(rules, 28) && (prev == obp_left || prev == obp_right)) {
		if (v == wrap_index(obp - 3, points) || v == wrap_index(obp + 3, points)) return 1;
		if (RULE(rules, 25) && v == obp) return 1;
	}

	return 0;
} */


static int	ft_r_2(int v, t_game *r, t_relations *rels, const bool* rules)
{
	rels->second_left_neigbor_of_prev = (rels->prev - 2 + r->points) % r->points;
	rels->second_right_neigbor_of_prev = (rels->prev + 2) % r->points;
	rels->third_left_neigbor_of_prev = (rels->prev - 3 + r->points) % r->points;
	rels->third_right_neigbor_of_prev = (rels->prev + 3) % r->points;
	rels->fourth_left_neigbor_of_prev = (rels->prev - 4 + r->points) % r->points;
	rels->fourth_right_neigbor_of_prev = (rels->prev + 4) % r->points;

	if (rules[5] && v == rels->second_left_neigbor_of_prev)
		return (1);
	if (rules[6] && v == rels->second_right_neigbor_of_prev)
		return (1);
	if (rules[7] && (v == rels->prev) && (v == rels->one_before_prev))
		return (1);
	if (rules[8] && v == rels->third_left_neigbor_of_prev)
		return (1);
	if (rules[9] && v == rels->third_right_neigbor_of_prev)
		return (1);
	if (rules[10] && v == rels->fourth_left_neigbor_of_prev)
		return (1);
	if (rules[11] && v == rels->fourth_right_neigbor_of_prev)
		return (1);
	if (rules[16] && rels->one_before_prev == rels->prev)
	{
		if (v == rels->fourth_left_neigbor_of_prev)
			return (1);
		if (v == rels->fourth_right_neigbor_of_prev)
			return (1);
	}
	if (rules[12] && rels->one_before_prev == rels->prev)
	{
		if (v == rels->second_left_neigbor_of_prev)
			return (1);
		if (v == rels->second_right_neigbor_of_prev)
			return (1);
	}
	if (rules[13] && rels->one_before_prev == rels->prev)
	{
		if (v == rels->third_left_neigbor_of_prev)
			return (1);
		if (v == rels->third_right_neigbor_of_prev)
			return (1);
	}
	return (0);
}

static int	ft_r_3(int v, t_game *r, t_relations *rels, const bool* rules)
{
	rels->left_neigbor_of_one_before_prev = (rels->one_before_prev - 1 + r->points) % r->points;
	rels->right_neigbor_of_one_before_prev = (rels->one_before_prev + 1) % r->points;
	rels->second_left_neigbor_of_one_before_prev = (rels->one_before_prev - 2 + r->points) % r->points;
	rels->second_right_neigbor_of_one_before_prev = (rels->one_before_prev + 2) % r->points;
	rels->third_left_neigbor_of_one_before_prev = (rels->one_before_prev - 3 + r->points) % r->points;
	rels->third_right_neigbor_of_one_before_prev = (rels->one_before_prev + 3) % r->points;
	rels->fourth_left_neigbor_of_one_before_prev = (rels->one_before_prev - 4 + r->points) % r->points;
	rels->fourth_right_neigbor_of_one_before_prev = (rels->one_before_prev + 4) % r->points;

	if (rules[14] && v == rels->left_neigbor_of_one_before_prev)
		return (1);
	if (rules[15] && v == rels->right_neigbor_of_one_before_prev)
		return (1);
	if (rules[17] && v == rels->second_left_neigbor_of_one_before_prev)
		return (1);
	if (rules[18] && v == rels->second_right_neigbor_of_one_before_prev)
		return (1);
	if (rules[19] && v == rels->third_left_neigbor_of_one_before_prev)
		return (1);
	if (rules[20] && v == rels->third_right_neigbor_of_one_before_prev)
		return (1);
	if (rules[21] && v == rels->fourth_left_neigbor_of_one_before_prev)
		return (1);
	if (rules[22] && v == rels->fourth_right_neigbor_of_one_before_prev)
		return (1);
	return (0);
}

static int	ft_r_4(int v, t_relations *rels, const bool* rules)
{
	if (rules[23] && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == rels->left_neigbor_of_one_before_prev)
			return (1);
		if (v == rels->right_neigbor_of_one_before_prev)
			return (1);
		if (rules[25] && v == rels->one_before_prev)
			return (1);
	}
	if (rules[24] && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == rels->second_left_neigbor_of_one_before_prev)
			return (1);
		if (v == rels->second_right_neigbor_of_one_before_prev)
			return (1);
		if (rules[25] && v == rels->one_before_prev)
			return (1);
	}
	return (0);
}

static int	ft_r_5(int rv[], int v, long i, t_game *r, t_relations *rels, const bool* rules)
{
	if (rules[26] && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == (rv[(i - 3) % r->rv_len] - 1 + r->points) % r->points)
			return (1);
		if (v == (rv[(i - 3) % r->rv_len] + 1) % r->points)
			return (1);
		if (rules[25] && v == rv[(i - 3) % r->rv_len])
			return (1);
	}
	if (rules[27] && (rels->prev == rels->right_neigbor_of_one_before_prev
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{							
		if (v == rels->second_left_neigbor_of_prev)
			return (1);
		if (v == rels->second_right_neigbor_of_prev)
			return (1);
		if (rules[25] && v == rels->prev)
			return (1);
	}
	//------new rules-----
	if (rules[28] && (rels->prev == rels->right_neigbor_of_one_before_prev//29
			|| rels->prev == rels->left_neigbor_of_one_before_prev))
	{
		if (v == rels->third_left_neigbor_of_one_before_prev)
			return (1);
		if (v == rels->third_right_neigbor_of_one_before_prev)
			return (1);
		if (rules[25] && v == rels->one_before_prev)
			return (1);
	}
	//---------
	return (0);
}

int	ft_r(int rv[], int v, long i, t_game *r)
{
	t_relations rels;
	const bool			*rules = r->rules;
	
	rels.prev = rv[(i - 1) % r->rv_len];
	rels.one_before_prev = rv[(i - 2) % r->rv_len];
	rels.left_neighbor_of_prev = (rels.prev - 1 + r->points) % r->points;
	rels.right_neighbor_of_prev = (rels.prev + 1) % r->points;
	
	//printf(BOLD_BLUE"here\n"RESET);
	if (rules[0] && (v == rels.prev))
		return (1);
	if (rules[1] && v == rels.left_neighbor_of_prev)
		return (1);
	if (rules[2] && v == rels.right_neighbor_of_prev)
		return (1);
	if (rules[3] && rels.one_before_prev == rels.prev)
	{
		if (v == rels.left_neighbor_of_prev)
			return (1);
		if (v == rels.right_neighbor_of_prev)
			return (1);
	}
	if (rules[4] && v == rels.one_before_prev)
		return (1);
	if (ft_r_2(v, r, &rels, rules) || ft_r_3(v, r, &rels, rules))
		return (1);
	if (ft_r_4(v, &rels, rules) || ft_r_5(rv, v, i, r, &rels, rules))
		return (1);
	return (0);
}
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