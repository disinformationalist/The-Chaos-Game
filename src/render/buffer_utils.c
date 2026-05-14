#include "chaos.h"


//stuff to set up vals for experimenting with some of the stuff used in buddha

void	set_vals(t_game *r, int min_1, int min_2, int min_3, int max_1, int max_2, int max_3)
{
	r->info->min1 = min_1;
	r->info->min2 = min_2;
	r->info->min3 = min_3;
	r->info->max1 = max_1;
	r->info->max2 = max_2;
	r->info->max3 = max_3;
}

void	set_edge_vals(t_color_info *info, float e0b, float e0g, float e0r, float e1b, float e1g, float e1r)
{
	info->edge0_b = e0b;
	info->edge0_g = e0g;
	info->edge0_r = e0r;
	info->edge1_b = e1b;
	info->edge1_g = e1g;
	info->edge1_r = e1r;
}

void	set_powers(t_color_info *info, float b, float g, float r)
{
	info->bpow = b;
	info->gpow = g;
	info->rpow = r;
}

void	init_color_info(t_game *r)
{
	t_color_info	*info;
	//t_btype			type;

	info = r->info;
	info->filter = false;
	info->fchan = 0;
	info->flevel = 5;
	info->smootherstep = false;
//.86,.93., .83   -.05, .7,.5,.55
	r->buffs = 3;
	if (info->smootherstep)//todo: gui for smoother controls, edges, powers, able to use with buddha also
		set_powers(info, .41, .39, .34);
		//set_powers(info, 1, 1, 1);
		//set_powers(info, .43, .38, .35);
	else
		set_powers(info, 1, 1, 1);
	set_edge_vals(info, .31, .29, .29, .59, .61, .61);
	//set_edge_vals(info, .35, .35, .35, .65, .65, .65);
}	
	//info->ftype = MEAN;


//needs to be set up for using color struct for per channel nlm, 
// we send each point to one of the buffers, use more points... use normal amount per buffer. iters *= buffs. 


