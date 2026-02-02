/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_mouse_super.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:08:10 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:12:53 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"

void	zoom_iters_mouse(int button, t_game *r)
{
	double rad_times_zoom;
	
	r->col_shift_x = ft_round((double)r->col_shift_x / r->zoom);
	r->col_shift_y = ft_round((double)r->col_shift_y / r->zoom);
	if (button == MOUSE_FORWARD)
		r->zoom *= 1.1;
	else if (button == MOUSE_BACK)
		r->zoom /= 1.1;
	rad_times_zoom = r->r * r->zoom;

	if (r->dist_ratio <= .5)
		r->iters = .5 * (8 + r->iters_change) * SQ(r->dist_ratio * rad_times_zoom) * r->area_factor;
	else if (r->dist_ratio < 1.0 && r->dist_ratio > .5)
		r->iters = .5 * (8 + r->iters_change) * SQ((1 - r->dist_ratio) * rad_times_zoom) * r->area_factor;
	else //(r->dist_ratio > 1)
		r->iters = .5 * (8 + r->iters_change) * SQ(rad_times_zoom * r->ratio_change * (r->dist_ratio - 1)) * r->area_factor;
	r->col_shift_x = ft_round((double)r->col_shift_x * r->zoom);
	r->col_shift_y = ft_round((double)r->col_shift_y * r->zoom);
}

//for center on one axis, move_axis = 0, and not other
static int	mouse_handler_2(int button, int x, int y, t_game *r)
{
	double cen_x;
	double cen_y;
	double old_zoom;
	
	if (button == MOUSE_FORWARD || button == MOUSE_BACK)
	{
		if (!r->mouse_zoom)
			zoom_iters_mouse(button, r);
		else
		{
			cen_x = r->width / 2.0;
			cen_y = r->height / 2.0;
			old_zoom = r->zoom;
			zoom_iters_mouse(button, r);			
			
			//For mouse zoom tran pnt to center, zoom, tran back
			//original form for clarity
			/* r->move_x = (cen_x - x) / old_zoom - (cen_x - x) / r->zoom + r->move_x;
			r->move_y = (y - cen_y) / old_zoom - (y - cen_y) / r->zoom + r->move_y; */

			//opti, pack action into x and y to be added
			double factor = (r->zoom - old_zoom) / (old_zoom * r->zoom);
			r->move_x += (cen_x - x) * factor;
			r->move_y += (y - cen_y) * factor;
		}
	}
	render(r);
	return (0);
}

int	ft_round(double val)
{
	return ((int)floor(val + .5));
}

//rotates img about center by the angle color wheel handled if on

void	rotate(t_game *r, double angle)
{
	/* double cen_x, cen_y, old_zoom;//try to make rotate about curr center
	old_zoom = r->zoom;
	cen_x = r->width / 2.0;
	cen_y = r->height / 2.0; */

	/* (*vertices)[i][0] = r->width / 2 + (r->move_x + factor * r->r * cos(final_angle)) * r->zoom;
		(*vertices)[i][1] = r->height / 2 + (factor * r->r * sin(final_angle) - r->move_y) * r->zoom; */
	/* r->move_x = (cen_x - r->move_x) / old_zoom - (cen_x - r->move_x) / r->zoom + r->move_x;
	r->move_y = (r->move_y - cen_y) / old_zoom - (r->move_y - cen_y) / r->zoom + r->move_y; */

		double ca = cos(angle);
		double sa = sin(angle);
		double tempx = r->col_shift_x;
		double tempy = r->col_shift_y;
		int shift;
		r->rotate += angle;
		r->rotate = fmod(r->rotate, 2 * M_PI);
		r->col_shift_x = ft_round((tempx * ca + tempy * sa));//for rounding(drift prevention).
		r->col_shift_y = ft_round((tempy * ca - tempx * sa));
		shift = (ft_round(angle * (RAD_TO_DEG)));
		r->color_rot += shift;
		r->color_rot %= 360;
}

double round_to_mag(double angle, double (*ft_r)(double val), double mag)
{
	return (ft_r(angle / mag) * mag);
}

void	nav_arrow_change(t_game *r, double mag, double *to_change, double (*ft_r)(double val), int sign)
{
	double curr = *to_change;
	double change = (round_to_mag(curr, ft_r, sign * mag) - curr);
	if (fabs(change) <= 1e-5)
		change = mag;
	*to_change += change;
	render(r);
}

