#include "chaos.h"


// multi[0] = avg red
// multi[1] = avg green
// multi[2] = avg blue
// multi[3] = avg alpha

void	set_highs(int width, int height, float ***multi, t_game *r)
{
	float	*highs;
	int		i;
	int		j;
	int		k;

	highs = malloc(sizeof(float) * 4);
	if (!highs)
		clear_all(r);

	k = -1;
	while (++k < 4)
		highs[k] = 0.0f;

	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			k = -1;
			while (++k < 4)
			{
				if (highs[k] < multi[k][j][i])
					highs[k] = multi[k][j][i];
			}
		}
	}

	if (r->info->highs)
		free(r->info->highs);
	r->info->highs = highs;
}

float	clampf(float f)
{
	if (f < 0.0)
		f = 0.0;
	if (f > 1.0)
		f = 1.0;
	return (f);
}

float smoothstep(float edge0, float edge1, float x)
{
	float t;

	t = clampf((x - edge0) / (edge1 - edge0));
	return (t * t * (3 - 2 * t));
}

//x * x * x * (x * (6.0f * x - 15.0f) + 10.0f)

float smootherstep(float edge0, float edge1, float x)
{
	float t;

	t = clampf((x - edge0) / (edge1 - edge0));
	return (t * t * t * (t * (6.0f * t - 15.0f) + 10.0f));
}

static inline void set_pixel(t_game *r, int i, int j, unsigned int color)
{
	//if (!r->supersample)
		my_pixel_put_plus(i, j, &r->img, color);
	/* else
		r->pixels_xl[j][i] |= color; */
}

static inline void	set_blue(t_game *r, float num, float **density, t_piece *piece, den_f numf)
{
	unsigned int		color;
	int					i;
	int					j;
	float				n;
	float				factor;
	t_color_info		info;

	info = *(r->info);

	if (info.smootherstep)
		n = 1.0 / numf(num, info.bpow);
	else
		n = 255.0 / numf(num, info.bpow);

	//n = 255.0 / numf(log(num + 1), r->info->bpow);//log scaling
	j = piece->y_s - 1;
	while (++j < piece->y_e)
	{
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			factor = numf(density[j][i], info.bpow) * n;
			//factor = numf(log(density[j][i] + 1), r->info->bpow) * n;
			
			if (info.smootherstep)
				color = ft_round(smootherstep(info.edge0_b, info.edge1_b, factor) * 255.0);
			else
				color = ft_round(factor);
			
			//---
			//density[j][i] = color;
			//set_pixel(r, i, j, color);
			my_pixel_put_plus(i, j, &r->img, color);

		}
	}
}

static inline void	set_green(t_game *r, float num, float **density, t_piece *piece, den_f numf)
{
	unsigned int	color;
	int				i;
	int				j;
	float			n;
	float			factor;
	t_color_info	info;

	info = *(r->info);

	if (info.smootherstep)
		n = 1.0 / numf(num, info.gpow);
	else
		n = 255.0 / numf(num, info.gpow);

	j = piece->y_s - 1;
	while (++j < piece->y_e)
	{
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			factor = numf(density[j][i], info.gpow) * n;

			if (info.smootherstep)
				color = ft_round(smootherstep(info.edge0_g, info.edge1_g, factor) * 255.0) << 8;
			else
				color = ft_round(factor) << 8;
				//---
			//density[j][i] = color;
			//set_pixel(r, i, j, color);
			my_pixel_put_plus(i, j, &r->img, color);

		}
	}
}

