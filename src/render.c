/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:07:25 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/22 15:09:25 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chaos.h"
#include "xoro128.h"

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

//convert rule set to mask for faster checks

static inline uint32_t rules_to_mask(const bool *rules) 
{
    uint32_t m = 0;
    for (int i = 0; i < MAX_R && i < 32; ++i) 
		if (rules[i]) m |= (1u << i);
    return m;
}



static inline void	set_background_texture(t_game *r, t_img *tx)
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

	if (!r->supersample)
	{
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
}



/* static inline void set_background_texture(t_game *r, t_img *tx)
{
    int j, i;
    unsigned int back = r->colors.background & 0xFF000000; // AARRGGBB assumed

    if (!r->supersample)
    {
        for (j = 0; j < r->height; j++)
        {
            // map destination y -> source y
            int sy = (int)((long long)j * (double)r->height_tex / (double)r->height);
            sy = clampi(sy, 0, r->height_tex - 1);

            for (i = 0; i < r->width; i++)
            {
                int sx = (int)((long long)i * (double)r->width_tex / (double)r->width);
                sx = clampi(sx, 0, r->width_tex - 1);

                unsigned int color = map_color(tx, r, sx, sy);
                color = (color & 0x00FFFFFF) | back; // force alpha if you really want that
                my_pixel_put(i, j, &r->img, color);
            }
        }
    }
} */



//fill background color directly..

static inline void	set_background_color(t_game *r, unsigned int background)
{
	int	j;
	int	i;

	if (!r->supersample)
	{
		j = -1;
		while(++j < r->height)
		{
			i = -1;
			while (++i < r->width)
				my_pixel_put(i, j, &r->img, background);
		}
	}
}

//2 back color sometimes more clear looking on details function mode maybe other

static inline void	render_init(t_game *r, int ***vertices, double *x, double *y)//CONSIDER MOVIN POLYGON HERE FOR MULIT THREAD...
{	
	bool use_tx = 0;//TEMP TURN ON BACKGROUND TX
	
	if (r->resize)//maybe change to cycle through desired presets.
		resize_window(r, (int)((double)r->width_orig * r->win_change_x), (int)((double)r->height_orig * r->win_change_x));
	if (r->supersample)
	{
		r->pixels_xl = malloc_ui_matrix(r->width_orig * r->s_kernel, r->height_orig * r->s_kernel);
		if (!r->pixels_xl)
		{
			printf(RED"The super malloc has failed\n"RESET);
			close_screen(r);
		}
		if (!use_tx)
			set_ui_matrix(r->pixels_xl, r->width, r->height, r->colors.background);
		else
			set_ui_matrix_tx(r->pixels_xl, r->width, r->height, r, r->texture);
	}
	if (r->jump_to_sides)
		r->points = r->sides * 2;
	else
		r->points = r->sides;
	if (r->jump_to_center)
		r->points++;


	if (!set_color_wheel(r->wheel))//360, 1.0, 0.5, 202);//num, slbt
		clear_all(r);
	*x = r->width / 2 + r->move_x;
	*y = r->height / 2 - r->move_y; 
	*vertices = NULL;
	r->prev = 1;
	r->obp = 0;
	r->tbp = 2;
	r->thbp = 1;

	r->rules_mask = rules_to_mask(r->rules);
	//convert_colors_to_cmyk_safe(&r->colors);//uses only cmyk, not black
	if (!use_tx)
		set_background_color(r, r->colors.background);//no good with memset does gscale
	else//build frac onto a texture
		set_background_texture(r, r->texture);
		
}

static inline void	reset_img_memory(t_game *r)
{
	memset(r->img.pixels_ptr, (int)r->colors.background, r->width_orig * r->height_orig * \
		(r->img.bpp / 8));
}

static inline void	run_game(t_game *r, int **vertices, double x, double y)
{
	if (r->curved)
		chaos_game_curved(r, vertices, x, y);
	else if (r->mode == 1)
		chaos_game(r, vertices, x, y);
	else if (r->mode == 2 || r->mode == 3 || r->mode == 4)
		chaos_game_apply_function(r, vertices, x,  y);
}

static inline void	end_render(t_game *r)
{
	if (r->supersample)
	{	
		//mean_convo_filter(r->mlx_connect, &r->img_new, r->width, r->height, 9);//seems to help. smooth here then UNsharp mask!? monte carlo denoising? non local means!
		//---------perfom circle reflect in xl_size before downsample to make nice-----------this works!!!!!//square only for now
	/* 	unsigned int **new = pm_circle_matrix(r->pixels_xl, r->width, r->height, r->height / 8, r->height / 2 - r->height / 8);
		free(r->pixels_xl);
		r->pixels_xl = new; */
		//-----------------------------------------------------------------------------------
		downsample_xl(r->width, r->height, &r->img, r->pixels_xl, r->s_kernel);///
		free_ui_matrix(r->pixels_xl, r->height);
		
		//mean_convo_filter(r->mlx_connect, &r->img, r->width_orig, r->height_orig, 5);
		//gaussian_convo_filter(r->mlx_connect, &r->img, r->width_orig, r->height_orig, 7, 1.5);//testing.....

		ft_putstr_color("...SUPERSAMPLE COMPLETE\n", BOLD_BRIGHT_BLUE);
		
	}
	if (r->wheel)
		free(r->wheel->colors);
	// set the cmyk version img to switch between
	cmyk_softproof_image(r, &r->img, &r->cmyk);
	if (r->con_open)
		set_controls(r);
	else
		mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);

}

//This is now the main rendering function
//need to build out a multi buffer for super, multiple char *pix, on very large images
//they can be downsampled into the same img buffer.

void	render(t_game *r)
{
	long		start;
	long		end;
	int			**vertices;
	double		x;
	double		y;
	uint64_t	glob_seed;

	glob_seed = 0xFABDECAF;//secret number fAb DeCaF
	sxoro128(&r->rng, glob_seed);// * 142857);

	start = get_time();
	render_init(r, &vertices, &x, &y);
	run_game(r, vertices, x, y);
	end_render(r);
	end = get_time();
	print_time(start, end, "RENDER TIME: %f seconds\n");
}
