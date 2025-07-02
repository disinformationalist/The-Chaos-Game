#include "chaos.h"

//destroy malloced img made with create img or imported

void	destroy_img(t_img *img, void *con)
{
	mlx_destroy_image(con, img->img_ptr);
	free(img);
}

void	free_controls(void *con, t_control *controls)
{
	if (controls->color_con)
		destroy_img(controls->color_con, con);
	if (controls->r)
		destroy_img(controls->r, con);
	if (controls->g)
		destroy_img(controls->g, con);
	if (controls->b)
		destroy_img(controls->b, con);
	if (controls->s)
		destroy_img(controls->s, con);
	if (controls->ct)
		destroy_img(controls->ct, con);
	free(controls);
}

void	null_init2(t_control *new)
{
	new->color_con = NULL;
	new->r = NULL;
	new->g = NULL;
	new->b = NULL;
	new->ct = NULL;
}

t_control	*make_controls(void *m_con)
{
	t_control	*new;

	new = (t_control *)malloc(sizeof(t_control));
	if (!new)
		return (NULL);
	null_init2(new);
	new->color_con = import_png(m_con, \
	"gui_imgs/color_con.png", &new->m_width, &new->m_height);
	if (!new->color_con)
		return (NULL);
	new->r = import_png(m_con, \
	"gui_imgs/red.png", &new->k_width, &new->k_height);
	if (!new->r)
		return (NULL);
	new->g = import_png(m_con, \
	"gui_imgs/green.png", &new->k_width, &new->k_height);
	if (!new->g)
		return (NULL);
	new->b = import_png(m_con, \
	"gui_imgs/blue.png", &new->k_width, &new->k_height);
	if (!new->b)
		return (NULL);

	new->s = import_png(m_con, \
	"gui_imgs/silver.png", &new->k_width, &new->k_height);
	if (!new->s)
		return (NULL);
		
	new->ct = import_png(m_con, \
	"gui_imgs/col_track.png", &new->ct_width, &new->ct_height);
	if (!new->ct)
		return (NULL);
	new->col_num = 1;
	new->dragging = false;
	
	return (new);
}