static inline void	set_red(t_game *r, float num, float **density, t_piece *piece, den_f numf)
{
	unsigned int	color;
	int				i;
	int				j;
	float			n;
	float			factor;
	t_color_info	info;


	info = *(r->info);
	if (info.smootherstep)
		n = 1.0 / numf(num, info.rpow);
	else
		n = 255.0 / numf(num, info.rpow);

//	n = 255.0 / numf(log(num + 1), r->info->rpow);//make this method too

	j = piece->y_s - 1;
	while (++j < piece->y_e)
	{
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			factor = numf(density[j][i], info.rpow) * n;
//			factor = numf(log(density[j][i] + 1), r->info->rpow) * n;

			if (info.smootherstep)
				color = ft_round(smootherstep(info.edge0_r, info.edge1_r, factor) * 255.0) << 16;
			else
				color = ft_round(factor) << 16;
			
				//---
			//density[j][i] = color;
			//set_pixel(r, i, j, color);
			my_pixel_put_plus(i, j, &r->img, color);

		}
	}
}

static inline void	set_alpha(t_game *r, float num, float **density, t_piece *piece, den_f numf)
{
	unsigned int		color;
	int					i;
	int					j;
	float				n;
	float				factor;
	t_color_info		info;

	info = *(r->info);

	if (info.smootherstep)
		n = 1.0 / numf(num, info.bpow);
	else
		n = 255.0 / numf(num, info.bpow);

	//n = 255.0 / numf(log(num + 1), r->info->bpow);//log scaling
	j = piece->y_s - 1;
	while (++j < piece->y_e)
	{
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			factor = numf(density[j][i], info.bpow) * n;
			//factor = numf(log(density[j][i] + 1), r->info->bpow) * n;
			
			if (info.smootherstep)
				color = ft_round(smootherstep(info.edge0_b, info.edge1_b, factor) * 255.0);
			else
				color = ft_round(factor);
			
			//---
			//density[j][i] = color;
			//set_pixel(r, i, j, color);
			my_pixel_put_plus(i, j, &r->img, color);

		}
	}
}

float pow_ft(float num, float power)
{
    return pow(num, power);
}

/**use multithread to perform any color adjustments and recombine back into a final image **/

static inline void	*set_colors(void *arg)
{
	t_piece			*piece;
	t_game			*r;
	t_color_info	*info;
	/* int				width;
	int				height; */

	piece = (t_piece *)arg;
	r = piece->r;
	info = r->info;

	set_red(r, info->highs[0], r->img_buffs[0], piece, pow_ft);
	set_green(r, info->highs[1], r->img_buffs[1], piece, pow_ft);
	set_blue(r, info->highs[2], r->img_buffs[2], piece, pow_ft);
//	set_alpha(r, info->highs[3], r->img_buffs[3], piece, pow_ft);
	pthread_exit(NULL);
}

static inline void	set_pieces_color(t_game *r, t_piece piece[][r->num_cols], int i, int j)
{
	piece[j][i].x_s = i * (r->width_orig / r->num_cols);
	piece[j][i].x_e = (i + 1) * (r->width_orig/ r->num_cols);
	piece[j][i].y_s = j * (r->height_orig / r->num_rows);
	piece[j][i].y_e = (j + 1) * (r->height_orig / r->num_rows);
	piece[j][i].r = r;
}

void	thread_error(t_game *r, int i)
{
	while (--i >= 0)
	{
		if (r->threads[i])
			pthread_join(r->threads[i], NULL);
	}
	/* pthread_mutex_destroy(&r->mutex);
	pthread_mutex_destroy(&r->rand_mtx); */
	clear_all(r);
}

//multithreaded per channel color to allow channel manipulations

void	color_buffers(t_game *r)
{
	t_piece	piece[r->num_rows][r->num_cols];
	int		i;
	int		j;

	j = -1;
	while (++j < r->num_rows)
	{
		i = -1;
		while (++i < r->num_cols)
		{
				set_pieces_color(r, piece, i, j);
			if (pthread_create(&r->threads[j * r->num_cols + i], NULL, \
				&set_colors, (void *)&piece[j][i]) != 0)
				thread_error(r, j * r->num_cols + i);
		}
	}
	join_threads(r->threads, r->num_rows, r->num_cols);
}
