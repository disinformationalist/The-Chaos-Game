#include "chaos.h"
//still needs color data...
char *serialize_game_data(t_game *r)
{
    //int width;
	//int height;
	
    char *buffer = malloc(1024);
    if (!buffer)
        clear_all(r);

    char bool_str[2] = {0, 0}; // store single bool as '0' or '1'
    size_t offset = 0;

    /***Serialize data***/
    offset += snprintf(buffer + offset, 1024 - offset, "move_x=%f;", r->move_x);
    offset += snprintf(buffer + offset, 1024 - offset, "move_y=%f;", r->move_y);
    offset += snprintf(buffer + offset, 1024 - offset, "rotate=%f;", r->rotate);
    offset += snprintf(buffer + offset, 1024 - offset, "zoom=%f;", r->zoom);
    offset += snprintf(buffer + offset, 1024 - offset, "max_distance=%f;", r->max_distance);
    offset += snprintf(buffer + offset, 1024 - offset, "dist_ratio=%f;", r->dist_ratio);
    offset += snprintf(buffer + offset, 1024 - offset, "ratio_start=%f;", r->ratio_start);

    // Serialize rules array
    offset += snprintf(buffer + offset, 1024 - offset, "rules=");
    for (int j = 0; j < 30; j++)
	{
        bool_str[0] = r->rules[j] ? '1' : '0';
        offset += snprintf(buffer + offset, 1024 - offset, "%s", bool_str);
    }
    offset += snprintf(buffer + offset, 1024 - offset, ";");

    // Serialize remaining members
    offset += snprintf(buffer + offset, 1024 - offset, "disinfo_1=%d;", r->disinfo_1);
    offset += snprintf(buffer + offset, 1024 - offset, "disinfo_2=%d;", r->disinfo_2);
    offset += snprintf(buffer + offset, 1024 - offset, "jump_to_sides=%d;", r->jump_to_sides);
    offset += snprintf(buffer + offset, 1024 - offset, "jump_to_center=%d;", r->jump_to_center);
    offset += snprintf(buffer + offset, 1024 - offset, "mouse_zoom=%d;", r->mouse_zoom);
    offset += snprintf(buffer + offset, 1024 - offset, "supersample=%d;", r->supersample);
    offset += snprintf(buffer + offset, 1024 - offset, "layer=%d;", r->layer);
    offset += snprintf(buffer + offset, 1024 - offset, "curved=%d;", r->curved);
    offset += snprintf(buffer + offset, 1024 - offset, "r=%ld;", r->r);
    offset += snprintf(buffer + offset, 1024 - offset, "iters=%ld;", r->iters);
    offset += snprintf(buffer + offset, 1024 - offset, "sides=%d;", r->sides);
    offset += snprintf(buffer + offset, 1024 - offset, "points=%d;", r->points);
    offset += snprintf(buffer + offset, 1024 - offset, "center=%d,%d;", r->center[0], r->center[1]);
    offset += snprintf(buffer + offset, 1024 - offset, "s_kernel=%d;", r->s_kernel);
    
		
	offset += snprintf(buffer + offset, 1024 - offset, "width=%d;", r->width_orig);
    offset += snprintf(buffer + offset, 1024 - offset, "height=%d;", r->height_orig);
    
	offset += snprintf(buffer + offset, 1024 - offset, "mode=%d;", r->mode);
    offset += snprintf(buffer + offset, 1024 - offset, "function_id=%d;", r->function_id);

	//recent added must turn off in init for back compat, some need the first line here..
 	offset += snprintf(buffer + offset, 1024 - offset, "colors=%x,%x,%x,%x;", r->colors.color_1, r->colors.color_2, r->colors.color_3, r->colors.color_4);
	offset += snprintf(buffer + offset, 1024 - offset, "color_depth=%d;", r->color_depth);
    offset += snprintf(buffer + offset, 1024 - offset, "color_op=%d;", r->color_op);

    offset += snprintf(buffer + offset, 1024 - offset, "color_rot=%d;", r->color_rot);
    offset += snprintf(buffer + offset, 1024 - offset, "col_shift_x=%d;", r->col_shift_x);
    offset += snprintf(buffer + offset, 1024 - offset, "col_shift_y=%d;", r->col_shift_y);
    offset += snprintf(buffer + offset, 1024 - offset, "iters_change=%d;", r->iters_change);

    offset += snprintf(buffer + offset, 1024 - offset, "background=%x;", r->colors.background);

	
    offset += snprintf(buffer + offset, 1024 - offset, "vert_dist=%f;", r->vert_dist);






	return (buffer);
}
	/* if (r->supersample)
	{
		width = r->width / r->s_kernel
		height = r->height / r->s_kernel;
	}
	else
	{
		width = r->width;
		height = r->height;
	} */