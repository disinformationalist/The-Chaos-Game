#include "chaos.h"

static inline int ft_round1(double val)
{
	return (int)floor(val + .5);
}

//maps texture to fractal

unsigned int map_color(t_img *from, t_game *r, int x, int y)
{
	unsigned int	color;

	color = 0;
	if (!r->supersample)
		color = pixel_color_get(x, y, from);		
	else
		color = pixel_color_get(ft_round1(x / r->s_kernel), ft_round1(y / r->s_kernel), from);
	return (color);
}

//using mirrored tiles on super, doesnt look so good. 
/* src_x = (x_tile % 2 == 0) ? i : (r->width_orig  - 1 - i);
src_y = (y_tile % 2 == 0) ? j : (r->height_orig - 1 - j); */
