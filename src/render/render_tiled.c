#include "chaos.h"
#include "xoro128.h"

static inline int	min_i(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}


static inline void	export_put_argb(t_export *full,
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

static inline unsigned int	to_u8(float v)
{
	if (v < 0.0f)
		v = 0.0f;
	if (v > 1.0f)
		v = 1.0f;
	return ((unsigned int)(v * 255.0f + 0.5f));
}

//fill 1d unsigned char* array holding final image data

static void	copy_multibuff_tile_to_export(t_game *r, t_tile *t, t_export *full)
{
	int				x;
	int				y;
	int				src_x;
	int				src_y;
	int				dst_x;
	int				dst_y;
	unsigned int	a;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;

	y = -1;
	while (++y < t->h)
	{
		x = -1;
		while (++x < t->w)
		{
			src_x = x + (t->x0 - t->x0b);
			src_y = y + (t->y0 - t->y0b);

			dst_x = t->x0 + x;
			dst_y = t->y0 + y;

			red = to_u8(r->img_buffs[0][src_y][src_x]);
			green = to_u8(r->img_buffs[1][src_y][src_x]);
			blue = to_u8(r->img_buffs[2][src_y][src_x]);
			a = to_u8(r->img_buffs[3][src_y][src_x]);

			export_put_argb(full, dst_x, dst_y, a, red, green, blue);
		}
	}
}

//render the individual tile

static void	render_tile(t_game *r, t_tile *tile, uint64_t glob_seed, t_export *full_tiled)
{
	int		**vertices;
	double	x;
	double	y;

	sxoro128(&r->rng, glob_seed);
	render_init(r, &vertices, &x, &y);

	if (r->mode != 1 && r->multibuff && r->supersample)
	{
		loop_run(r, vertices, x, y, glob_seed);
		finalize_avg_var(r->img_buffs, r->buffs, render_w(r), render_h(r));
		thread_nlm(r, render_w(r), render_h(r));
		copy_multibuff_tile_to_export(r, tile, full_tiled);

		free_buffs(r);
	}
	else
	{
		run_game(r, vertices, x, y);
		downsample_xl_to_export_tile(r, tile, full_tiled);
		free_ui_matrix(r->pixels_xl, render_h_xl(r));
		r->pixels_xl = NULL;
	}
}

//set tile coordinates, size, etc.

static inline void	setup_tile(t_game *r, t_tile *t,
	int x0, int y0, int w, int h, int bleed)
{
	t->x0 = x0;
	t->y0 = y0;
	t->w = w;
	t->h = h;
	t->bleed = bleed;

	t->x0b = x0 - bleed;
	t->y0b = y0 - bleed;
	t->wb = w + bleed * 2;
	t->hb = h + bleed * 2;

	if (t->x0b < 0)
	{
		t->wb += t->x0b;
		t->x0b = 0;
	}
	if (t->y0b < 0)
	{
		t->hb += t->y0b;
		t->y0b = 0;
	}
	if (t->x0b + t->wb > r->width_orig)
		t->wb = r->width_orig - t->x0b;
	if (t->y0b + t->hb > r->height_orig)
		t->hb = r->height_orig - t->y0b;

	t->x0b_xl = t->x0b * r->s_kernel;
	t->y0b_xl = t->y0b * r->s_kernel;

	t->wb_xl = t->wb * r->s_kernel;
	t->hb_xl = t->hb * r->s_kernel;
}

/*setup and render large, high quality by working tiles and combining,
thereby sidestepping memory limitations. */ 

void	render_tiles(t_game *r)
{
	uint64_t	glob_seed;
	t_export	*full_tiled;
	size_t		full_bytes;
	long		start;
	long		end;
	int			tile_w;
	int			tile_h;
	int			bleed;
	int			tiles_x;
	int			tiles_y;
	int			tx;
	int			ty;
	int			x0;
	int			y0;
	int			w;
	int			h;


	r->use_tx = 0;
	r->multibuff = 0;

	tile_w = r->width_orig / 2;
	tile_h = r->height_orig / 2;

	//set up tile struct and img_ptr


	full_tiled = malloc(sizeof(*full_tiled));
	if (!full_tiled)
		clear_all(r);

	full_tiled->w = r->width_orig;
	full_tiled->h = r->height_orig;
	full_tiled->pixels = NULL;

	full_bytes = (size_t)full_tiled->w * (size_t)full_tiled->h * 4;
	full_tiled->pixels = malloc(full_bytes);
	if (!full_tiled->pixels)
		free(full_tiled), clear_all(r);

	memset(full_tiled->pixels, 0, full_bytes);
	//------------------------
	

	if (r->mode != 1 && r->multibuff && r->supersample)
		bleed = r->nlm.sr + r->nlm.f;
	else
		bleed = 0;

	glob_seed = 0xFABDECAF;

	tiles_x = (r->width_orig + tile_w - 1) / tile_w;
	tiles_y = (r->height_orig + tile_h - 1) / tile_h;

	start = get_time();

	ty = -1;
	while (++ty < tiles_y)
	{
		tx = -1;
		while (++tx < tiles_x)
		{
			x0 = tx * tile_w;
			y0 = ty * tile_h;

			w = min_i(tile_w, r->width_orig - x0);
			h = min_i(tile_h, r->height_orig - y0);

			setup_tile(r, r->tile, x0, y0, w, h, bleed);

			printf("rendering tile %d/%d, %d/%d\n",
				tx + 1, tiles_x, ty + 1, tiles_y);

			render_tile(r, r->tile, glob_seed, full_tiled);
		}
	}

	//perform final export to png here.
	//consider possible upgrade to directly write each tile into png, we never have to run with the full buffer.
	export_buffer(r, full_tiled);

	free(full_tiled->pixels);
	free(full_tiled);

	end = get_time();
	print_time(start, end, "TILED RENDER TIME: %f seconds\n");
}