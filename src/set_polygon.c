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
	int	i;
	int	**verts2;

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
	while (++i < r->points)// - r->jump_to_center)// center currently stored in last when on
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

void	set_verts_sides_on(int ***vertices, t_game *r, double angle, double factor)
{
	int		i;
	int		j;
	double	final_angle;
	double	shift_angle;

	i = 0;
	j = 0;
	shift_angle = M_PI / 2 + r->rotate;
	while (i < r->points - r->jump_to_center)//set verts
	{
		final_angle = angle * j - shift_angle;
		(*vertices)[i][0] = r->width / 2 + (r->move_x + factor * r->r * cos(final_angle)) * r->zoom;
		(*vertices)[i][1] = r->height / 2 + (factor * r->r * sin(final_angle) - r->move_y) * r->zoom;
		i += 2;
		j++;
	}
	i = 1;
	while (i < r->points - r->jump_to_center - 1)//set sides
	{
		(*vertices)[i][0] = ((*vertices)[i - 1][0] + (*vertices)[i + 1][0]) / 2;
		(*vertices)[i][1] = ((*vertices)[i - 1][1] + (*vertices)[i + 1][1]) / 2;
		i += 2;
	}
	(*vertices)[i][0] = ((*vertices)[i - 1][0] + (*vertices)[0][0]) / 2;
	(*vertices)[i][1] = ((*vertices)[i - 1][1] + (*vertices)[0][1]) / 2;
	if (r->jump_to_center)// center currently stored in last when on
	{
		(*vertices)[r->points - 1][0] = r->center[0];
		(*vertices)[r->points - 1][1] = r->center[1];
	}
}

void	set_verts(int ***vertices, t_game *r, double angle, double factor)
{
	int		i;
	double	final_angle;
	double	shift_angle;
	
	i = -1;
	shift_angle = M_PI / 2 + r->rotate;
	while (++i < r->points - r->jump_to_center)
	{
		final_angle = angle * i - shift_angle;
		(*vertices)[i][0] = r->width / 2 + (r->move_x + factor * r->r * cos(final_angle)) * r->zoom;
		(*vertices)[i][1] = r->height / 2 + (factor * r->r * sin(final_angle) - r->move_y) * r->zoom;
	}
	if (r->jump_to_center)// center currently stored in last when on
	{
		(*vertices)[i][0] = r->center[0];
		(*vertices)[i][1] = r->center[1];
	}
}

void	polygon(int ***vertices, t_game *r)
{
	double	angle;

	malloc_points(vertices, r);
	angle = 2 * M_PI / r->sides;
	/*center must be accessible for selection, but not for checking against normal rules, special center rules? if x, no center etc?*/
	r->center[0] = r->width / 2 + r->move_x * r->zoom;
	r->center[1] =  r->height / 2 - r->move_y * r->zoom;
	if (r->jump_to_sides)
		set_verts_sides_on(vertices, r, angle, 1);
	else
		set_verts(vertices, r, angle, 1);

	//set v2 for color
	//can also add x or y to move the color verts independently.... mouse click and drag verts?
	//pack vert shifts as whole pixel moves in x, y into 32bit int?//nah USE AN ARRAY WITH X,Y vals TO SHIFT BY
	//maybe have center as color vert when r->jump_to_center == true.// 2layer for center and side color activation on same keys, ?,`
	//in info maybe dont store center jump for shape rules.
	if (r->dist_ratio <= 1)// || r->dist_ratio == 2)
	{
		if (r->jump_to_sides)
			set_verts_sides_on(&r->vertices2, r, angle, r->vert_dist);
		else
			set_verts(&r->vertices2, r, angle, r->vert_dist);
	}
	else
	{
		if (r->jump_to_sides)
			set_verts_sides_on(&r->vertices2, r, angle, r->vert_dist * r->ratio_change);
		else
			set_verts(&r->vertices2, r, angle, r->vert_dist * r->ratio_change);
	}
}
		