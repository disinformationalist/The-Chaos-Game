#include "chaos.h"

void	free_poly(int ***vertices, int i)
{
	while (--i >= 0)
		free((*vertices)[i]);
	free(*vertices);
	*vertices = NULL;	
}

void	malloc_points(int ***vertices, t_game *r)
{
	int i;
	int **verts2;

	verts2 = (int **)malloc(r->points * sizeof(int *));
	if (!verts2)
		clear_all(r);
	*vertices = (int **)malloc(r->points * sizeof(int *));
	if (!(*vertices))
	{
		free(verts2);
		clear_all(r);
	}
	i = -1;
	while (++i < r->points - r->jump_to_center)
	{
		(*vertices)[i] = (int *)malloc(2 * sizeof(int));
		(verts2)[i] = (int *)malloc(2 * sizeof(int));
		if (!(*vertices)[i] || !(verts2)[i])
		{
			free_poly(&verts2, i);
			free_poly(vertices, i);
			clear_all(r);
		}
	}
	r->vertices2 = verts2;
}

void	set_verts_sides_on(int ***vertices, t_game *r, double angle)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	while (i < r->points - 1)//set verts
	{
		(*vertices)[i][0] = r->width / 2 + (r->move_x + r->r * cos(angle * j - M_PI / 2  + r->rotate)) * r->zoom;
		(*vertices)[i][1] = r->height / 2 + (r->r * sin(angle * j - M_PI / 2  + r->rotate) - r->move_y) * r->zoom;
		i += 2;
		j++;
	}
	i = 1;
	while (i < r->points - 2)//set sides
	{
		(*vertices)[i][0] = ((*vertices)[i - 1][0] + (*vertices)[i + 1][0]) / 2;
		(*vertices)[i][1] = ((*vertices)[i - 1][1] + (*vertices)[i + 1][1]) / 2;
		i += 2;
	}
	(*vertices)[i][0] = ((*vertices)[i - 1][0] + (*vertices)[0][0]) / 2;
	(*vertices)[i][1] = ((*vertices)[i - 1][1] + (*vertices)[0][1]) / 2;
}

void	polygon(int ***vertices, t_game *r)
{
	double	angle;
	int		i;

	malloc_points(vertices, r);
	angle = 2 * M_PI / r->sides;
	/*center must be accessible for selection, but not for checking against normal rules, special center rules? if x, no center etc?*/
	r->center[0] = r->width / 2 + r->move_x * r->zoom;
	r->center[1] =  r->height / 2 - r->move_y * r->zoom;
	if (r->jump_to_sides)
		set_verts_sides_on(vertices, r, angle);
	else
	{
		i = -1;
		while (++i < r->points - r->jump_to_center)
		{
			(*vertices)[i][0] = r->width / 2 + (r->move_x + r->r * cos(angle * i - M_PI / 2 + r->rotate)) * r->zoom;
			(*vertices)[i][1] = r->height / 2 + (r->r * sin(angle * i - M_PI / 2 + r->rotate) - r->move_y) * r->zoom;
		}
	}
	//set v2 for color
	i = -1;
	double n = r->ratio_change;
	while (++i < r->points - r->jump_to_center)
	{
		(r->vertices2)[i][0] = (int)(n * ((double)(*vertices)[i][0] - r->move_x * r->zoom) + (double)r->width * ((1 - n) / 2)) + r->move_x * r->zoom;
		(r->vertices2)[i][1] = (int)(n * ((double)(*vertices)[i][1] + r->move_y * r->zoom) + (double)r->height * ((1 - n) / 2)) - r->move_y * r->zoom;
	}
}