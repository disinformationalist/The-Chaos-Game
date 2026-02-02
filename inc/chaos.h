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
# define TWO_PI 6.283185
# define INV_E 0.36787944

# define ASPECT 1.7778
# define DEFAULT_BUMP 30
# define PI_HALVES 1.5708
# define FIVE_PI_SIXTHS 2.618
# define TWO_PI 6.283185
# define FOUR_PI_THIRDS 4.1888
# define TWO_PI_THIRDS 2.0944
# define PI_SIXTHS 0.5236
# define PI_FIFTHS 0.628319
# define PI_12THS 0.2618
# define THREE_OVER_2PI .477465
# define RAD_TO_DEG 57.29578
# define DEG_TO_RAD 0.0174533
# define ITER_FACTOR 0.00001866063

/* Interesting constants to try strictly in (0.5, φ) */

# define INV_PHI                 0.6180339887498948  // 1/phi (golden ratio reciprocal)
# define EULER_GAMMA             0.5772156649015329  // Euler–Mascheroni constant γ
# define LAMBERT_W1              0.5671432904097838  // Omega constant W(1), solves W*e^W = 1
# define LN_2                    0.6931471805599453  // natural log of 2
# define SQRT2_DIV_2             0.7071067811865476  // √2 / 2 = cos(45°)
# define DOTTIE                  0.7390851332151607  // fixed point of cos(x) in radians
# define GOLOMB_DICKMAN_LAMBDA   0.6243299885435509  // Golomb–Dickman constant λ
# define TWIN_PRIME_C2           0.6601618158468696  // twin prime constant C2
# define LAPLACE_LIMIT           0.6627434193491816  // Laplace limit for Kepler series
# define CAHEN                   0.6434105462883380  // Cahen’s constant (Sylvester reciprocals)
# define SQRT3_DIV_2             0.8660254037844386  // √3 / 2 = sin(60°)
# define E_DIV_PI                0.8652559794322651  // e / π
# define CATALAN_G               0.9159655941772190  // Catalan’s constant G
# define LANDau_RAMANUJAN        0.7642236535892206  // Landau–Ramanujan constant
# define AGM_GAUSS               0.8346268416740731  // Gauss’s AGM constant
# define PI_DIV_3                1.0471975511965976  // π / 3 (60°)
# define SQRT5_DIV_2             1.1180339887498950  // √5 / 2
# define VISWANATH               1.1319882487943000  // Viswanath’s constant (random Fibonacci growth)
# define LEVY_CONST              1.1865691104156252  // Lévy constant π² / (12 ln 2)
# define APERY_ZETA3             1.2020569031595942  // Apéry’s constant ζ(3)
# define GLAISHER_A              1.2824271291006226  // Glaisher–Kinkelin constant A
# define CONWAY_CONST            1.3035772690342960  // Conway’s constant (look-and-say growth)
# define MILLS_CONST             1.3063778838630806  // Mills’ constant (assuming RH)
# define PLASTIC                 1.3247179572447458  // plastic constant ρ, real root of x³ = x + 1
# define RAMANUJAN_SOLDNER       1.4513692348833810  // Ramanujan–Soldner constant μ (li(μ) = 0)
# define SQRT2                   1.4142135623730951  // √2
# define PI_DIV_2                1.5707963267948966  // π / 2 (90°)
# define ERDOS_BORWEIN           1.6066951524152918  // Erdős–Borwein constant

#define RULE(mask, n) (((mask) >> (n)) & 1)

static inline int wrap_index(int i, int max) 
{
	return (i + max) % max;
}

#define MAX_R 30

typedef	struct s_knob
{
	t_img *img;

	int		cx;
	int		cy;
	int		posx;
	int		posy;

	double	angle;
	int 	w;
	int 	h;

} t_knob;

typedef	struct s_knobs
{
	t_knob		posx;
	t_knob		posy;
	t_knob		rot;
	t_knob		zoom;
	t_knob		w_zoom;
	t_knob		w_posx;
	t_knob		w_posy;
	t_knob		w_rot;
	t_knob		iter;
}	t_knobs;

typedef struct s_control
{
	t_img		*color_con;
	t_img		*nav;
	t_img		*w_nav;
	t_img		*r;
	t_img		*g;
	t_img		*b;
	t_img		*s;
	t_knobs		knobs;

	int			start_x;
	int			start_y;
	int			iters;

	int 		nav_w;
	int 		nav_h;

	bool		nav_open;
	bool		nav_w_open;

	bool		on_nav;
	bool		on_w_nav;
	
	int			m_height;//menu
	int			m_width;

	int			k_width;//knobs
	int			k_height;

	int 		col_num;//4 cols

	bool		dragging;
	int			knob;

	double		max_d;
	double		vert_d;//god?
	double		base_d;

	double		old_zoom;
	
	//not used yet, button effect later
	bool		pressed;
	int			press_xs;
	int			press_ys;
	int			press_width;
	int			press_height;

} t_control;

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

typedef bool (*rule_ft)(int v, const t_relations *rels);

