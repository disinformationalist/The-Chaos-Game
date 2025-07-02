/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:03:29 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 14:07:36 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

int	ft_putstr_color_fd(int fd, char *s, char *color)
{
	int			len;
	const char	*reset_col;

	len = 0;
	reset_col = "\033[0m";
	write(fd, color, ft_strlen(color));
	while (*s)
		len += write(fd, s++, 1);
	write(fd, reset_col, ft_strlen(reset_col));
	return (len);
}

int	close_screen(t_game *r)
{
	if (r->con)
		free_controls(r->mlx_connect, r->con);
	if (r->w_colors)
		free(r->w_colors);
	mlx_destroy_image(r->mlx_connect, r->img.img_ptr);
	mlx_destroy_window(r->mlx_connect, r->mlx_win);
	mlx_destroy_display(r->mlx_connect);
	free(r->mlx_connect);
	exit(EXIT_SUCCESS);
}

static int	key_handler_3(int keysym, t_game *r)
{
	if (keysym == XK_Down)
		r->move_y += (r->height / (15 * r->zoom));
	else if (keysym == XK_Left)
		r->move_x += (r->height / (15 * r->zoom));
	else if (keysym == XK_Right)
		r->move_x -= (r->height / (15 * r->zoom));
	else if (keysym == XK_Up)
		r->move_y -= (r->height / (15 * r->zoom));
	else if (keysym == PLUS) 
	{
		r->iters += r->dist_ratio * r->dist_ratio * (r->r * r->zoom) * (r->r * r->zoom) * sqrt(3);
		r->iters_change += 1;
	}
	else if (keysym == MINUS)
	{
		r->iters -= r->dist_ratio * r->dist_ratio * (r->r * r->zoom) * (r->r * r->zoom) * sqrt(3);
		if (r->iters <= 0)
			r->iters = 0;
		r->iters_change -= 1;
		if (r->iters_change < -4)
			r->iters_change = -4;
	}
	else
		key_handler_r(keysym, r);
	return (0);
}

static int	key_handler_3layer(int keysym, t_game *r)
{
	if (keysym == XK_Up)
		r->col_shift_y += (r->height / 8);
	else if (keysym == XK_Down)
		r->col_shift_y -= (r->height / 8);
	else if (keysym == XK_Left)
		r->col_shift_x += (r->height / 8);
	else if (keysym == XK_Right)
		r->col_shift_x -= (r->height / 8);
	else if (keysym == PLUS)//maybe adjust these for width
	{
		r->resize = true;
		r->win_change_x += 200;
		r->win_change_y += 200;
	}
	else if (keysym == MINUS)
	{
		r->resize = true;
		r->win_change_x -= 200;
		r->win_change_y -= 200;
		if (r->width_orig <= 200 || r->height_orig <= 200)
		{
			r->win_change_x = 0;
			r->win_change_y = 0;
		}
	}
		key_handler_rlayer(keysym, r);
	return (0);
}

void adjust_ratio(t_game *r, double new_ratio)
{
	if (r->dist_ratio == 2)
	{
		r->r = r->height / 2 - r->height / 10;
		r->iters = 4 * r->dist_ratio * r->dist_ratio * r->r * r->r * sqrt(3);
		r->zoom = 1.0;
		r->iters_change = 0;
		r->move_x = 0.0;
		r->move_y = 0.0;
		r->rotate = 0;
	}
	r->dist_ratio = new_ratio;
/* 	if (r->dist_ratio > 1)//not working yet ... almost
		r->zoom =  (r->dist_ratio - 1) / r->dist_ratio;//geometric series solution.. not quite right though.
		//r->zoom = 1 / (((1 - r->dist_ratio) / r->dist_ratio) * ((1 - r->dist_ratio) / r->dist_ratio) * sqrt(3) * 3 / 2);//geometric series solution
		//r->zoom = ((1 - r->dist_ratio) / r->dist_ratio) / ((1 - r->dist_ratio) * r->dist_ratio * r->dist_ratio);//geometric series solution
	else
		r->zoom = 1; */
	
	if (new_ratio > 1.0)
		r->iters = (4 + r->iters_change) * r->dist_ratio * r->dist_ratio * (r->r * r->zoom) * (r->r * r->zoom) * sqrt(3);
}

