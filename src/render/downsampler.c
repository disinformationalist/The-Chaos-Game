#include "chaos.h"

static void	section_xl_to_multi(float ***multi, unsigned int **pixels_xl,
	int kern_size, t_filter *f, float n, int start_buff)
{
	int				i;
	int				j;
	float			red;
	float			green;
	float			blue;
	float			alpha;
	unsigned int	pixel;

	red = 0.0f;
	green = 0.0f;
	blue = 0.0f;
	alpha = 0.0f;
	j = -1;
	while (++j < kern_size)
	{
		i = -1;
		while (++i < kern_size)
		{
			pixel = pixels_xl[f->y + j][f->x + i];
			alpha += (pixel >> 24) & 0xFF;
			red += (pixel >> 16) & 0xFF;
			green += (pixel >> 8) & 0xFF;
			blue += pixel & 0xFF;
		}
	}
	multi[start_buff + 0][f->pix_y][f->pix_x] = red / n;
	multi[start_buff + 1][f->pix_y][f->pix_x] = green / n;
	multi[start_buff + 2][f->pix_y][f->pix_x] = blue / n;
	multi[start_buff + 3][f->pix_y][f->pix_x] = alpha / n;
}
//to use single xl buffer multiple times. multithread later

void	downsample_xl_to_multi(float ***multi, unsigned int **pixels_xl,
	int width, int height, int kern_size, int buff_id)
{
	t_filter	f;
	int			start_buff;
	float		n;

	start_buff = buff_id * 4;
	n = SQ(kern_size) * 255.0f;
	f.pix_y = 0;
	f.y = 0;
	while (f.y < height)
	{
		f.x = 0;
		f.pix_x = 0;
		while (f.x < width)
		{
			section_xl_to_multi(multi, pixels_xl, kern_size,
				&f, n, start_buff);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
}

//multithreaded version

static void	section_xl_to_multi_threaded(t_down_piece *p, int pix_x, int pix_y)
{
	int				i;
	int				j;
	int				x0;
	int				y0;
	float			red;
	float			green;
	float			blue;
	float			alpha;
	float			n;
	float			r0;
	float			g0;
	float			b0;
	float			a0;
	unsigned int	pixel;

	x0 = pix_x * p->kern_size;
	y0 = pix_y * p->kern_size;
	n = SQ(p->kern_size) * 255.0f;

	red = 0.0f;
	green = 0.0f;
	blue = 0.0f;
	alpha = 0.0f;

	j = -1;
	while (++j < p->kern_size)
	{
		i = -1;
		while (++i < p->kern_size)
		{
			pixel = p->pixels_xl[y0 + j][x0 + i];
			alpha += (pixel >> 24) & 0xFF;
			red += (pixel >> 16) & 0xFF;
			green += (pixel >> 8) & 0xFF;
			blue += pixel & 0xFF;
		}
	}

	r0 = red / n;
	g0 = green / n;
	b0 = blue / n;
	a0 = alpha / n;

	/*
		img_buffs[0..3] = sum RGBA
		img_buffs[4..7] = sumsq RGBA
	*/
	p->multi[0][pix_y][pix_x] += r0;
	p->multi[1][pix_y][pix_x] += g0;
	p->multi[2][pix_y][pix_x] += b0;
	p->multi[3][pix_y][pix_x] += a0;

	p->multi[4][pix_y][pix_x] += r0 * r0;
	p->multi[5][pix_y][pix_x] += g0 * g0;
	p->multi[6][pix_y][pix_x] += b0 * b0;
	p->multi[7][pix_y][pix_x] += a0 * a0;
}

static void	*run_downsample_piece(void *arg)
{
	t_down_piece	*p;
	int				x;
	int				y;

	p = (t_down_piece *)arg;
	y = p->y_s - 1;
	while (++y < p->y_e)
	{
		x = -1;
		while (++x < p->w)
			section_xl_to_multi_threaded(p, x, y);
	}
	return (NULL);
}

/*downsample xl image into buffers that hold the sums of each channel rgba and sumsq 
of each channel in prep for nlm*/

void	downsample_xl_to_multi_threaded(t_game *r, int w, int h)
{
	t_down_piece	*pieces;
	int				total;
	int				t;
	int				y0;
	int				y1;

	total = r->num_rows * r->num_cols;
	pieces = malloc(sizeof(t_down_piece) * total);
	if (!pieces)
		clear_all(r);

	t = -1;
	while (++t < total)
	{
		y0 = t * h / total;
		y1 = (t + 1) * h / total;

		pieces[t].r = r;
		pieces[t].multi = r->img_buffs;
		pieces[t].pixels_xl = r->pixels_xl;
		pieces[t].kern_size = r->s_kernel;
		pieces[t].buff_id = 0;
		pieces[t].y_s = y0;
		pieces[t].y_e = y1;
		pieces[t].w = w;
		pieces[t].h = h;

		if (pthread_create(&r->threads[t], NULL,
				&run_downsample_piece, &pieces[t]) != 0)
			thread_error(r, t);
	}
	join_threads(r->threads, 1, total);
	free(pieces);
}

//non nlm, direct to ppm version (no mthreead)

static void	write_ppm_header(FILE *f, int w, int h)
{
	fprintf(f, "P6\n%d %d\n255\n", w, h);
}

static void	filter_section_xl_to_ppm(
	FILE *f,
	unsigned int **pixels_xl,
	int src_x,
	int src_y,
	int kern_size)
{
	int				i;
	int				j;
	int				red;
	int				green;
	int				blue;
	int				alpha;
	int				avg_r;
	int				avg_g;
	int				avg_b;
	unsigned int	pixel;
	const int		n = SQ(kern_size);
	unsigned char	rgb[3];

	red = 0;
	green = 0;
	blue = 0;
	alpha = 0;

	j = -1;
	while (++j < kern_size)
	{
		i = -1;
		while (++i < kern_size)
		{
			pixel = pixels_xl[src_y + j][src_x + i];
			alpha += (pixel >> 24) & 0xFF;
			red += (pixel >> 16) & 0xFF;
			green += (pixel >> 8) & 0xFF;
			blue += pixel & 0xFF;
		}
	}

	avg_r = red / n;
	avg_g = green / n;
	avg_b = blue / n;

	(void)alpha;

	rgb[0] = (unsigned char)avg_r;
	rgb[1] = (unsigned char)avg_g;
	rgb[2] = (unsigned char)avg_b;

	fwrite(rgb, 1, 3, f);
}

void	downsample_xl_to_ppm_tile(t_game *r, t_tile *t, const char *filename)
{
	FILE	*f;
	int		out_x;
	int		out_y;
	int		src_x;
	int		src_y;
	int		crop_x;
	int		crop_y;

	f = fopen(filename, "wb");
	if (!f)
	{
		printf("failed to open %s\n", filename);
		return ;
	}

	write_ppm_header(f, t->w, t->h);

	/*
		t->x0/t->y0 are the real tile start.
		t->x0b/t->y0b are the bleeded tile start.
		So crop_x/y skip the left/top bleed in final-res pixels.
		Then multiply by s_kernel to get local XL source position.
	*/
	crop_x = (t->x0 - t->x0b) * r->s_kernel;
	crop_y = (t->y0 - t->y0b) * r->s_kernel;

	out_y = -1;
	while (++out_y < t->h)
	{
		out_x = -1;
		while (++out_x < t->w)
		{
			src_x = crop_x + out_x * r->s_kernel;
			src_y = crop_y + out_y * r->s_kernel;

			filter_section_xl_to_ppm(
				f,
				r->pixels_xl,
				src_x,
				src_y,
				r->s_kernel);
		}
	}

	fclose(f);
}

static inline void	export_put_argb1(t_export *full,
	int x, int y, unsigned int a, unsigned int r,
	unsigned int g, unsigned int b)
{
	size_t	i;

	i = ((size_t)y * (size_t)full->w + (size_t)x) * 4;
	full->pixels[i + 0] = (unsigned char)a;
	full->pixels[i + 1] = (unsigned char)r;
	full->pixels[i + 2] = (unsigned char)g;
	full->pixels[i + 3] = (unsigned char)b;
}

//strait to png version of tiles

static inline void	split_argb(unsigned int color,
	unsigned int *a, unsigned int *r,
	unsigned int *g, unsigned int *b)
{
	*a = (color >> 24) & 255;
	*r = (color >> 16) & 255;
	*g = (color >> 8) & 255;
	*b = color & 255;
}

static void	filter_section_xl_to_export(t_game *r,
	t_export *full, int dst_x, int dst_y,
	int src_x, int src_y, int s_kernel)
{
	int				x;
	int				y;
	unsigned int	a;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	unsigned int	color;
	uint64_t		sum_a;
	uint64_t		sum_r;
	uint64_t		sum_g;
	uint64_t		sum_b;
	uint64_t		n;

	sum_a = 0;
	sum_r = 0;
	sum_g = 0;
	sum_b = 0;

	y = -1;
	while (++y < s_kernel)
	{
		x = -1;
		while (++x < s_kernel)
		{
			color = r->pixels_xl[src_y + y][src_x + x];
			split_argb(color, &a, &red, &green, &blue);
			sum_a += a;
			sum_r += red;
			sum_g += green;
			sum_b += blue;
		}
	}

	n = (uint64_t)s_kernel * (uint64_t)s_kernel;

	export_put_argb1(full, dst_x, dst_y,
		(unsigned int)(sum_a / n),
		(unsigned int)(sum_r / n),
		(unsigned int)(sum_g / n),
		(unsigned int)(sum_b / n));
}

void	downsample_xl_to_export_tile(t_game *r, t_tile *t, t_export *full)
{
	int		out_x;
	int		out_y;
	int		src_x;
	int		src_y;
	int		dst_x;
	int		dst_y;
	int		crop_x;
	int		crop_y;

	crop_x = (t->x0 - t->x0b) * r->s_kernel;
	crop_y = (t->y0 - t->y0b) * r->s_kernel;

	out_y = -1;
	while (++out_y < t->h)
	{
		out_x = -1;
		while (++out_x < t->w)
		{
			src_x = crop_x + out_x * r->s_kernel;
			src_y = crop_y + out_y * r->s_kernel;

			dst_x = t->x0 + out_x;
			dst_y = t->y0 + out_y;

			filter_section_xl_to_export(
				r,
				full,
				dst_x,
				dst_y,
				src_x,
				src_y,
				r->s_kernel);
		}
	}
}