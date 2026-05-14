#include "chaos.h"

void	reset_background_color(t_game *r, unsigned int background, int w, int h)
{
	int	j;
	int	i;

	j = -1;
	while(++j < h)
	{
		i = -1;
		while (++i < w)
			my_pixel_put(i, j, &r->img, background);
	}
}

//can be used to 0 out the img values, but not set background as memset only does grayscale

static inline void	reset_img_memory(t_game *r)
{
	memset(r->img.pixels_ptr, 0, r->width_orig * r->height_orig * \
		(r->img.bpp / 8));
}


//sets background color for the super image

static inline void	set_ui_matrix(unsigned int **pixels_xl, int width, int height, unsigned int color)
{
	int j;
	int i;
	
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
			pixels_xl[j][i] = color;
	}
}

static inline void	set_ui_matrix_tx(unsigned int **pixels_xl, int width, int height, t_game *r, t_img *tx)
{
	double fact_w = (double)r->width_tex / (double)r->width_orig;
	double fact_h =  (double)r->height_tex / (double)r->height_orig;
	if (fact_w == 0)
		fact_w += 1.0;
	if (fact_h == 0)
		fact_h += 1.0;
	int j;
	int i;
	unsigned int back;
	unsigned int color;
	back = r->colors.background;
	back >>= 24;
	back <<= 24;
	
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			color = map_color(tx, r, ft_round((double)i * fact_w), ft_round((double)j * fact_h));
			/* if (i < width / 2 || j > height / 2)
				color = (color & 0x00FFFFFF) | 0x00000000;
			else */
				color = (color & 0x00FFFFFF) | back;
			pixels_xl[j][i] = color; 
		}
	}
}


void	set_ui_mat(t_game *r, bool use_tx, int w, int h)
{
	if (!use_tx)
		set_ui_matrix(r->pixels_xl, w, h, r->colors.background);
	else
		set_ui_matrix_tx(r->pixels_xl, w, h, r, r->texture);
}

void init_xl(t_game *r, bool use_tx, int w, int h)
{
	r->pixels_xl = malloc_ui_matrix(w, h);
	if (!r->pixels_xl)
	{
		printf(RED"The super malloc has failed\n"RESET);
		close_screen(r);
	}
	set_ui_mat(r, use_tx, w, h);
}

void	set_background_texture(t_game *r, t_img *tx)
{
	int	j;
	int	i;
	double fact_w = (double)r->width_tex / (double)r->width_orig;
	double fact_h =  (double)r->height_tex / (double)r->height_orig;
	if (fact_w == 0)
		fact_w += 1.0;
	if (fact_h == 0)
		fact_h += 1.0;
	unsigned int color;
	unsigned int back;
	back = r->colors.background;
	back >>= 24;
	back <<= 24;

	j = -1;
	while(++j < r->height)
	{
		i = -1;
		while (++i < r->width)
		{
			color =	map_color(tx, r, ft_round((double)i * fact_w), ft_round((double)j * fact_h));
			color = (color & 0x00FFFFFF) | back;
			my_pixel_put(i, j, &r->img, color);
		}
	}
}

//fill background color directly..

void	set_background_color(t_game *r, unsigned int background, int w, int h)
{
	int	j;
	int	i;

	j = -1;
	while(++j < h)
	{
		i = -1;
		while (++i < w)
			my_pixel_put(i, j, &r->img, background);
	}
}