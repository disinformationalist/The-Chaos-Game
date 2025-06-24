#include "chaos.h"
/*how to structure

--fixed dist_ratio
--changing ratio
>function based
>> trig
>> other
>non function based, plain div, etc.
*/
//PROB FUNC BASED VERT SELECTION
/*some good fun	

		return (r->ratio_start * 2 * sin(cos(d /(r->r * r->zoom)) * cos(d /(r->r * r->zoom)) * M_PI));


*/
//---------- also make argument of trig fts adjustable..
/*nice effect but how to zoom?
if (v % 2 != 0)
		return (r->dist_ratio);	
*/
//------------------

//JUMPS TO CENTER AND MAYBE SIDES, CENTER FIXED VALUE JUMP, NOT RATIO?
/*some fts
cos
cos^2
sin^2
a * cos^2 + b * sin^2
etc...*/

// to reduce grain, try smoothing the super image, then downsample

	//return (r->dist_ratio + (r->dist_ratio - r->ratio_start) * cos(log(d /(r->r * r->zoom)) * M_PI));// try applying function dependent shifts.. this ones not great,watev
	//double shift = (r->dist_ratio - r->ratio_start) * (double)r->i / (double)r->iters;
	//double shift = cos(d /(r->r) * M_PI);
	//(void)v;


double submode(t_game *r, int v, double ft_result)
{
	if (r->mode == 2)
		return (ft_result);	
	else if (r->mode == 3)
	{
		if (v % 2 == 0)
			return (r->dist_ratio);
		else
			return (ft_result);
	}
	else
	{
		if (v % 2 != 0)
			return (r->dist_ratio);
		else
			return (ft_result);
	}
}
	//	return (r->ratio_start * 2 * sin(log(d /(r->r * r->zoom))) * sin(log(d /(r->r * r->zoom)) * M_PI));
	//	return (submode(r, v, r->ratio_start * 2 * cos(cos(log(d /(r->r * r->zoom)) * M_PI)))); eh not bad..

double	apply_function(t_game *r, double d, int **vertices, int v)//, generic function change with keys recieves distance to vertex and applies ft.
{
	(void)vertices;
	if (r->function_id == 1)
		return (submode(r, v, r->ratio_start * 2 * sin(d /(r->r * r->zoom) * M_PI)));
	else if (r->function_id == 2)
		return (submode(r, v, r->ratio_start * 2 * cos(d /(r->r * r->zoom) * M_PI)));
	else if (r->function_id == 3)
		return (submode(r, v, r->ratio_start * 2 * cos(cos(d /(r->r * r->zoom)) * M_PI)));
	else if (r->function_id == 4)
		return (submode(r, v, r->ratio_start * 2 * sin(sin(d /(r->r * r->zoom)) * M_PI)));
	else if (r->function_id == 5)
		return(submode(r, v, r->ratio_start * 2 * cos(log(d /(r->r * r->zoom)) * M_PI)));
	else if (r->function_id == 6)
		return(submode(r, v, r->ratio_start * 2 * cos(d / r->r * M_PI) * cos(d / (r->r *r->zoom) * M_PI)));
	else if (r->function_id == 7)
		return (submode(r, v, r->ratio_start * 2 * sin(cos(d /(r->r * r->zoom)) * cos(d /(r->r * r->zoom)) * M_PI)));				
	else if (r->function_id == 8)
		return (submode(r, v, r->ratio_start * 2 * cos(sin(d /(r->r * r->zoom)) * sin(d /(r->r * r->zoom)) * M_PI)));	 //circles about each vertex		
	else if (r->function_id == 9)
		return(submode(r, v, r->ratio_start * 2 * sin(log(d /(r->r * r->zoom)) * M_PI)));//very good
	else if (r->function_id == 10)
		return (submode(r, v, r->ratio_start * 2 * cos(log(d /(r->r * r->zoom)) * log(d /(r->r * r->zoom)) * M_PI)));			
	else
		return (r->dist_ratio);

			//return (r->dist_ratio + (r->dist_ratio - r->ratio_start) * cos(log(d /(r->r * r->zoom)) * M_PI));
		
			
			//return (r->dist_ratio);
//return (r->ratio_start + shift);
//return (r->ratio_start + shift);// + shift);//try diff function to apply here..
		

	//double val = (pow(cos(d), 2));

	
	//return(r->ratio_start * (2  * cos(x * M_PI) + 2 * sin(x * M_PI)));//some intersting stuff here.. try diff combos
	
	//return(r->ratio_start * ( 2 * cos(x) * cos (x) + 3 * sin(x) * sin(x)));//some intersting stuff here.. try diff combos
}

