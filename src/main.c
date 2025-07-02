#include "chaos.h"

static void	init_god(t_game *r, char **av)
{
	char	*god;
	
	if (strncmp(av[1], "chaos", 5))//check if more protect needed, name a mandel chaos_3 and see...
	{
		ft_putstr_color("File parameter must be a chaos game image\n", BOLD_RED);
		exit (EXIT_FAILURE);
	}
	if (access(av[1], F_OK) != 0)
	{
		ft_putstr_color("Error: File does not exist\n", BOLD_RED);
		exit (EXIT_FAILURE);
	}
	ft_putstr_color("LOADING...\n", BOLD_BRIGHT_GREEN);
	r->god = true;
	god = read_png_text_metadata(av[1]);
	deserialize_game_data(r, god);
	free(god);	
}

static void	init_god2(t_game *r, char **av)
{
	char	*god;
	
	if (strncmp(av[3], "chaos", 5))
	{
		ft_putstr_color("File parameter must be a chaos game image\n", BOLD_RED);
		exit (EXIT_FAILURE);
	}
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
	
	double old_hyp = sqrt(((r->width * r->width) + (r->height * r->height)) / 4);
	//double max_change = r->max_distance - old_hyp;
	
	r->width = ft_atoi(av[1]);
	r->height = ft_atoi(av[2]);
	
	double new_hyp = sqrt(((r->width * r->width) + (r->height * r->height)) / 4);
	r->max_distance = (new_hyp / old_hyp) * (r->max_distance);// + max_change);


	double incs = round(4 * ((double)r->iters / (((double)r->r * r->zoom) * ((double)r->r * r->zoom) * sqrt(3)) - 1));
	
	r->r = (r->height / 2 - r->height / 10);
	r->iters = round(((double)r->r * r->zoom) * ((double)r->r * r->zoom) * sqrt(3) * (1 + incs / 4));
	if (r->supersample)
		r->iters *= r->s_kernel * r->s_kernel;

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
	r->width_orig = r->width;//maybe rework with w_super, h_super
	r->height_orig = r->height;
	r->supersample = false;
	r_init(r);
}

int	main(int ac, char **av)
{
	t_game	r;

//printf("the number of cores: %d\n", get_num_cores());
	if (ac == 2)
		init_god(&r, av);
	else if (ac == 4)
		init_god2(&r, av);
	else
		not_god(&r, ac, av);
	game_init(&r);
	r.width_orig = r.width;
	r.height_orig = r.height;
	if (r.supersample)// for s_sample god
	{
		r.width = r.width * r.s_kernel;
		r.height = r.height * r.s_kernel;
	}
	if (ac == 4)
		r.r = r.height / 2 - r.height / 10;
	if (r.god)
		r.god = false;
	intermed(&r);
	mlx_loop(r.mlx_connect);
	return (0);
}
