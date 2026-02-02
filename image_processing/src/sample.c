/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:13:46 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:13:48 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_processing.h"

static void	filter_section_d(t_img *img, t_img *img_big, int kern_size, t_filter *f)
{
	f->red = 0;
	f->green = 0;
	f->blue = 0;
	f->j = -f->half_k - 1;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		while (++f->i <= f->half_k)
		{
			f->offset = (f->y + f->j) * img_big->line_len + \
			((f->x + f->i) * (img_big->bpp / 8));
			f->pixel = *(unsigned int *)(img_big->pixels_ptr + f->offset);
			f->red += (f->pixel >> 16) & 0xFF;
			f->green += (f->pixel >> 8) & 0xFF;
			f->blue += f->pixel & 0xFF;
		}
	}
	f->avg_r = f->red / (kern_size * kern_size);
	f->avg_g = f->green / (kern_size * kern_size);
	f->avg_b = f->blue / (kern_size * kern_size);
	my_pixel_put(f->pix_x, f->pix_y, img, \
	((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

void	downsample(long width, long height, t_img *img, t_img *img_big, int kern_size) //maybe switch img order, so in, then out. intiuitive//change to ints on inputs
{
	t_filter	f;
	
	f.pix_y = 0;
	f.half_k = kern_size / 2;
	f.y = f.half_k;
	while (f.y < height - f.half_k)
	{
		f.x = f.half_k;
		f.pix_x = 0;
		while (f.x < width - f.half_k)
		{
			filter_section_d(img, img_big, kern_size, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
}

void	convo_down_filter_section(t_img *img, t_img *img_big, t_filter *f)
{
	int m;
	int n;
	
	f->red = 0;
	f->green = 0;
	f->blue = 0;
	f->j = -f->half_k - 1;
	n = 0;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		m = 0;
		while (++f->i <= f->half_k)
		{
			f->offset = (f->y + f->j) * img_big->line_len + \
			((f->x + f->i) * (img_big->bpp / 8));
			f->pixel = *(unsigned int *)(img_big->pixels_ptr + f->offset);
			f->red += (((f->pixel >> 16) & 0xFF) * f->kernel[m][n]);
			f->green += ((f->pixel >> 8) & 0xFF) * f->kernel[m][n];
			f->blue += (f->pixel & 0xFF) * f->kernel[m][n];
		m++;
		}
	n++;
	}
	f->avg_r = round(f->red / f->kern_sum);
	f->avg_g = round(f->green / f->kern_sum);
	f->avg_b = round(f->blue / f->kern_sum);
	my_pixel_put(f->pix_x, f->pix_y, img, ((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

int	gaussian_downsample(long width, long height, t_img *img, t_img *img_big, int kern_size, double sigma)
{
	t_filter	f;

	if (kern_size != 3 && kern_size != 5 && kern_size != 7)
		return (-1);
	if (make_gaussian(&f, kern_size, sigma) == -1)
		return (-1);
	f.pix_y = 0;
	f.half_k = kern_size / 2;
	f.y = f.half_k;
	while (f.y < height - f.half_k)
	{
		f.x = f.half_k;
		f.pix_x = 0;
		while (f.x < width - f.half_k)
		{
			convo_down_filter_section(img, img_big, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
	free_matrix_i(f.kernel, kern_size);
	return (0);
}

/* static void	filter_section_xl(t_img *img, unsigned int **pixels_xl, int kern_size, t_filter *f)
{
	f->red = 0;
	f->green = 0;
	f->blue = 0;
	f->j = -f->half_k - 1;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		while (++f->i <= f->half_k)
		{	
			f->pixel = pixels_xl[f->y + f->j][f->x + f->i];
			f->red += (f->pixel >> 16) & 0xFF;
			f->green += (f->pixel >> 8) & 0xFF;
			f->blue += f->pixel & 0xFF;
		}
	}
	f->avg_r = f->red / (kern_size * kern_size);
	f->avg_g = f->green / (kern_size * kern_size);
	f->avg_b = f->blue / (kern_size * kern_size);
	my_pixel_put(f->pix_x, f->pix_y, img, \
	((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

void	downsample_xl(int width, int height, t_img *img, unsigned int **pixels_xl, int kern_size)
{
	t_filter	f;
	
	f.pix_y = 0;
	f.half_k = kern_size / 2;
	f.y = f.half_k;
	while (f.y < height - f.half_k)
	{
		f.x = f.half_k;
		f.pix_x = 0;
		while (f.x < width - f.half_k)
		{
			filter_section_xl(img, pixels_xl, kern_size, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
} */

static void	filter_section_double(double **dst, double **src, int kern_size, t_filter *f)
{
	double sum;
	
	sum = 0.0;
	f->j = -f->half_k - 1;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		while (++f->i <= f->half_k)
		{	
			sum += src[f->y + f->j][f->x + f->i];
		}
	}
	sum /= (kern_size * kern_size);
	dst[f->pix_y][f->pix_x] = sum;
}

void	downsample_double(int width, int height, double **dst, double **src, int kern_size)
{
	t_filter	f;
	
	f.pix_y = 0;
	f.half_k = kern_size / 2;
	f.y = f.half_k;
	while (f.y < height - f.half_k)
	{
		f.x = f.half_k;
		f.pix_x = 0;
		while (f.x < width - f.half_k)
		{
			filter_section_double(dst, src, kern_size, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
}

static void	filter_section_xl(t_img *img, unsigned int **pixels_xl, int kern_size, t_filter *f, const int N)
{
	f->red = 0;//these are the sums of each square (kern X kern) section
	f->green = 0;
	f->blue = 0;
	f->alpha = 0;
	f->j = -1;
	while (++f->j < kern_size)
	{
		f->i = -1;
		while (++f->i < kern_size)
		{	
			f->pixel = pixels_xl[f->y + f->j][f->x + f->i];
			f->alpha += (f->pixel >> 24);
			f->red += (f->pixel >> 16) & 0xFF;
			f->green += (f->pixel >> 8) & 0xFF;
			f->blue += f->pixel & 0xFF;
		}
	}
	f->avg_r = f->red / N;
	f->avg_g = f->green / N;
	f->avg_b = f->blue / N;
	f->avg_t = f->alpha / N;

	my_pixel_put(f->pix_x, f->pix_y, img, \
	((f->avg_t << 24) | (f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

/* static void filter_section_xl(t_img *img, unsigned int **pixels_xl, int kern_size, t_filter *f, const int N)
{
    uint64_t sumA  = 0;
    uint64_t sumRA = 0;
    uint64_t sumGA = 0;
    uint64_t sumBA = 0;

    f->j = -1;
    while (++f->j < kern_size)
    {
        f->i = -1;
        while (++f->i < kern_size)
        {
            unsigned int px = pixels_xl[f->y + f->j][f->x + f->i];

            uint32_t a = (px >> 24);
            uint32_t r = (px >> 16) & 0xFF;
            uint32_t g = (px >>  8) & 0xFF;
            uint32_t b = px & 0xFF;

            sumA  += a;
            sumRA += (uint64_t)r * a;
            sumGA += (uint64_t)g * a;
            sumBA += (uint64_t)b * a;
        }
    }

    // Average alpha (0..255)
    uint32_t avgA = (uint32_t)((sumA + (N/2)) / (uint64_t)N);

    uint32_t outR = 0, outG = 0, outB = 0;

    if (sumA != 0)
    {
        // Compute average premultiplied channels: (sum(R*A)/N)
        // Then unpremultiply by avgA:
        //
        // outR = ( (sumRA/N) * 255 ) / (sumA/N)  == (sumRA * 255) / sumA
        //
        // Use rounding:
        outR = (uint32_t)((sumRA + (sumA/2)) / sumA);
        outG = (uint32_t)((sumGA + (sumA/2)) / sumA);
        outB = (uint32_t)((sumBA + (sumA/2)) / sumA);

        if (outR > 255) outR = 255;
        if (outG > 255) outG = 255;
        if (outB > 255) outB = 255;
    }
    my_pixel_put(f->pix_x, f->pix_y, img,
        ((avgA << 24) | (outR << 16) | (outG << 8) | outB));
} */


void	downsample_xl(int width, int height, t_img *img, unsigned int **pixels_xl, int kern_size)
{
	t_filter	f;
	
	const int N = SQ(kern_size);
	f.pix_y = 0;
	f.y = 0;
	while (f.y < height)
	{
		f.x = 0;
		f.pix_x = 0;
		while (f.x < width)
		{
			filter_section_xl(img, pixels_xl, kern_size, &f, N);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
}