void	free_fmatrix_i(float **matrix, int i)
{
	while (--i >= 0)
	{
		free (matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
}


void	zero_fmatrix(float **matrix, int width, int height)
{
	int	i;
	int	j;

	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
			matrix[j][i] = 0.0;
	}
}

void	free_3df_array_i(float ***array3d, int k, int height)
{
	while(--k >= 0)
		free_fmatrix_i(array3d[k], height);
	free(array3d);
	array3d = NULL;
}


float	**malloc_fmatrix(int width, int height)
{
	float	**matrix;
	int i;

	i = -1;
	matrix = (float **)malloc(height * sizeof(float *));
	if (!matrix)
		return (NULL);
	while (++i < height)
	{
		matrix[i] = (float *)malloc(width * sizeof(float));
		if (!matrix[i])
		{
			free_fmatrix_i(matrix, i);
			return (NULL);
		}
	}
	return (matrix);
}

//buffs 0 -> 3 =>sum
//buffs 4 -> 7 =>sumsq
//then
//buffs 0 -> 3 =>avgs
//buffs 4 -> 7 =>vars

void	init_multibuff(t_game *r, int w, int h)
{
	int	k;
	int buffs = 8;//4 color chans, 4 sums, 4 vars

	r->img_buffs = (float ***)malloc(buffs * sizeof(float **));
	if (!r->img_buffs)
		clear_all(r);
	k = -1;
	while (++k < buffs)
	{
		r->img_buffs[k] = malloc_fmatrix(w, h);
		if (!r->img_buffs[k])
		{
			free_3df_array_i(r->img_buffs, k, h);
			clear_all(r);
		}
		zero_fmatrix(r->img_buffs[k], w, h);
	}
}

// set the average values for r g b a buffers into buffers[0, 1, 2, 3]  variances into buffer[4,5,6,7]

void	combine_buff_set_var(float ***buffers, int chan, int buffs, int width, int height)
{
	int		j;
	int		i;
	int		k;
	float	avg;
	float	var;
	float	sum;

		j = -1;
		while (++j < height)
		{
			i = -1;
			while (++i < width)
			{
				k = -1;
				sum = 0;
				//get avg
				while (++k < buffs)
					sum += buffers[chan + k * 4][j][i];
				avg = sum / buffs;
				k = -1;
				sum = 0;
				//get var
				while (++k < buffs)
				{
					float v1 = avg - buffers[chan + k * 4][j][i];
					sum += v1 * v1;
				}
				var = sum / buffs;
				buffers[chan][j][i] = avg;
				buffers[chan + 4][j][i] = var;
			}
		}
}

//replaces above ft

void	combine_to_avg_var(float ***buffers, int buffs,
		int width, int height)
{
	int		i;
	int		j;
	int		k;
	int		c;
	float	sum;
	float	avg;
	float	var;
	float	diff;

	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			c = -1;
			while (++c < 4) // RGBA
			{
				sum = 0.0f;

				k = -1;
				while (++k < buffs)
					sum += buffers[k * 4 + c][j][i];

				avg = sum / (float)buffs;

				sum = 0.0f;
				k = -1;
				while (++k < buffs)
				{
					diff = buffers[k * 4 + c][j][i] - avg;
					sum += diff * diff;
				}

				if (buffs > 1)
					var = sum / (float)(buffs - 1);
				else
					var = 0.0f;

				// write into compact layout
				buffers[c][j][i] = avg;
				buffers[c + 4][j][i] = var;
			}
		}
	}
}
//this replaces the ft above it.
void	finalize_avg_var(float ***buffers, int buffs, int width, int height)
{
	int		i;
	int		j;
	int		c;
	float	sum;
	float	sumsq;
	float	avg;
	float	var;

	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			c = -1;
			while (++c < 4)
			{
				sum = buffers[c][j][i];
				sumsq = buffers[c + 4][j][i];

				avg = sum / (float)buffs;

				if (buffs > 1)
					var = (sumsq - ((sum * sum) / (float)buffs))
						/ (float)(buffs - 1);
				else
					var = 0.0f;

				if (var < 0.0f)
					var = 0.0f;

				buffers[c][j][i] = avg;
				buffers[c + 4][j][i] = var;
			}
		}
	}
}

void	color_to_buffs(unsigned int color_in, int x, int y, float ***img_buffs)
{
	img_buffs[0][y][x] = ((color_in >> 24) & 0xFF) / 255.0f; // alpha
	img_buffs[1][y][x] = ((color_in >> 16) & 0xFF) / 255.0f; // red
	img_buffs[2][y][x] = ((color_in >> 8)  & 0xFF) / 255.0f; // green
	img_buffs[3][y][x] = ( color_in        & 0xFF) / 255.0f; // blue
}

//color is first curr array, var is curr var array,  use:
//nlm_fchannel(r->buffs[curr](using 1,2,3,4 thats r,g,b,a,), r->buffs[curr + 4] (thats var r, var g, var b, var a) ())

