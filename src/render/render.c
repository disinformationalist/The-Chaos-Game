
#include "chaos.h"
#include "xoro128.h"



//dimension helpers

int	render_w(t_game *r)
{
	if (r->tile_mode)
		return (r->tile->wb);
	return (r->width_orig);
}

int	render_h(t_game *r)
{
	if (r->tile_mode)
		return (r->tile->hb);
	return (r->height_orig);
}

int	render_w_xl(t_game *r)
{
	if (r->tile_mode)
		return (r->tile->wb_xl);
	return (r->width);
}

int	render_h_xl(t_game *r)
{
	if (r->tile_mode)
		return (r->tile->hb_xl);
	return (r->height);
}

//direct coloring without channel manipulation:
static inline unsigned int	to_u8(float v)
{
	if (v < 0.0f)
		v = 0.0f;
	if (v > 1.0f)
		v = 1.0f;
	return ((unsigned int)(v * 255.0f + 0.5f));
}

void	color_buffers_simple(t_game *r)
{
	int				i;
	int				j;
	unsigned int	a;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	unsigned int	color;

	j = -1;
	while (++j < r->height_orig)
	{
		i = -1;
		while (++i < r->width_orig)
		{
			red = to_u8(r->img_buffs[0][j][i]);
			green = to_u8(r->img_buffs[1][j][i]);
			blue = to_u8(r->img_buffs[2][j][i]);
			a = to_u8(r->img_buffs[3][j][i]);

			color = (a << 24) | (red << 16) | (green << 8) | blue;
			my_pixel_put(i, j, &r->img, color);
		}
	}
}

//----------------------

