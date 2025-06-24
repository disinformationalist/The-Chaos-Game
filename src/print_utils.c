/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:14:55 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:14:58 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

static void	put_line(int first_rule, int next_rule, char *s1, char *s2)
{
	ft_putstr_c(s1);
	put_bool(first_rule);
	ft_putstr_c(s2);
	put_bool(next_rule);
	ft_putstr_c("      ║\n");
}

static void	print_title(void)
{
	ft_putstr_c(" ╔══════════════════════════════════════════╗\n");
	ft_putstr_c(" ║       ");
	ft_putstr_color( " ✰ ✰ ✰ ", "\033[29m");
	ft_putstr_color( "THE CHAOS GAME", BOLD_BLUE);
	ft_putstr_color( " ✰ ✰ ✰ ", "\033[29m");
	ft_putstr_c("       ║\n");
	ft_putstr_c(" ╚══════════════════════════════════════════╝\n");
}

char	*mode_str(t_game *r)///ADD TO THIS AS MODE TYPES ARE SET
{
	char	*mode;
	
	if (r->mode == 1)
		mode = "NORMAL";
	else if (r->curved)
		mode = "CURVED";
	else if (r->mode == 2)
		mode = "FUNCTION";
	else if (r->mode == 3)
		mode = "ALT FT";
	else if (r->mode == 4)
		mode = "ALT FT 2";
	else
		mode = "OTHER";
	return (mode);
}


void	put_type(t_game *r)
{
	char	*mode;
	int		i;
	int		len;
	
	ft_putstr_c(" ║    MODE: ");
	mode = mode_str(r);
	ft_putstr_color(mode, BLUE);
	i = -1;
	len = 10 - ft_strlen(mode);
	while (++i < len)
		write(1, " ", 1);
	ft_putstr_c("FUNCTION: ");
	if (r->mode == 1 || r->curved)
		ft_putstr_color("NONE        ", BLUE);
	else
	{
		printf(BLUE"%d"RESET, r->function_id);
		fflush(stdout);

		i = -1;
		while (++i < 12 - get_numlen(r->function_id))
			write(1, " ", 1);
	}
	ft_putstr_c("║\n");
}


void	print_board(t_game *r)
{
	int	i;
	char *num; 
	char *num_2;
	char *rule_s1;
	char *rule_s2;
	
	print_title();
	ft_putstr_c("            ╔════════════════════╗           \n");
	ft_putstr_c(" ╔══════════╣ RULES AND SETTINGS ╠══════════╗\n");
	ft_putstr_c(" ║          ╚════════════════════╝          ║\n");
	i = -1;
	while (++i < 15)
	{
		num = ft_itoa(i + 1);
		num_2 = ft_itoa(i + 16);
		if (i < 9)
			rule_s1 = ft_strjoin_n(3," ║      RULE_", num, ": ");
		else
			rule_s1 = ft_strjoin_n(3," ║     RULE_", num, ": ");
		rule_s2 = ft_strjoin_n(3,"       RULE_", num_2, ": ");
		put_line(r->rules[i], r->rules[i + 15], rule_s1, rule_s2);
		free(num);
		free(num_2);
		free(rule_s1);
		free(rule_s2);
	}
	put_line(r->disinfo_1, r->disinfo_2, " ║   DISINFO_1: ", "     DISINFO_2: ");
	put_line(r->jump_to_center, r->jump_to_sides, " ║   TO_CENTER: ", "      TO_SIDES: ");
	put_type(r);
	put_vals(r);
	put_ratio(r);
	ft_putstr_c(" ║           BROUGHT TO YOU BY:             ║\n");
	ft_putstr_c(" ║          ");
	ft_putstr_color("DISINFORMATIONALIST", BOLD_BLUE);
	ft_putstr_c("             ║\n");
	ft_putstr_c(" ╚══════════════════════════════════════════╝\n");
}