double get_distance(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	chaos_game_apply_function(t_game *r, int **vertices, double x, double y)//try to app function and change ratio here
{
	int		v;
	double	distance_to_vert;
	double curr_ratio;
	Xoro128 *rng = &r->rng;
	//int		bv;
	
	r->i = 1;
	polygon(&vertices, r);
	
	while (++r->i <= r->iters + 1)
	{
		v = r_loop(r, rng);
		if (v == -1)
			break ;
		information(r, v, r->i);
		
		/* if (r->sides > 1) doesnt seem to make difference
			bv = (r->i - 2) % r->rv_len;
		else
			bv = 0;
		distance_to_vert = get_distance(x, y, vertices[r->rv[bv]][0], vertices[r->rv[bv]][1]);//previous vert
		 */
		if (r->jump_to_center && (v == r->points - 1))
			distance_to_vert = get_distance(x, y, r->center[0], r->center[1]);// current vert set as center...//can try not applying function if center
		else
			distance_to_vert = get_distance(x, y, vertices[v][0], vertices[v][1]);// current vert
		curr_ratio = apply_function(r, distance_to_vert, vertices, v);// get ratio based on function
		if (r->jump_to_center && (v == r->points - 1))//changed to handle center
		{
			x = x + (r->center[0] - x) * curr_ratio; //try fixed for center
			y = y + (r->center[1] - y) * curr_ratio;
		}
		else
		{	
			x = x + (vertices[v][0] - x) * curr_ratio; //normal
			y = y + (vertices[v][1] - y) * curr_ratio;
		}	 
		if ((r->i > 10) && (x > 0 && x < r->width) && (y > 0 && y < r->height))
			color_option(r, v, x, y, vertices);
	}
	free_poly(&vertices, r->points - r->jump_to_center);
}

/* double	get_ratio(double r_start, double r_end, long iter, long iters)//this is working but not clear how its different? seemss to be none
{
	double progress = (double)iter / iters;
	return (r_start + progress * (r_end - r_start));
} */

/* void	chaos_game_curved_pro(t_game *r, int **vertices, double x, double y)//I THINK THIS DOES NOTHING DIFFERENT REALLY
{
	int		v;

	double ratio_end = r->dist_ratio;
	//double shift = (ratio_end - r->ratio_start) / (double)r->iters;
	
	r->dist_ratio = r->ratio_start;
	r->i = 1;
	polygon(&vertices, r);
	while (++r->i <= r->iters + 2)
	{
		v = r_loop(r);
		if (v == -1)
			break ;
		information(r, v, r->i);
		if (r->jump_to_center && (v == r->points - 1))//changed to handle center
		{
			x = x + (r->center[0] - x) * r->dist_ratio;
			y = y + (r->center[1] - y) * r->dist_ratio;
		}
		else
		{	
		x = x + (vertices[v][0] - x) * r->dist_ratio; //normal
		y = y + (vertices[v][1] - y) * r->dist_ratio;
		}	 
	// 	if (r->i == r->iters / 2)//testingstuff
	//		shift *= 1.5;// 
		//r->dist_ratio += shift;
		r->dist_ratio = get_ratio(r->ratio_start, ratio_end, r->i, r->iters);
		//r->dist_ratio += shift;
			//r->dist_ratio = r->dist_ratio
		if ((r->i > 10) && (x > 0 && x < r->width) && (y > 0 && y < r->height))
			color_option(r, v, x, y);
	}
	free_poly(&vertices, r->points - r->jump_to_center);
	r->dist_ratio = ratio_end;
}// different verts get different ratios, try based on functions, probabilities etc. set up a game like the fern, */