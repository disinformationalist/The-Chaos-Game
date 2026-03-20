#include "chaos.h"

char **read_png_text_metadata(const char *filename) 
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) 
	{
        perror("fopen");
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
	{
        fclose(fp);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
	{
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png)))
	{
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    png_textp text_ptr;
    int num_text, len;
	char **data;

	
    if (png_get_text(png, info, &text_ptr, &num_text) > 0)
	{
	//printf("num_txt: %d\n", num_text);
	len = num_text - 31;
	data = (char **)malloc((len + 1) * sizeof(char *));
	data[len] = NULL;
        for (int i = 0; i < num_text; i++) 
		{
            if (!strcmp(text_ptr[i].key, "GOD")) 
			{
               // printf("here\n");
				data[0] = strdup(text_ptr[i].text);
				//exporter does not exp the null term line, get the rest of the lines after first god str
				int j = 0;
				while (++j < len)
					data[j] = strdup(text_ptr[i + j].text);
				break;
            }
        }
    }
	else
		return (NULL);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return (data);
}

void deserialize_game_data(t_game *game, char **data) 
{
    char rules_str[31];  // 30 characters for rules + null terminator
    int disinfo_1_temp, disinfo_2_temp, jump_to_sides_temp, jump_to_center_temp;
    int mouse_zoom_temp, supersample_temp, layer_temp, curved_temp;
	unsigned int color_1, color_2, color_3, color_4, background;
	double vert;
	int parsed;
	char *gdata = data[0];
	//s_kernel -> s_kernal for back compat
    parsed = sscanf(gdata,
           "move_x=%lf;move_y=%lf;rotate=%lf;zoom=%lf;max_distance=%lf;dist_ratio=%lf;ratio_start=%lf;"
           "rules=%30s;disinfo_1=%d;disinfo_2=%d;jump_to_sides=%d;jump_to_center=%d;mouse_zoom=%d;"
           "supersample=%d;layer=%d;curved=%d;r=%ld;iters=%ld;sides=%d;points=%d;center=%d,%d;"
           "s_kernel=%d;width=%d;height=%d;mode=%d;function_id=%d;colors=%x,%x,%x,%x;"
		   "color_depth=%d;color_op=%d;color_rot=%d;col_shift_x=%d;col_shift_y=%d;iters_change=%d;background=%x;vert_dist=%lf;",
           &game->move_x, &game->move_y, &game->rotate, &game->zoom, &game->max_distance, &game->dist_ratio, &game->ratio_start,
           rules_str, &disinfo_1_temp, &disinfo_2_temp, &jump_to_sides_temp, &jump_to_center_temp, &mouse_zoom_temp,
           &supersample_temp, &layer_temp, &curved_temp, &game->r, &game->iters, &game->sides, &game->points,
           &game->center[0], &game->center[1], &game->s_kernel, &game->width, &game->height, &game->mode, &game->function_id,
		   &color_1, &color_2, &color_3, &color_4, &game->color_depth, &game->color_op, &game->color_rot, &game->col_shift_x, 
		  &game->col_shift_y, &game->iters_change, &background, &vert);
	//for vert_dist back compat
	if (parsed == 38)  // No vert_dist
		game->vert_dist = 1;
	else if (parsed == 39)
		game->vert_dist = vert;
	else
		fprintf(stderr, "deserialize_game_data: unexpected input format (parsed %d items)\n", parsed);
	
		
	//for pre background back compat, must use to replace vert_dist back compat for older images
	/* if (parsed == 38)  // No background provided
		game->colors.background = 0x000000;  // Default background
	else if (parsed == 39)
		game->colors.background = background;
	else
		fprintf(stderr, "deserialize_game_data: unexpected input format (parsed %d items)\n", parsed); */
	game->colors.background = background;
	
    // Convert rules from '0'/'1' characters to boolean values
    for (int i = 0; i < 30; i++) 
        game->rules[i] = rules_str[i] == '1';
    
	// Convert other boolean values
    game->disinfo_1 = disinfo_1_temp;
    game->disinfo_2 = disinfo_2_temp;
    game->jump_to_sides = jump_to_sides_temp;
    game->jump_to_center = jump_to_center_temp;
    game->mouse_zoom = mouse_zoom_temp;
    game->supersample = supersample_temp;
	double rad_times_zoom = game->r * game->zoom;
	game->area_factor = .25 * (double)game->sides * sin(2 * M_PI / (double)game->sides);
	if (game->dist_ratio <= .5)
		game->iters = .5 * (8 + game->iters_change) * SQ(game->dist_ratio * rad_times_zoom) * game->area_factor;
	else if (game->dist_ratio < 1.0 && game->dist_ratio > .5)
		game->iters = .5 * (8 + game->iters_change) * SQ((1 - game->dist_ratio) * rad_times_zoom) * game->area_factor;
	else //(game->dist_ratio > 1)
		game->iters = .5 * (8 + game->iters_change) * SQ(rad_times_zoom * game->ratio_change * (game->dist_ratio - 1)) * game->area_factor;
    game->layer = layer_temp;
    game->curved = curved_temp;

	//colors
	game->colors.color_1 = color_1;
    game->colors.color_2 = color_2;
    game->colors.color_3 = color_3;
    game->colors.color_4 = color_4;

	/*** DESERIAL ADDITIONAL ROWS***/
 	if (data[1])
	{
		//printf("str: %s\n", data[1]);
		parsed = sscanf(data[1],
           "num_colors=%d;saturation=%lf;lightness=%lf;base_hue=%d;trans=%d;freq=%lf;spiral=%lf;", 
		   &game->wheel->num_colors, &game->wheel->saturation, &game->wheel->lightness, &game->wheel->base_hue, &game->wheel->trans, &game->wheel->freq, &game->wheel->spiral);
		//printf("num: %d\n", game->wheel->base_hue);
	}
	else
		init_wheel(game);

	//set up for extra col stuff, ghost, etc.	
	if (data[2])
	{
		printf("str: %s\n", data[2]);
		parsed = sscanf(data[2],
           "jump_to_center_col=%d;jump_to_sides_col=%d;ghost2=%d;", 
		   &game->jump_to_center_col, &game->jump_to_sides_col, &game->ghost2);
	}
	else
	{
		game->jump_to_center_col = 0;
		game->jump_to_sides_col = 0; 
		game->ghost2 = 0;
	}
		//}
}