static int	key_handler_2(int keysym, t_game *r)
{
	if (keysym == PAD_8)
		adjust_ratio(r, r->dist_ratio + .05);
	else if (keysym == PAD_2)
		adjust_ratio(r, r->dist_ratio -.05);
	else if (keysym == PAD_4)
		adjust_ratio(r, r->dist_ratio -.005);
	else if (keysym == PAD_6)
		adjust_ratio(r, r->dist_ratio +.005);
	else if (keysym == PAD_1)
	{
		//r->zoom /= 3;
		adjust_ratio(r, 1.5);
	}
	else if (keysym == PAD_5)
		adjust_ratio(r, .5);
	else if (keysym == PAD_PLUS)
		r->sides += 1;
	else if (keysym == PAD_MINUS)
	{
		if (r->sides > 1)
			r->sides -= 1;
	}
	if (keysym == ALT)
		r->rotate = 0;
	else if (keysym == F2)
	{
		r->mode++;
		if (r->mode > 4)//function mode > num_modes
			r->mode = 1;
		if (r->mode == 2)
			r->function_id = 1;
	}
	else if (keysym == LESS_THAN)
	{
		r->color_depth++;
		if (r->color_depth > 3)
			r->color_depth = 0;
	}
	else if (keysym == QUES)
		r->jump_to_center = !r->jump_to_center;
	else if (keysym == APOST)
		r->jump_to_sides = !r->jump_to_sides;
	else
		key_handler_3(keysym, r);
	intermed(r);
	return (0);
}

static int	key_handler_2layer(int keysym, t_game *r)//need ot make new iters adjustments??
{
	if (keysym == PAD_8)
		r->ratio_start += .05;
	else if (keysym == PAD_2)
		r->ratio_start -= .05;
	else if (keysym == PAD_4)
		r->ratio_start -= .005;
	else if (keysym == PAD_6)
		r->ratio_start += .005;
	else if (keysym == PAD_1)
	{
		r->ratio_start = 1.5;
	}
	else if (keysym == PAD_5)
		r->ratio_start = .5;
	else if (keysym == PAD_PLUS)
	{
		if (r->supersample)
		{
			set_supersampler_off(r);
			r->s_kernel += 2;
			if (r->s_kernel > 31)
				r->s_kernel = 31;
			set_supersampler_on(r);
		}
		else
		{
			r->s_kernel += 2;
			if (r->s_kernel > 31)
				r->s_kernel = 31;
		}
		printf("Supersample Level: %d\n", r->s_kernel);
	}
	else if (keysym == PAD_MINUS)
	{
		if (r->supersample)
		{
			set_supersampler_off(r);
			r->s_kernel -= 2;
			if (r->s_kernel < 3)
				r->s_kernel = 3;
			set_supersampler_on(r);
		}
		else
		{
			r->s_kernel -= 2;
			if (r->s_kernel < 3)
				r->s_kernel = 3;
		}
		printf("Supersample Level: %d\n", r->s_kernel);
	}
	else if (keysym == ALT)//curved
		r->curved = !r->curved;
	else if (keysym == F2)//avail
	{
		r->mode++;
		if (r->mode > 4)//function mode > num_modes
			r->mode = 1;
		if (r->mode == 2)
		r->function_id = 1;
	}
	else if (keysym == LESS_THAN)
	{
		r->color_op++;
		if (r->color_op > 1)//
			r->color_op = 0;
	}
	else if (keysym == QUES)//ava
		r->jump_to_center = !r->jump_to_center;
	else if (keysym == APOST)//ava
		r->jump_to_sides = !r->jump_to_sides;
	else
		key_handler_3layer(keysym, r);
	intermed(r);
	return (0);
}

