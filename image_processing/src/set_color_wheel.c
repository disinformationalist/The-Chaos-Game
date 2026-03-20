#include "image_processing.h"

void	wheel_helper(t_wheel *co)
{
	if (co->hue >= 0 && co->hue < 60)
	{
		co->r_ = co->c;
		co->g_ = co->x;
		co->b_ = 0;
	}
	if (co->hue >= 60 && co->hue < 120)
	{
		co->r_ = co->x;
		co->g_ = co->c;
		co->b_ = 0;
	}
	if (co->hue >= 120 && co->hue < 180)
	{
		co->r_ = 0;
		co->g_ = co->c;
		co->b_ = co->x;
	}
}

void	wheel_helper_2(t_wheel *co)
{
	if (co->hue >= 180 && co->hue < 240)
	{
		co->r_ = 0;
		co->g_ = co->x;
		co->b_ = co->c;
	}
	if (co->hue >= 240 && co->hue < 300)
	{
		co->r_ = co->x;
		co->g_ = 0;
		co->b_ = co->c;
	}
	if (co->hue >= 300 && co->hue <= 360)
	{
		co->r_ = co->c;
		co->g_ = 0;
		co->b_ = co->x;
	}
}

/* sets a malloced array (int *colors) of a seleced number(co.num_colors) of 
colors taken evenly from around a color wheel using hsl model, which allows 
the user to adjust saturation, lightness and starting hue, the stored 
colors will have been converted to rgb in the returned array */

//RETURN VALUE: a malloced array of colors or NULL on failure.
//NOTE: saturation should be set between 0 and 1 (it is like brightness)
// lightness also between 0 and 1 (start at .5 for no lightness effect)
// setting the base_hue at 0 starts with first color as red.

t_wheel	*set_color_wheel(t_wheel *wheel)
{
	int		i;
	t_wheel	co;

	co = *wheel;
	wheel->colors = (int *)malloc(co.num_colors * sizeof(int));
	if (!wheel->colors)
		return (NULL);
	i = -1;
	co.hue_step = 360.0 / (double)co.num_colors;
	while (++i < co.num_colors)
	{
		co.hue = fmod(co.base_hue + i * co.hue_step, 360.0);
		co.c = (1 - fabs(2 * co.lightness - 1)) * co.saturation;
		co.x = co.c * (1 - fabs(fmod(co.hue / 60.0, 2) - 1));
		co.m = co.lightness - co.c / 2.0;
		wheel_helper(&co);
		wheel_helper_2(&co);
		wheel->colors[i] = (co.trans << 24) | ((int)(255 * (co.r_ + co.m)) << 16)
			| ((int)(255 * (co.g_ + co.m)) << 8)
			| (int)(255 * (co.b_ + co.m));
	}
	return (wheel);
}