float	**nlm_fchannel(float **color, float **var, int width, int height)
{
	int f = 2; //patch_rad = 5 x 5 square
	int r = 15; //search_rad(window) = 31 x 31 square
	float kc = 1.0;
	float eps = 1e-8;
	float **out;

	out = malloc_fmatrix(width, height);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			float total_weight = 0.0;
			float filtered_val = 0.0;
			  for (int dy = -r; dy <= r; dy++) {
                for (int dx = -r; dx <= r; dx++) {
                    int qx = i + dx;
                    int qy = j + dy;

                    if (qx < 0 || qx >= width || qy < 0 || qy >= height)
                        continue;

                    float d2 = 0.0;

                    for (int py = -f; py <= f; py++) {
                        for (int px = -f; px <= f; px++) {
                            int p1x = i + px, p1y = j + py;
                            int p2x = qx + px, p2y = qy + py;

                            if (p1x < 0 || p1x >= width || p1y < 0 || p1y >= height ||
                                p2x < 0 || p2x >= width || p2y < 0 || p2y >= height)
                                continue;

                            float u1 = color[p1y][p1x];
                            float u2 = color[p2y][p2x];
                            float var1 = var[p1y][p1x];
                            float var2 = var[p2y][p2x];

                            float delta = (u1 - u2) * (u1 - u2);
                            float var_corr = var1 + ((var1 < var2) ? var1 : var2);
                            float norm = eps + kc * kc * (var1 + var2);

                            d2 += (delta - var_corr) / norm;
                        }
                    }

                    d2 = fmax(0.0, d2 / ((2*f + 1)*(2*f + 1)));

                    float w = exp(-d2);
                    total_weight += w;
                    filtered_val += w * color[qy][qx];
                }
            }
            out[j][i] = (total_weight > 0) ? filtered_val / total_weight : color[j][i];
		}
	}
	free_fmatrix_i(color, height);
	return (out);
}



/*NLM STUFF*/

static void	nlm_fchannel_piece(float **color, float **var, float **out,
	int width, int height, t_piece *piece, t_nl n)
{
	
	int		i;
	int		j;
	float 	eps;

	eps = 1e-8f;
	j = piece->y_s - 1;
	while (++j < piece->y_e)
	{
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			float	total_weight = 0.0f;
			float	filtered_val = 0.0f;
			int		dy = -n.sr - 1;

			while (++dy <= n.sr)
			{
				int dx = -n.sr - 1;
				while (++dx <= n.sr)
				{
					int qx = i + dx;
					int qy = j + dy;

					if (qx < 0 || qx >= width || qy < 0 || qy >= height)
						continue;

					float	d2 = 0.0f;
					int		patch_count = 0;
					int		py = -n.f - 1;

					while (++py <= n.f)
					{
						int px = -n.f - 1;
						while (++px <= n.f)
						{
							int p1x = i + px;
							int p1y = j + py;
							int p2x = qx + px;
							int p2y = qy + py;

							if (p1x < 0 || p1x >= width || p1y < 0 || p1y >= height ||
								p2x < 0 || p2x >= width || p2y < 0 || p2y >= height)
								continue;

							float u1 = color[p1y][p1x];
							float u2 = color[p2y][p2x];
							float var1 = var[p1y][p1x];
							float var2 = var[p2y][p2x];
							float diff = u1 - u2;
							float delta = diff * diff;
							float var_sum = var1 + var2;
							float norm = eps + n.kc * n.kc * var_sum;

							d2 += (delta - var_sum) / norm;
							patch_count++;
						}
					}

					if (patch_count > 0)
						d2 = fmaxf(0.0f, d2 / (float)patch_count);
					else
						d2 = 0.0f;

					float w = expf(-d2);
					total_weight += w;
					filtered_val += w * color[qy][qx];
				}
			}
			if (total_weight > 0.0f)
				out[j][i] = filtered_val / total_weight;
			else
				out[j][i] = color[j][i];
		}
	}
}

static void	*run_nlm(void *arg)
{
	t_nlm_piece	*np;
	t_game		*r;
	t_nl 		n;
	int			chan;

	np = (t_nlm_piece *)arg;
	r = np->r;
	n = r->nlm;

	chan = -1;
	while (++chan < 4)
	{
		nlm_fchannel_piece(
			r->img_buffs[chan],
			r->img_buffs[chan + 4],
			np->out[chan],
			np->width,
			np->height,
			&np->piece, n);
	}
	return (NULL);
}

static void	set_pieces_nlm(t_game *r, t_piece *piece, int i, int j, int width, int height)
{
	piece->x_s = i * (width / r->num_cols);
	piece->x_e = (i + 1) * (width / r->num_cols);
	piece->y_s = j * (height / r->num_rows);
	piece->y_e = (j + 1) * (height/ r->num_rows);

	if (i == r->num_cols - 1)
		piece->x_e = width;
	if (j == r->num_rows - 1)
		piece->y_e = height;

	piece->r = r;
}