void free_buffs(t_game *r)
{
	if (r->pixels_xl)
	{
		free_ui_matrix(r->pixels_xl, render_h_xl(r));
		r->pixels_xl = NULL;
	}
	if (r->info->highs)
	{	
		free(r->info->highs);
		r->info->highs = NULL;
	}
	if (r->img_buffs)
	{
		free_3df_array_i(r->img_buffs, 8, render_h(r));
		r->img_buffs = NULL;
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

static inline void	init_history(t_game *r,  double *x, double *y, bool rv)
{
	*x = r->width / 2 + r->move_x;
	*y = r->height / 2 - r->move_y; 
	r->prev = 1;
	r->obp = 0;
	r->tbp = 2;
	r->thbp = 1;

	if (rv)
	{
		int i;
		i = -1;
		while (++i < r->rv_len)
			r->rv[i] = i;
	}
}

//2 back color sometimes more clear looking on details function mode maybe other


void	render_init(t_game *r, int ***vertices, double *x, double *y)//CONSIDER MOVIN POLYGON HERE FOR MULIT THREAD...
{	
	if (r->resize && !r->tile_mode)//maybe change to cycle through desired presets.
		resize_window(r, (int)((double)r->width_orig * r->win_change_x), (int)((double)r->height_orig * r->win_change_x));
	if (r->supersample)
	{
		init_xl(r, r->use_tx, render_w_xl(r), render_h_xl(r));
		if (r->mode != 1 && r->multibuff)
		{
			if (r->s_kernel > 18 && !r->tile_mode)
			{
				printf(BRIGHT_RED"supersample must not exceed 18 for multibuffer\n");
				close_screen(r);
			}
			if (!r->tile_mode)
				reset_background_color(r, r->colors.background, render_w(r),  render_h(r));//set backround for fin image buffer as putplus adds, not overwrites
			init_multibuff(r, render_w(r),  render_h(r));//init the buffs for the nlm
		}
	}
	else if(!r->tile_mode)
	{
		if (!r->use_tx)
			set_background_color(r, r->colors.background, render_w(r),  render_h(r));//no good with memset does gscale
		else//build frac onto a texture
			set_background_texture(r, r->texture);
	}

	if (r->jump_to_sides)
		r->points = r->sides * 2;
	else
		r->points = r->sides;
	if (r->jump_to_center)
		r->points++;

	
	*vertices = NULL;
	init_history(r,  x, y, 1);
	r->rules_mask = rules_to_mask(r->rules);
	//convert_colors_to_cmyk_safe(&r->colors);//uses only cmyk, not black
}

static inline void	end_render(t_game *r)
{
	if (r->supersample)
	{	
		if (!r->multibuff || r->mode == 1)
		{
			downsample_xl(render_w_xl(r), render_h_xl(r), &r->img, r->pixels_xl, r->s_kernel);
			free_ui_matrix(r->pixels_xl, render_h_xl(r));
		}
		else
		{
			finalize_avg_var(r->img_buffs, r->buffs, render_w(r),  render_h(r));
			thread_nlm(r, render_w(r), render_h(r));//multithread nlm run on each color channel

			//color the final image buffers and combine
			/* set_highs(r->width_orig, r->height_orig, r->img_buffs, r);//these two for per channel manip, powft, smootherstep, etc
			color_buffers(r); */
			color_buffers_simple(r);//directly convert and pack
			//color_buffers_simple_threaded(r);//this was a lot slower??
			free_buffs(r);
		}
		ft_putstr_color("...SUPERSAMPLE COMPLETE\n", BOLD_BRIGHT_BLUE);
	}
	
	// set the cmyk version img to switch between
	cmyk_softproof_image(r, &r->img, &r->cmyk);
	if (r->con_open)
		set_controls(r);
	else
		mlx_put_image_to_window(r->mlx_connect, r->mlx_win, r->img.img_ptr, 0, 0);

}

void	run_game(t_game *r, int **vertices, double x, double y)
{
	if (r->curved)
		chaos_game_curved(r, vertices, x, y);
	else if (r->mode == 1)
		chaos_game(r, vertices, x, y);
	else if (r->mode == 2 || r->mode == 3 || r->mode == 4)
		chaos_game_apply_function(r, vertices, x,  y);
}



//for multiple runs of the multibuff loop

void	loop_run(t_game *r, int **verts, double x, double y, uint64_t glob_seed)
{
	//statistically indepentent runs for buff count 
	for (int b = 0; b < r->buffs; b++)
	{

		r->curr_buff = b;//number of the xl buff associated
		r->i = 0;
		set_ui_mat(r, r->use_tx, render_w_xl(r), render_h_xl(r));
		run_game(r, verts, x, y);
		downsample_xl_to_multi_threaded(r, render_w(r), render_h(r));
		
		/* reset for full independence. Adding this greatly improved nlm 
		result quality, look into for buddha nlm one day */
		init_history(r,  &x, &y, 1);
		sxoro128(&r->rng, glob_seed + b * 0x9E3779B97F4A7C15ULL);
	}
}


//This is now the main rendering function
//need to build out a multi buffer for super, multiple char *pix, on very large images- ammend, now planning to do image stitching
//they can be downsampled into the same img buffer.

void	render(t_game *r)
{
	long		start;
	long		end;
	int			**vertices;
	double		x;
	double		y;
	uint64_t	glob_seed;

	r->use_tx = 0;//TEMP TURN ON BACKGROUND TX, move these to outside and make gui/ key on/off
	r->multibuff = 0;//TEMP TURN ON multibuff for nlm

	glob_seed = 0xFABDECAF;//secret number fAb DeCaF
	sxoro128(&r->rng, glob_seed);// * 142857);

	start = get_time();
	render_init(r, &vertices, &x, &y);

	if (r->mode != 1 && r->multibuff && r->supersample)//nlm
		loop_run(r, vertices, x, y, glob_seed);
	else
		run_game(r, vertices, x, y);//normal run

	end_render(r);
	end = get_time();
	print_time(start, end, "RENDER TIME: %f seconds\n");
}






		//---------perfom circle reflect in xl_size before downsample to make nice-----------this works!!!!!//square only for now
	/* 	unsigned int **new = pm_circle_matrix(r->pixels_xl, r->width, r->height, r->height / 8, r->height / 2 - r->height / 8);
		free(r->pixels_xl);
		r->pixels_xl = new; */
		//-----------------------------------------------------------------------------------
		/* downsample_xl(r->width, r->height, &r->img, r->pixels_xl, r->s_kernel);///just these 2 lines with no multibuff nlm
		free_ui_matrix(r->pixels_xl, r->height); */
		
		//mean_convo_filter(r->mlx_connect, &r->img, r->width_orig, r->height_orig, 5);
		//gaussian_convo_filter(r->mlx_connect, &r->img, r->width_orig, r->height_orig, 7, 1.5);//testing.....


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