void	rot_arrow_change(t_game *r, double (*ft_r)(double val), double sign)
{
	double mag = 30.0f / (double)r->sides;
	double curr = r->rotate * RAD_TO_DEG;
	double change = round_to_mag(curr, ft_r, mag) - curr;
	if (fabs(change) <= 1e-5)
		change = sign * mag;
	change *= DEG_TO_RAD;
	rotate(r, change);
	render(r);
}

void	zoom_arrow_change(t_game *r, double (*ft_r)(double val), double sign)
{
	double curr = r->zoom;
	double change = round_to_mag(curr, ft_r, .05) - curr;
	if (fabs(change) <= 1e-5)
		change = sign * .05;
	r->zoom += change;
	zoom_iters_mouse_move(r);
	render(r);
}
//handle events on the nav panel
//R = r->r
//A = r->r * cos(M_PI / r->sides);

void	nav_press(int x, int y, t_game *r, t_control *con)
{
	con->on_nav = true;
	//check buttons //top nav
	if (check_top_buttons(r, x, y, con))
		return ;
	//other buttons here
	double change_val = r->r * r->dist_ratio * .5;//fractional MAG VAL
	if ((x >= 121 && x < 141) && (y >= 164 && y < 184))
	{
	//	arrow_change(r, r->r * r->dist_ratio * .5, &r->move_x);//along rad
		nav_arrow_change(r, ft_round( change_val * cos(M_PI / r->sides)) , &r->move_x, ceil, 1);//along apothem
	}
	else if ((x >= 121 && x < 141) && (y >= 214 && y < 234))
	{
		nav_arrow_change(r, -ft_round( change_val * cos(M_PI / r->sides)), &r->move_x, floor, -1);//along apothem
	}
	else if ((x >= 259 && x < 279) && (y >= 164 && y < 184))
	{
		nav_arrow_change(r, change_val, &r->move_y, ceil, 1);//along radius
	}
	else if ((x >= 259 && x < 279) && (y >= 214 && y < 234))
	{
		nav_arrow_change(r, -change_val , &r->move_y, floor, -1);//along radius
		//printf("on arrow\n");
	}
	else if ((x >= 184 && x < 216) && (y >= 110 && y < 137))
	{
		set_home(r);
		render(r);	
	}
	else if ((x >= 190 && x < 210) && (y >= 285 && y < 305))//rot
		rot_arrow_change(r, ceil, 1);
	else if ((x >= 190 && x < 210) && (y >= 335 && y < 355))
		rot_arrow_change(r, floor, -1);
	else if ((x >= 190 && x < 210) && (y >= 416 && y < 436))
		zoom_arrow_change(r, ceil, 1);
	else if ((x >= 190 && x < 210) && (y >= 416 && y < 486))
		zoom_arrow_change(r, floor, -1);
	check_nav_knobs(x, y, r, con);
}

void	w_nav_arrow_change(t_game *r, int val, int *to_change)
{
	*to_change -= val;
	render(r);
}

void	w_rot_arrow_change(t_game *r, double (*ft_r)(double val), int sign)
{
	double curr = r->color_rot;
	int change = ft_round(round_to_mag(curr, ft_r, 5.0) - curr);
	if (change == 0)
		change = 5 * sign;
	r->color_rot += change;
	render(r);
}

void	set_home_w(t_game *r)
{
	r->col_shift_x = 0;
	r->col_shift_y = 0;
	r->color_rot = 0;
	render(r);
}

void	w_nav_press(int x, int y, t_game *r, t_control *con)
{
	//printf("here\n");

	con->on_w_nav = true;
	if (check_top_buttons(r, x, y, con))
		return ;
	double change_val = r->r * r->dist_ratio * .5 * r->zoom;
	if ((x >= 121 && x < 141) && (y >= 164 && y < 184))
	{
	//	arrow_change(r, r->r * r->dist_ratio * .5, &r->move_x);//along rad
		w_nav_arrow_change(r, ft_round( change_val * cos(M_PI / r->sides)), &r->col_shift_x);//along apothem
	}
	else if ((x >= 121 && x < 141) && (y >= 214 && y < 234))
	{
	//	arrow_change(r, -r->r * r->dist_ratio * .5, &r->move_x);
		w_nav_arrow_change(r, -ft_round(change_val * cos(M_PI / r->sides)), &r->col_shift_x);//along apothem
	}
	else if ((x >= 259 && x < 279) && (y >= 164 && y < 184))
	{
		w_nav_arrow_change(r, ft_round(change_val), &r->col_shift_y);//along rad
	}
	else if ((x >= 259 && x < 279) && (y >= 214 && y < 234))
	{
		w_nav_arrow_change(r, -ft_round(change_val), &r->col_shift_y);//along rad
	}
	else if ((x >= 184 && x < 216) && (y >= 110 && y < 137))
		set_home_w(r);
	else if ((x >= 190 && x < 210) && (y >= 285 && y < 305))//rot
		w_rot_arrow_change(r, ceil, 1);
	else if ((x >= 190 && x < 210) && (y >= 335 && y < 355))
		w_rot_arrow_change(r, floor, -1);
	else if ((x >= 190 && x < 210) && (y >= 416 && y < 436))
		zoom_arrow_change(r, ceil, 1);
	else if ((x >= 190 && x < 210) && (y >= 416 && y < 486))
		zoom_arrow_change(r, floor, -1);
	check_nav_knobs(x, y, r, con);
}

