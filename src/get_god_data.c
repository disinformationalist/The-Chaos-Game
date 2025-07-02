#include "chaos.h"

char *read_png_text_metadata(const char *filename) 
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
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
    int num_text;
    char *metadata = NULL;

    if (png_get_text(png, info, &text_ptr, &num_text) > 0)
	{
        for (int i = 0; i < num_text; i++) {
            if (strcmp(text_ptr[i].key, "GOD") == 0) {
                metadata = strdup(text_ptr[i].text);
                break;
            }
        }
    }
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return (metadata);
}

void deserialize_game_data(t_game *game, const char *data) 
{
    char rules_str[31];  // 30 characters for rules + null terminator
    int disinfo_1_temp, disinfo_2_temp, jump_to_sides_temp, jump_to_center_temp;
    int mouse_zoom_temp, supersample_temp, layer_temp, curved_temp;
	unsigned int color_1, color_2, color_3, color_4, background;

	int parsed;
	//s_kernel -> s_kernal for back compat
    parsed = sscanf(data,
           "move_x=%lf;move_y=%lf;rotate=%lf;zoom=%lf;max_distance=%lf;dist_ratio=%lf;ratio_start=%lf;"
           "rules=%30s;disinfo_1=%d;disinfo_2=%d;jump_to_sides=%d;jump_to_center=%d;mouse_zoom=%d;"
           "supersample=%d;layer=%d;curved=%d;r=%ld;iters=%ld;sides=%d;points=%d;center=%d,%d;"
           "s_kernel=%d;width=%d;height=%d;mode=%d;function_id=%d;colors=%x,%x,%x,%x;"
		   "color_depth=%d;color_op=%d;color_rot=%d;col_shift_x=%d;col_shift_y=%d;iters_change=%d;background=%x",
           &game->move_x, &game->move_y, &game->rotate, &game->zoom, &game->max_distance, &game->dist_ratio, &game->ratio_start,
           rules_str, &disinfo_1_temp, &disinfo_2_temp, &jump_to_sides_temp, &jump_to_center_temp, &mouse_zoom_temp,
           &supersample_temp, &layer_temp, &curved_temp, &game->r, &game->iters, &game->sides, &game->points,
           &game->center[0], &game->center[1], &game->s_kernel, &game->width, &game->height, &game->mode, &game->function_id, 
		   &color_1, &color_2, &color_3, &color_4, &game->color_depth, &game->color_op, &game->color_rot, &game->col_shift_x, 
		  &game->col_shift_y, &game->iters_change, &background);
	//for pre background back compat
	if (parsed == 37)  // No background provided
		 game->colors.background = 0x000000;  // Default background
	else if (parsed == 38)
		game->colors.background = background;
	else
		fprintf(stderr, "deserialize_game_data: unexpected input format (parsed %d items)\n", parsed);
		  
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
    game->layer = layer_temp;
    game->curved = curved_temp;

	//newstuff
	game->colors.color_1 = color_1;
    game->colors.color_2 = color_2;
    game->colors.color_3 = color_3;
    game->colors.color_4 = color_4;
}