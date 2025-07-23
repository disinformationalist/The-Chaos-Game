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

	if (button == MOUSE_FORWARD)
		r->zoom *= 1.1;
	else if (button == MOUSE_BACK)
		r->zoom /= 1.1;
	rad_times_zoom = r->r * r->zoom;
	r->iters = (4 + r->iters_change) * r->dist_ratio * r->dist_ratio * rad_times_zoom * rad_times_zoom * sqrt(3);
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
	intermed(r);
	return (0);
}

void	handle_left_click(int x, int y, t_game *r, t_control con)
{
	if (r->con_open && x < con.m_width && y < con.m_height)
		con_press(x, y, r, r->con);
	else
	{
		//perform normal left click
		if (!r->layer)
			r->rotate -= M_PI / ((r->sides * 6) * r->zoom);
		else
			r->move_x = 0;
		intermed(r);
	}
}

int	mouse_handler(int button, int x, int y, t_game *r)
{
	//printf("%d\n", button);
	if (r->supersample && button != 1)
	{
		x *= r->s_kernel;
		y *= r->s_kernel;
	}
	if (button == 1)//
		handle_left_click(x, y, r, *r->con);//
	else if (button == 3)//
	{
		if (!r->layer)
			r->rotate += M_PI / ((r->sides * 6) * r->zoom);
		else
			r->move_y = 0;
		intermed(r);	
	}
	else
		mouse_handler_2(button, x, y, r);
	return (0);
}

void	set_supersampler_off(t_game *r)
{
	r->iters /= (r->s_kernel * r->s_kernel);
	r->width = r->width_orig;
	r->height = r->height_orig;
	
	r->r = r->height / 2 - r->height / 10;
	
	r->max_distance /= r->s_kernel;
	r->start_maxd /= r->s_kernel;
	r->move_x /= r->s_kernel;
	r->move_y /= r->s_kernel;
}

void	set_supersampler_on(t_game *r)
{
	r->width *= r->s_kernel;
	r->height *= r->s_kernel;
	r->iters *= r->s_kernel * r->s_kernel;
	
	r->r = r->height / 2 - r->height / 10;
	
	r->max_distance *= r->s_kernel;
	r->start_maxd *= r->s_kernel;
	r->move_x *= r->s_kernel;
	r->move_y *= r->s_kernel;
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
	}
	else if (keysym == PAD_7)
		adjust_ratio(r, 2.0 / 3.0);
	return (0);
}