void	handle_left_click(int x, int y, t_game *r, t_control con)
{
	if ((con.nav_open || con.nav_w_open) && x < con.nav_w && y < con.nav_h)
	{
		if (con.nav_open)
			nav_press(x, y, r, r->con);
		else if (con.nav_w_open)
			w_nav_press(x, y, r, r->con);
	}
	else if (r->con_open && x < con.m_width && y < con.m_height)
	{
		if ((r->con->nav_open || r->con->nav_w_open))
		{
			r->con->nav_open = false;
			r->con->nav_w_open = false;
			r->con->on_nav = false;
			r->con->on_w_nav = false;
			set_controls(r);
		}
		r->on_con = true;
		con_press(x, y, r, r->con);
	}
	else//perform normal left click
	{
		if (!r->layer)
			rotate(r, M_PI / (r->sides * 6));
		else
		{//center image about the mouse click point
			r->move_x -= (x - r->width_orig * .5) / r->zoom;
			r->move_y -= (r->height_orig * .5 - y) / r->zoom;
		}
		render(r);
	}
}

//handles mouse events (clicks and wheel)

int	mouse_handler(int button, int x, int y, t_game *r)
{
	//printf("%d\n", button);
	if (r->supersample && button != 1)
	{
		x *= r->s_kernel;
		y *= r->s_kernel;
	}
	if (button == 1)
		handle_left_click(x, y, r, *r->con);
	else if (button == 3)
	{
		if (!r->layer)
			rotate(r, -M_PI / (r->sides * 6));
		else
		{//center color wheel on click
			if (!r->supersample)
			{
				r->col_shift_x = r->width_orig * .5 - x + r->move_x * r->zoom;
				r->col_shift_y = r->height_orig * .5 - y - r->move_y * r->zoom;
			}
			else
			{
				r->col_shift_x = (r->width * .5 - x + r->move_x * r->zoom);
				r->col_shift_y = (r->height * .5 - y - r->move_y * r->zoom);
			}
		}
		render(r);	
	}
	else
		mouse_handler_2(button, x, y, r);
	return (0);
}

void	set_supersampler_off(t_game *r)
{
	r->iters /= SQ(r->s_kernel);
	r->width = r->width_orig;
	r->height = r->height_orig;
	r->r = r->height * .4;
	r->max_distance /= r->s_kernel;
	r->start_maxd /= r->s_kernel;
	r->move_x /= r->s_kernel;
	r->move_y /= r->s_kernel;
	r->col_shift_x /= r->s_kernel;
	r->col_shift_y /= r->s_kernel;
}

void	set_supersampler_on(t_game *r)
{
	r->width *= r->s_kernel;
	r->height *= r->s_kernel;
	r->iters *= SQ(r->s_kernel);
	r->r = r->height * .4;
	r->max_distance *= r->s_kernel;
	r->start_maxd *= r->s_kernel;
	r->move_x *= r->s_kernel;
	r->move_y *= r->s_kernel;
	r->col_shift_x *= r->s_kernel;
	r->col_shift_y *= r->s_kernel;
}

int	supersample_handler(int keysym, t_game *r)
{
	if (keysym == SPACE)
	{
		if (r->supersample)
			set_supersampler_off(r);
		else
		{
			set_supersampler_on(r);
			ft_putstr_color("SUPERSAMPLE IN PROGRESS...\n", BOLD_BRIGHT_GREEN);
		}
		r->supersample = !r->supersample;
	}
	else if (keysym == PAD_9)
	{
		adjust_ratio(r, PHI);
		//adjust_ratio(r, PI_DIV_3);
	}
	else if (keysym == PAD_7)
	{
		adjust_ratio(r, INV_PHI);//use with sides on
//		adjust_ratio(r, PI_DIV_2);//not bad
		//adjust_ratio(r, 2.0 / 3.0);
	}
	return (0);
}