void	print_guide(void)
{
	write(1, "\n", 1);
	ft_putstr_color_fd(1, "                  ╔╦═════════════╦╗\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "                  ╠╣  KEY GUIDE  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╔╦════════════════╩╩═════════════╩╩════════════════╦╗\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣", BRIGHT_BLUE);
	ft_putstr_color_fd(1, " [F1]       => Print current settings            ", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [F2]       => Print this guide                  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [ESC]      => Exit and close window             ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [F3]       => Export image to png               ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ ----------------- MOUSE HOOKS ------------------╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Wheel UP] => Zoom in                           ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Wheel DN] => Zoom out                          ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [L CLICK]  => Rotate image left                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [R CLICK]  => Rotate image right                ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣-------------------------------------------------╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [R  CNTRL] => Mouse Zoom on/off (starts off)    ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [L  CNTRL] => Reset to start(except color)      ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ (Mouse Zoom zooms about mouse arrow position)   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [UP ⬆]     => Move view upward                  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Down ⬇]   => Move view downward                ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Left ⬅]   => Move view left                    ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [right ➡]  => Move view right                   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [+]        => increase iterations               ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [-]        => decrease iterations               ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣", BRIGHT_BLUE);
	ft_putstr_color_fd(1, " [SPACE]      => Supersample on/off(antialiasing)", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [1] - [0]  => Toggle rules 1 - 10  on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Q] - [P]  => Toggle rules 11 - 20 on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [A] - [H]  => Toggle rules 21 - 26 on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [J],  [K]  => Toggle disinfo 1, 2  on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Ö]        => Toggle rule 27       on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Ä]        => Toggle rule 28       on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Ü]        => Toggle rule 29       on/off       ╠╣\n", BRIGHT_BLUE);

	ft_putstr_color_fd(1, "╠╣ [?]        => Jumps to center      on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [´]        => Jumps to sides       on/off       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 0]   => reset position/zoom/iters         ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD +]   => increase sides by 1               ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD -]   => decrease sides by 1               ╠╣\n", BRIGHT_BLUE);

	ft_putstr_color_fd(1, "╠╣ [NPAD /]   => zoom div   9                      ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD *]   => zoom times 9                      ╠╣\n", BRIGHT_BLUE);

	ft_putstr_color_fd(1, "╠╣ [NPAD 8]   => increase dist ratio .05           ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 2]   => decrease dist ratio .05           ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 6]   => increase dist ratio .005          ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 4]   => increase dist ratio .005          ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 5]   => dist ratio to .5(start)           ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 3]   => dist ratio to 2(clouds)           ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 1]   => dist ratio to 1.5(invert)         ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 9]   => dist ratio to golden ratio(~1.618)╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 7]   => dist ratio to .667 (2 / 3)        ╠╣\n", BRIGHT_BLUE);



//		TO FINISH GUIDE BELOW HERE..

	//ft_putstr_color_fd(1, "╠╣ [ALT]      => toggle on off auto iter adjust    ╠╣\n", BRIGHT_BLUE);

//	ft_putstr_color_fd(1, "╠╣ [NUM 1]    => Switch to/from color wheel colors ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ --------------- Color Controls ---------------- ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                          //TODO--------                       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Y] - [M]  => change to diff color sets         ╠╣\n", BRIGHT_BLUE);

	ft_putstr_color_fd(1, "╠╣ [A D,W S]  => Move center of color wheel in x, y╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Q]        => recenter color wheel              ╠╣\n", BRIGHT_BLUE);
	
	ft_putstr_color_fd(1, "╠╣ [<]   =>        ╠╣\n", BRIGHT_BLUE);

	ft_putstr_color_fd(1, "╠╣ [,]   =>        ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [.]   =>        ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣-------------------------------------------------╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [^]  => Switch to/from second key layer         ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ ( some keys will now do something different)    ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣---------- Second layer key functions -----------╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD +]   => increase supersample level by 2   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD -]   => decrease supersample level by 2   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [+]   => increase iterations level by 500       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [-]   => decrease iterations level by 500       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╚╩═════════════════════════════════════════════════╩╝\n", BRIGHT_BLUE);
}
//╔ ╗ ╚ ╝ ═ ║, ╦ ╩ ╠ ╣, ➡ ⬅ ⬆ ⬇

int	key_handler(int keysym, t_game *r)//place things not to rerender upon keypress here
{
	//printf("%d\n", keysym);
	if (keysym == XK_Escape)
		close_screen(r);
	else if (keysym == F4)
		controls(r);
	else if (keysym == UP_CARET)
		r->layer = !r->layer;
	else if (keysym == F1)
		print_board(r);
	/* else if (keysym == )
		print_guide(); */
	else if (keysym == RGHT_STRG && !r->layer)
		r->mouse_zoom = !r->mouse_zoom;
	else if (keysym == F3)
	{
		char		*name;
		png_text	*text;
		name = get_nxt_name("chaos_");
		if (!name)
			close_screen(r);
		text = build_chaos_text(r);
		if (export_png(name, &r->img, r->width_orig, r->height_orig, text) == -1)
		{
			free(name);
			close_screen(r);
		}
		free(name);
		ft_putstr_color("EXPORT COMPLETE\n", BOLD_BRIGHT_MAGENTA);
	}
	else if (r->layer)
		key_handler_2layer(keysym, r);
	else
		key_handler_2(keysym, r);
	return (0);
}