typedef struct s_game
{
	Xoro128		rng;
	t_data		*data;
	t_control 	*con;
	bool		con_open;
	bool		on_con;
	t_4color	curr_col;
	double		start_maxd;
	double		ratio_change;
	int			**vertices2;
	
	rule_ft 	active[MAX_R + 1];
	uint32_t	rules_mask;//rules mask

	bool		ghost;//ghost center vert for color
	int			prev;//prev vert ind
	int			obp;//one before prev
	int			tbp;//two before
	int			thbp;//3 before

	double		area_factor;


//---included in god data----
	bool		god;
	double		move_x;
	double		move_y;
	double		rotate;
	double		zoom;
	double		max_distance;
	double		dist_ratio;
	double		ratio_start;
	bool		rules[30];
	bool		disinfo_1;
	bool		disinfo_2;
	bool		jump_to_sides;
	bool		jump_to_center;
	bool		mouse_zoom;
	bool		supersample;
	bool		layer;
	bool		curved;
	long		r;
	long		iters;
	int			sides;
	int			points;
	int			center[2];
	int			s_kernel;
	int			width;
	int			height;
	int			mode;
	int			function_id;
	
	int			color_depth;
	int			color_op;
	t_colors	colors;

	int			color_rot;
	int 		col_shift_y;//from int
	int 		col_shift_x;
	int			iters_change;

	double		vert_dist;
//--------------------------------
	int			jump_to_center_col;//add to god?
	bool		jump_to_sides_col;//add to god?
	int			ghost2;
	t_img 		*texture;//make double circle list later //addgod
	int			height_tex;//tex struct
	int			width_tex;

//----------not in god----------
	bool		resize;
	double			win_change_x;
	double			win_change_y;
	unsigned int **pixels_xl;

	int			*w_colors;
	int			rv[3];// vary rv
	int			rv_len;
	long		i;
	int			num_rules;
	t_img		img;
	t_img		cmyk;

	void		*mlx_connect;
	void		*mlx_win;

	int			width_orig;
	int			height_orig;
}		t_game;

typedef struct 
{
    int x;
    int y;
    int w;
    int h;
}	Rect;

/****CHAOS UTILS****/
void			render(t_game *r);
void			information(t_game *r, int v, long i);
int				r_loop(t_game *r, Xoro128 *rng);
int				ft_r(int rv[], int v, long i, t_game *r);
void			events_init(t_game *r);
void			info_init(t_game *r);
void			r_init(t_game *r);
void			game_init(t_game *r);
void			adjust_ratio(t_game *r, double new_ratio);
int				set_and_check(int rv[], int v, long i, t_game *r);
void			polygon(int ***vertices, t_game *r);
void			free_poly(int ***vertices, int i);
int				ft_round(double val);

/*****GAMES*****/
void			chaos_game(t_game *r, int **vertices, double x, double y);
void			chaos_game_curved(t_game *r, int **vertices, double x, double y);
void			chaos_game_apply_function(t_game *r, int **vertices, double x, double y);

/***COLORS***/
void			color_option(t_game *r, int v, double x, double y, int **vertices);
double			get_distance(double x1, double y1, double x2, double y2);
unsigned int	map_color(t_img *from, t_game *r, int x, int y);

/***HOOKS***/
int				key_handler(int keysym, t_game *r);
int				key_handler_r(int keysym, t_game *r);
int				key_handler_other(int keysym, t_game *r);
int				mouse_handler(int button, int x, int y, t_game *r);
int				supersample_handler(int keysym, t_game *r);

void			zoom_iters_key(int keysym, t_game *r);
int				close_screen(t_game *r);
void			set_supersampler_on(t_game *r);
void			set_supersampler_off(t_game *r);

int				key_handler_rlayer(int keysym, t_game *r);
int				key_handler_otherlayer(int keysym, t_game *r);

void			resize_window(t_game *r, int new_width, int new_height);
void			reset_iters(t_game *r);
void			rotate(t_game *r, double angle);
void			zoom_iters_mouse(int button, t_game *r);
void			zoom_iters_mouse_move(t_game *r);
void			set_home(t_game *r);



	
/***PRINT BOARD AND SET PNG DATA***/
void			print_board(t_game *r);
void			put_ratio(t_game *r);
void			put_vals(t_game *r);
void			put_bool(bool x);
char			*mode_str(t_game *r);
void			ft_putstr_c(char *s);
void			ft_putstr_color(char *s, char *color);
int 			get_numlen(long n);
void			write_num_color(int num, char *color);
png_text		*build_chaos_text(t_game *r);
char			*serialize_game_data(t_game *game);
char			*read_png_text_metadata(const char *filename);
void			deserialize_game_data(t_game *r, const char *data);

/***GUI CONTROLS***/
void			destroy_controls(void *con, t_control *controls);
t_control		*make_controls(void *m_con);
void			set_controls(t_game *r);
void			controls(t_game *r);
int				con_press(int x, int y, t_game *game, t_control *con);
int				mouse_move(int x, int y, t_game *game);
int				mouse_release(int button, int x, int y, t_game *game);
unsigned int	pixel_color_get3(int x, int y, t_img *img);
void			set_color_vals(void *mlx_con, void *mlx_win, t_game *game);

void			set_nav(t_game *r);
void			set_w_nav(t_game *r);
void			set_nav_vals(void *mlx_con, void *mlx_win, t_game *game, int nav);
void			check_nav_knobs(int x, int y, t_game *r, t_control *con);
void			set_nav_knobs(t_game *r, t_control *con);
int				check_top_buttons(t_game *r, int x, int y, t_control *con);

void			set_color_con(t_game *r, t_control con);


//void			reset_track(t_img *img, t_control control, int move_y);

/***OTHER***/
//void		build_rules_active(const bool *rules, rule_ft active[MAX_R + 1]);
void			print_time(long start, long end, char *msg);
void			convert_colors_to_cmyk_safe(t_colors *colors);
void			cmyk_softproof_image(t_game *r, t_img *src, t_img *dst);

/***CLEAN***/
void			clear_all(t_game *r);
void			free_poly(int ***vertices, int i);

#endif
