/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:13:28 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:13:31 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

void	ft_putstr_c(char *s)
{
	char	*color;

	color = BRIGHT_CYAN;//"\033[32m";
	write(1, color, ft_strlen(color));
	while (*s)
		write(1, s++, 1);
	write(1, "\033[0m", ft_strlen("\033[0m"));
}

void	put_bool(bool x)
{
	if (x)
		write(1, "\033[32mON\033[0m ", ft_strlen("\033[32mON\033[0m "));
	else
		write(1, "\033[31mOFF\033[0m", ft_strlen("\033[31mOFF\033[0m"));
}

void	write_num_color(int num, char *color)
{
	if (num / 10)
		write_num_color(num / 10, color);
	write(1, color, 7);
	write(1, &("0123456789"[num % 10]), 1);
	write(1, "\033[0m", ft_strlen("\033[0m"));
}

int get_numlen(long n)
{
	int len;
	
	len = 1;
	if (n < 0)
	{
		len = 2;
		n = -n;
	}
	while (n / 10)
	{
		len++;
		n /= 10;
	}
	return (len);
}


static inline void	put_spaces(double param)
{
	int i;

	i = -1;
	while (++i < 11 - 3 - get_numlen(param))
		write(1, " ", 1);
}

void	put_vals(t_game *r)
{
	int		i;

	ft_putstr_c(" ║   ITERS: ");
	write_num_color(r->iters, BOLD_MAGENTA);
	i = -1;
	while (++i < 12 - get_numlen(r->iters))
		write(1, " ", 1);
	ft_putstr_c("  ZOOM: ");
	printf(BOLD_MAGENTA"%.3f"RESET, r->zoom);
	fflush(stdout);
	put_spaces(r->zoom);
	ft_putstr_c("║\n");
	ft_putstr_c(" ║  MOVE_X: ");
	printf(BOLD_MAGENTA"%.3f"RESET, r->move_x);
	fflush(stdout);
	put_spaces(r->move_x);
	ft_putstr_c("MOVE_Y: ");
	printf(BOLD_MAGENTA"%.3f"RESET, r->move_y);
	fflush(stdout);
	put_spaces(r->move_y);
	ft_putstr_c("║\n");
}

void	put_ratio(t_game *r)
{
	char	*num_color;
	int		i;
	
	num_color = BOLD_MAGENTA;
	
	ft_putstr_c(" ║  RATIO_START: ");
	printf(BOLD_MAGENTA"%.3f"RESET, r->ratio_start);
	fflush(stdout);
	write(1, " ", 1);
	if (r->ratio_start > 0)
		write(1, " ", 1);
	ft_putstr_c(" SUPER: ");
	write_num_color(r->s_kernel, num_color);
	i = -1;
	while (++i < 12 - get_numlen(r->s_kernel))
		write(1, " ", 1);
	ft_putstr_c("║\n"); 
	ft_putstr_c(" ║   DIST_RATIO: ");
	printf(BOLD_MAGENTA"%.3f"RESET, r->dist_ratio);
	fflush(stdout);
	write(1, " ", 1);
	if (r->dist_ratio > 0)
		write(1, " ", 1);
	ft_putstr_c(" SIDES: ");
	write_num_color(r->sides, num_color);
	i = -1;
	while (++i < 12 - get_numlen(r->sides))
		write(1, " ", 1);
	ft_putstr_c("║\n");
	ft_putstr_c(" ║        LAYER: ");
	write_num_color(r->layer, num_color);
	ft_putstr_c("                          ║\n");
}
