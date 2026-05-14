#include "chaos.h"

void	print_tile_info(t_tile *t)
{
	printf("\n==== TILE INFO ====\n");

	printf("final tile:\n");
	printf("x0      = %d\n", t->x0);
	printf("y0      = %d\n", t->y0);
	printf("w       = %d\n", t->w);
	printf("h       = %d\n", t->h);

	printf("\nbleed:\n");
	printf("bleed   = %d\n", t->bleed);

	printf("\nbleeded final region:\n");
	printf("x0b     = %d\n", t->x0b);
	printf("y0b     = %d\n", t->y0b);
	printf("wb      = %d\n", t->wb);
	printf("hb      = %d\n", t->hb);

	printf("\nXL region:\n");
	printf("x0b_xl  = %d\n", t->x0b_xl);
	printf("y0b_xl  = %d\n", t->y0b_xl);
	printf("wb_xl   = %d\n", t->wb_xl);
	printf("hb_xl   = %d\n", t->hb_xl);

	printf("\nfinal crop offset inside bleed:\n");
	printf("crop_x  = %d\n", t->x0 - t->x0b);
	printf("crop_y  = %d\n", t->y0 - t->y0b);

	printf("\nXL crop offset:\n");
	printf("crop_xl = %d\n", (t->x0 - t->x0b));
	printf("crop_yl = %d\n", (t->y0 - t->y0b));

	printf("===================\n\n");
}

//deprecated ppm testing versions

/* 
static void	make_tile_filename(char *buf, size_t size, int tx, int ty)
{
	snprintf(buf, size, "tile_%04d_%04d.ppm", ty, tx);
}

static void	write_ppm_tile(t_game *r, t_tile *t, const char *filename)
{
	FILE			*f;
	int				x;
	int				y;
	int				src_x;
	int				src_y;
	unsigned char	rgb[3];

	f = fopen(filename, "wb");
	if (!f)
		return ;

	fprintf(f, "P6\n%d %d\n255\n", t->w, t->h);

	y = -1;
	while (++y < t->h)
	{
		x = -1;
		while (++x < t->w)
		{
			src_x = x + (t->x0 - t->x0b);
			src_y = y + (t->y0 - t->y0b);

			rgb[0] = (unsigned char)to_u8(r->img_buffs[0][src_y][src_x]);
			rgb[1] = (unsigned char)to_u8(r->img_buffs[1][src_y][src_x]);
			rgb[2] = (unsigned char)to_u8(r->img_buffs[2][src_y][src_x]);

			fwrite(rgb, 1, 3, f);
		}
	}
	fclose(f);
}

static void	render_tile(t_game *r, t_tile *tile, uint64_t glob_seed, const char *filename)
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
		write_ppm_tile(r, tile, filename);
		free_buffs(r);
	}
	else
	{
		run_game(r, vertices, x, y);
		downsample_xl_to_ppm_tile(r, tile, filename);
		free_ui_matrix(r->pixels_xl, render_h_xl(r));
	}
}

void	render_tiles(t_game *r)
{
	uint64_t	glob_seed;
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
	char		filename[128];

	r->use_tx = 0;
	r->multibuff = 0;

	tile_w = r->width_orig / 2;
	tile_h = r->height_orig / 2;

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

			make_tile_filename(filename, sizeof(filename), tx, ty);

			printf("rendering tile %d/%d, %d/%d: %s\n",
				tx + 1, tiles_x, ty + 1, tiles_y, filename);

			render_tile(r, r->tile, glob_seed, filename);
		}
	}


	end = get_time();
	print_time(start, end, "TILED RENDER TIME: %f seconds\n");
} */
