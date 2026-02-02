#include "chaos.h"

static void	init_god(t_game *r, char **av)
{
	char	*god;
	
	/* if (strncmp(av[1], "chaos", 5))//see below, change this.
	{
		ft_putstr_color("File parameter must be a chaos game image\n", BOLD_RED);
		exit (EXIT_FAILURE);
	} */
	if (access(av[1], F_OK) != 0)
	{
		ft_putstr_color("Error: File does not exist\n", BOLD_RED);
		exit (EXIT_FAILURE);
	}
	r->god = true;
	god = read_png_text_metadata(av[1]);
	deserialize_game_data(r, god);
	free(god);
	r->area_factor = .25 * (double)r->sides * sin(2 * M_PI / (double)r->sides);
}

//used when opening with a new size

static void	init_god2(t_game *r, char **av)
{
	char	*god;
	
	/* if (strncmp(av[3], "chaos", 5))//change this, get rid of and use a metadata check instead.
	{
		ft_putstr_color("File parameter must be a chaos game image\n", BOLD_RED);
		exit (EXIT_FAILURE);
	} */
	if (access(av[3], F_OK) != 0)
	{
		ft_putstr_color("Error: File does not exist\n", BOLD_RED);
		exit (EXIT_FAILURE);
	}
	ft_putstr_color("LOADING...\n", BOLD_BRIGHT_GREEN);
	r->god = true;
	god = read_png_text_metadata(av[3]);
	deserialize_game_data(r, god);
	free(god);

	r->area_factor = .25 * (double)r->sides * sin(2 * M_PI / (double)r->sides);

	//pre
	double old_hyp = sqrt((double)(SQ(r->width) + SQ(r->height)) * .25);
	r->col_shift_x = ft_round((double)r->col_shift_x / ((double)r->height * .125));
	r->col_shift_y = ft_round((double)r->col_shift_y / ((double)r->height * .125));

	r->move_x *= (r->zoom * 15) / (double)r->width;
	r->move_y *= (r->zoom * 15) / (double)r->height;
	//change dim
	r->width = ft_atoi(av[1]);
	r->height = ft_atoi(av[2]);
	
	//post
	double new_hyp = sqrt((double)(SQ(r->width) + SQ(r->height)) * .25);
	r->max_distance = (new_hyp / old_hyp) * r->max_distance;
	r->start_maxd = new_hyp;
	//r->start_maxd = (new_hyp / old_hyp) * r->start_maxd;

	r->col_shift_x = ft_round((double)r->col_shift_x * ((double)r->height * .125));
	r->col_shift_y = ft_round((double)r->col_shift_y * ((double)r->height * .125));
	r->move_x *= (double)(r->width) / (15 * r->zoom);
	r->move_y *= (double)(r->height) / (15 * r->zoom);
	double incs = round(4 * ((double)r->iters / ((SQ((double)r->r * r->zoom) * r->area_factor) - 1)));
	
	r->r = r->height * .4;
	r->iters = (long)(round(SQ((double)r->r * r->zoom) * r->area_factor * (1 + incs * .25)));
	if (r->supersample)
		r->iters *= SQ(r->s_kernel);

}

static void	not_god(t_game *r, int ac, char **av)
{
	// check av[1] and av[2] are pos numbers with set maximum
	if (ac != 3)
	{
		ft_putstr_color("Syntax: ./chaos <width> <height>"
			"\n", "\033[31m");
		exit(EXIT_SUCCESS);
	}
	r->god = false;
	r->s_kernel = 3;
	r->width = ft_atoi(av[1]);
	r->height = ft_atoi(av[2]);
	r->width_orig = r->width;
	r->height_orig = r->height;
	r->supersample = false;
	r_init(r);
}

int	main(int ac, char **av)
{
	t_game	r;
	bool	flag = 0;

	//printf("the number of cores: %d\n", get_num_cores());
	
	if (ac == 2)
	{
		init_god(&r, av);
		flag = 1;
	}
	else if (ac == 4)
		init_god2(&r, av);
	else
		not_god(&r, ac, av);
	game_init(&r);
	r.width_orig = r.width;
	r.height_orig = r.height;
	if (ac == 4)
		r.r = r.height * .4;
	if (r.god)
		r.god = false;
	r.layer = 0;

	if (flag)//load the super img without rebuilding it. ONLY WITH GOD1, NO RESIZE
	{
		t_img	*temp;

		if (r.supersample)
		{
			r.width *= r.s_kernel;
			r.height *= r.s_kernel;
		}
		temp = import_png(r.mlx_connect, av[1], &r.width_orig, &r.height_orig);
		img_cpy(&r.img, temp, r.width_orig, r.height_orig);
		mlx_destroy_image(r.mlx_connect, temp->img_ptr);
		free(temp);
		mlx_put_image_to_window(r.mlx_connect, r.mlx_win, r.img.img_ptr, 0, 0);
	}
	//for keeping super off on open for god2
	else if (r.supersample)
	{	
		set_supersampler_off(&r);
		r.supersample = 0;
	}
	if (!flag)
		render(&r);
	mlx_loop(r.mlx_connect);
	return (0);
}
