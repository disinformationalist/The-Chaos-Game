#include "chaos.h"


static inline uint8_t to_srgb(uint8_t linear_val)
{
	float lin = linear_val / 255.0f;
	float srgb;

	if (lin <= 0.0031308f)
		srgb = 12.92f * lin;
	else
		srgb = 1.055f * powf(lin, 1.0f / 2.4f) - 0.055f;

	int corrected = (int)(srgb * 255.0f + 0.5f);
	if (corrected < 0) corrected = 0;
	if (corrected > 255) corrected = 255;
	return (uint8_t)corrected;
}


static inline uint8_t to_gray(uint8_t r, uint8_t g, uint8_t b)
{
    // integer approx of 0.299*r + 0.587*g + 0.114*b
    return (uint8_t)((77 * r + 150 * g + 29 * b + 128) >> 8);
}

//set pixels for all types


int set_buffer_png_pixels(t_png_io *png_img, unsigned char *pixels, int height, int width)//pass the unsigned char * here
{
    png_byte	*row;
	size_t		idx;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint8_t		a;

    for (png_img->y = 0; png_img->y < height; png_img->y++)
    {
        row = png_malloc(png_img->png_ptr, (size_t)width * png_img->pixel_size);
        if (!row)
        {
            perror("Error: export_png row malloc failure\n");
            clean_memory(png_img, png_img->y, true);
            return -1;
        }
        png_img->row_pointers[png_img->y] = row;

        for (png_img->x = 0; png_img->x < width; png_img->x++)
        {
            /* get_pixel(&png_img->temp_pixel, img, png_img->x, png_img->y);

            uint8_t r = png_img->temp_pixel.red;
            uint8_t g = png_img->temp_pixel.green;
            uint8_t b = png_img->temp_pixel.blue;
            uint8_t a = png_img->temp_pixel.alpha; */

			idx = ((size_t)png_img->y * (size_t)width
				+ (size_t)png_img->x) * 4;

			a = pixels[idx + 0];
			r = pixels[idx + 1];
			g = pixels[idx + 2];
			b = pixels[idx + 3];

            switch (png_img->color_type)
            {
                case PNG_COLOR_TYPE_RGB:
                    *row++ = r;
                    *row++ = g;
                    *row++ = b;
                    break;

                case PNG_COLOR_TYPE_RGBA:
                    *row++ = r;
                    *row++ = g;
                    *row++ = b;
                    *row++ = a;
                    break;

                case PNG_COLOR_TYPE_GRAY:
                {
                    uint8_t y8 = to_gray(r, g, b);
                    *row++ = y8;
                    break;
                }

                case PNG_COLOR_TYPE_GRAY_ALPHA:
                {
                    uint8_t y8 = to_gray(r, g, b);
                    *row++ = y8;
                    *row++ = a;
                    break;
                }
				//not used
                case PNG_COLOR_TYPE_PALETTE:
                    // You must write palette *indices* (0..N-1)
                    // Needs a palette mapping step (see below)
                    //*row++ = (png_byte)png_img->temp_index; // compute per pixel
					return error_3(png_img, "Palette PNG not supported");
                    break;

                default:
                    // Should never happen if IHDR is set correctly
                    clean_memory(png_img, png_img->y + 1, true);
                    return -1;
            }
        }
    }
    return 0;
}


static inline void	write_img(t_png_io *png_img)
{
	int i;
	int len;

	if (png_img->text != NULL)
	{
		len = 0;
		while (png_img->text[len].text != NULL)
			len++;
		png_set_text(png_img->png_ptr, png_img->info, png_img->text, len);
		i = -1;
		while (++i < len)
		{
			free(png_img->text[i].text);
			free(png_img->text[i].key);
		}
		free(png_img->text[i].key);
		free(png_img->text);
	}
	png_init_io(png_img->png_ptr, png_img->fp);
	png_set_rows(png_img->png_ptr, png_img->info, png_img->row_pointers);
	png_write_png(png_img->png_ptr, png_img->info, \
	PNG_TRANSFORM_IDENTITY, NULL);
}

//-----consider creating a check for t(a) channel and create rgb or rgba
//will have to adjust png_set_ihdr, png color type

int	export_buffer_png(const char *filename, unsigned char *pixels, int width, int height, png_text *text, t_pixel_format fmt)
{
	t_png_io	*png_img;

	//t_pixel p;
	//get_pixel(&p, img, width / 2, height / 2);
	//printf("A=%u R=%u G=%u B=%u\n", p.alpha, p.red, p.green, p.blue);
	png_img = (t_png_io *)malloc(sizeof(t_png_io));
	if (!png_img)
		return (-1);
	init_vars(png_img, fmt);
	if (png_img->pixel_size < 0)
    	return (error_1(png_img, "Invalid/unsupported pixel format\n"));
	if (init_png_structs(png_img, filename) == -1)
		return (-1);
	png_set_IHDR(png_img->png_ptr, png_img->info, width, height, \
	png_img->depth, png_img->color_type, PNG_INTERLACE_NONE, \
	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_set_sRGB(png_img->png_ptr, png_img->info, PNG_sRGB_INTENT_PERCEPTUAL);
	png_img->row_pointers = png_malloc(png_img->png_ptr, height \
	* sizeof(png_byte *));
	if (!png_img->row_pointers)
	{
		png_destroy_write_struct(&png_img->png_ptr, &png_img->info);
		return (error_1(png_img, "Error: row ptrs malloc failure\n"));
	}
	if (set_buffer_png_pixels(png_img, pixels, height, width) == -1)
		return (-1);
	png_img->text = text;
	write_img(png_img);
	clean_memory(png_img, height, true);
	return (0);
}

void	export_buffer(t_game *r, t_export *full_tiled)
{
	char		*name;
	png_text	*text = NULL;

	name = get_nxt_name("tiled_");
	if (!name)
		close_screen(r);	
	text = build_chaos_text(r);//add a tiled flag later
	if (export_buffer_png(name, full_tiled->pixels, r->width_orig, r->height_orig, text, RGBA) == -1)
	{
		free(name);
		close_screen(r);
	}
	printf( BOLD_BRIGHT_MAGENTA"EXPORT "BOLD_GREEN);
	printf("%s", name);
	printf(BOLD_BRIGHT_MAGENTA" COMPLETE\n"RESET);
	free(name);
}