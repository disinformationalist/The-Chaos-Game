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
	//-------try adding in xoro in place of rand

//-------------------------

//printf("the number of cores: %d\n", get_num_cores());
	if (ac == 2)
		init_god(&r, av);
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
	if (r.god)
		r.god = false;
	intermed(&r);
	mlx_loop(r.mlx_connect);
	return (0);
}