void	thread_nlm(t_game *r, int width, int height)
{
	t_nlm_piece	*pieces;
	float		***out;
	int			total;
	int			i;
	int			j;
	int			idx;
	int			chan;

	total = r->num_rows * r->num_cols;
	pieces = malloc(sizeof(t_nlm_piece) * total);
	if (!pieces)
		clear_all(r);

	out = malloc(sizeof(float **) * 4);
	if (!out)
		clear_all(r);

	chan = -1;
	while (++chan < 4)
	{
		out[chan] = malloc_fmatrix(width, height);
		if (!out[chan])
			clear_all(r);
	}

	j = -1;
	while (++j < r->num_rows)
	{
		i = -1;
		while (++i < r->num_cols)
		{
			idx = j * r->num_cols + i;
			pieces[idx].r = r;
			pieces[idx].out = out;
			pieces[idx].width = width;
			pieces[idx].height = height;
			set_pieces_nlm(r, &pieces[idx].piece, i, j, width, height);

			if (pthread_create(&r->threads[idx], NULL,
					&run_nlm, (void *)&pieces[idx]) != 0)
				thread_error(r, idx);
		}
	}

	join_threads(r->threads, r->num_rows, r->num_cols);

	chan = -1;
	while (++chan < 4)
	{
		free_fmatrix_i(r->img_buffs[chan], height);
		r->img_buffs[chan] = out[chan];
	}

	free(out);
	free(pieces);
}
//threaded version for the simple coloring

static inline unsigned int	to_u8(float v)
{
	if (v < 0.0f)
		v = 0.0f;
	if (v > 1.0f)
		v = 1.0f;
	return ((unsigned int)(v * 255.0f + 0.5f));
}

static void	*set_color_simple_thread(void *arg)
{
	t_piece			*piece;
	t_game			*r;
	int				i;
	int				j;
	unsigned int	a;
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	unsigned int	color;

	piece = (t_piece *)arg;
	r = piece->r;
	j = piece->y_s - 1;
	while (++j < piece->y_e)
	{
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			red = to_u8(r->img_buffs[0][j][i]);
			green = to_u8(r->img_buffs[1][j][i]);
			blue = to_u8(r->img_buffs[2][j][i]);
			a = to_u8(r->img_buffs[3][j][i]);

			color = (a << 24) | (red << 16) | (green << 8) | blue;
			my_pixel_put(i, j, &r->img, color);
		}
	}
	return (NULL);
}

static void	set_piece_final(t_game *r, t_piece *piece, int i, int j)
{
	piece->x_s = i * (r->width_orig / r->num_cols);
	piece->x_e = (i + 1) * (r->width_orig / r->num_cols);
	piece->y_s = j * (r->height_orig / r->num_rows);
	piece->y_e = (j + 1) * (r->height_orig / r->num_rows);

	if (i == r->num_cols - 1)
		piece->x_e = r->width_orig;
	if (j == r->num_rows - 1)
		piece->y_e = r->height_orig;

	piece->r = r;
}

void	color_buffers_simple_threaded(t_game *r)
{
	t_piece	*pieces;
	int		i;
	int		j;
	int		idx;
	int		total;

	total = r->num_rows * r->num_cols;
	pieces = malloc(sizeof(t_piece) * total);
	if (!pieces)
		clear_all(r);

	j = -1;
	while (++j < r->num_rows)
	{
		i = -1;
		while (++i < r->num_cols)
		{
			idx = j * r->num_cols + i;
			set_piece_final(r, &pieces[idx], i, j);
			if (pthread_create(&r->threads[idx], NULL,
					&set_color_simple_thread, &pieces[idx]) != 0)
				thread_error(r, idx);
		}
	}

	join_threads(r->threads, r->num_rows, r->num_cols);
	free(pieces);
}

//patch rad: 1 for buddha2, 2 for buddha 1