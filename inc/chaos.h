/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chaos.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:13:17 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 17:29:42 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAOS_H
#define CHAOS_H

# include <unistd.h>
# include "../minilibx-linux/mlx.h"
# include "ansi_colors.h"///
# include "keyboard.h"///-----------shouldnt need these two but its not working.
# include "xoro128.h"
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <time.h>
# include <sys/time.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include <stdbool.h>
# include "../image_processing/inc/image_processing.h"
# include <stdarg.h>
# include <string.h>
# include <stdint.h>
# include "color_sets.h"

# define PHI 1.61803398875

#define RULE(mask, n) (((mask) >> (n)) & 1)

static inline int wrap_index(int i, int max) {
	return (i + max) % max;
}


typedef struct s_color_vars
{
	int	num_colors;
	int	color_index_low;
	int	color_index_high;
	int	color_low;
	int	color_high;
	int	color;
}	t_color_vars;

typedef struct s_relations
{
	int prev;
	int one_before_prev;
	int left_neighbor_of_prev;
	int right_neighbor_of_prev;
	int second_left_neigbor_of_prev;
	int second_right_neigbor_of_prev;
	int third_left_neigbor_of_prev;
	int third_right_neigbor_of_prev;
	int fourth_left_neigbor_of_prev;
	int fourth_right_neigbor_of_prev;
	int left_neigbor_of_one_before_prev;
	int right_neigbor_of_one_before_prev;
	int second_left_neigbor_of_one_before_prev;
	int second_right_neigbor_of_one_before_prev;
	int third_left_neigbor_of_one_before_prev;
	int third_right_neigbor_of_one_before_prev;
	int fourth_left_neigbor_of_one_before_prev;
	int fourth_right_neigbor_of_one_before_prev;

}	t_relations;

typedef struct s_data
{
	int		points;
	int		points_min_one;
	bool	jump_to_cen;
	double	dist_ratio;
	double	ratio_start;
	int		width;
	int		height;
	long	iters;
	int		center[2];

} t_data;

typedef struct s_game
{
	uint32_t	rules_mask;
	Xoro128		rng;
	t_data		*data;
//---included in god data----
	bool		god;
	double		move_x;
	double		move_y;
	double		rotate;
	double		zoom;
	double		max_distance;
	double		dist_ratio;///
	double		ratio_start;///
	bool		rules[30];
	bool		disinfo_1;
	bool		disinfo_2;
	bool		jump_to_sides;
	bool		jump_to_center;////
	bool		mouse_zoom;
	bool		supersample;
	bool		layer;
	bool		curved;
	long		r;
	long		iters;////
	int			sides;///////
	int			points;/////
	int			center[2];/////
	int			s_kernel;
	int			width;/////
	int			height;/////
	int			mode;
	int			function_id;
	
	int			color_depth;
	int			color_op;
	t_colors	colors;

	int			color_rot;
	int 		col_shift_y;
	int 		col_shift_x;
	int			iters_change;

//----------not in god----------
	bool		resize;
	int			win_change_x;
	int			win_change_y;
	unsigned int **pixels_xl;

	int			*w_colors;
	int			rv[3];// vary rv
	int			rv_len;
	long		i;
	int			num_rules;
	t_img		img;
	void		*mlx_connect;
	void		*mlx_win;

	int			width_orig;
	int			height_orig;
}		t_game;

void	print_time(long start, long end, char *msg);

/****CHAOS UTILS****/

void		intermed(t_game *r);
void		information(t_game *r, int v, long i);
int			r_loop(t_game *r, Xoro128 *rng);
int			ft_r(int rv[], int v, long i, t_game *r);
void		events_init(t_game *r);
void		info_init(t_game *r);
void		r_init(t_game *r);
void		game_init(t_game *r);
void		adjust_ratio(t_game *r, double new_ratio);

void		polygon(int ***vertices, t_game *r);
void		free_poly(int ***vertices, int i);

/*****GAMES*****/
void		chaos_game(t_game *r, int **vertices, double x, double y);
void		chaos_game_curved(t_game *r, int **vertices, double x, double y);
void		chaos_game_apply_function(t_game *r, int **vertices, double x, double y);
//void		chaos_game_curved_pro(t_game *r, int **vertices, double x, double y);

/***COLORS***/
void		color_option(t_game *r, int v, double x, double y, int **vertices);
int			calc_color_4(double distance, double max_distance, t_colors *colors);
void		put_pixel(int x, int y, t_game *r, unsigned int color);

double		get_distance(double x1, double y1, double x2, double y2);

/***HOOKS***/
int			key_handler(int keysym, t_game *r);
int			key_handler_r(int keysym, t_game *r);
int			key_handler_other(int keysym, t_game *r);
int			mouse_handler(int button, int x, int y, t_game *r);
int			supersample_handler(int keysym, t_game *r);

void		zoom_iters_key(int keysym, t_game *r);
int			close_screen(t_game *r);
void		set_supersampler_on(t_game *r);
void		set_supersampler_off(t_game *r);

int			key_handler_rlayer(int keysym, t_game *r);
int			key_handler_otherlayer(int keysym, t_game *r);

void		resize_window(t_game *r, int new_width, int new_height);

	
/***PRINT BOARD AND SET PNG DATA***/
void		print_board(t_game *r);
void		put_ratio(t_game *r);
void		put_vals(t_game *r);
void		put_bool(bool x);
char		*mode_str(t_game *r);
void		ft_putstr_c(char *s);
void		ft_putstr_color(char *s, char *color);
int 		get_numlen(long n);
void		write_num_color(int num, char *color);
png_text	*build_chaos_text(t_game *r);
char		*serialize_game_data(t_game *game);
char		*read_png_text_metadata(const char *filename);
void		deserialize_game_data(t_game *r, const char *data);

/***CLEAN***/
void		clear_all(t_game *r);
void		free_poly(int ***vertices, int i);

#endif
