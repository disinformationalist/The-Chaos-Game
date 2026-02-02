#include "chaos.h"

//destroy malloced img made with create img or imported

void	destroy_img(t_img *img, void *con)
{
	mlx_destroy_image(con, img->img_ptr);
	free(img);
}

void	destroy_controls(void *con, t_control *controls)
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
	if (controls->nav)
		destroy_img(controls->nav, con);
	if (controls->w_nav)
		destroy_img(controls->w_nav, con);
	free(controls);
}

void	null_init2(t_control *new)
{
	new->color_con = NULL;
	new->r = NULL;
	new->g = NULL;
	new->b = NULL;
	
	new->nav = NULL;
	new->w_nav = NULL;

	new->col_num = 1;
	new->dragging = false;
	new->on_nav = false;
	new->on_w_nav = false;
	new->nav_open = false;
	new->nav_w_open = false;
}


t_knob		init_knob(t_img *png, int cx, int cy, int w, int h)
{
	t_knob	knob;
	
	knob.img = png;
	knob.cx = cx;
	knob.cy = cy;
	knob.angle = 0;
	knob.posx = cx;
	knob.posy = cy;
	knob.w = w;
	knob.h = h;
	
	return (knob);
}

void		init_knobs(t_control *new, t_knobs *knobs)
{
	knobs->posx = init_knob(new->g, 122, 190, 8, 8);
	knobs->posy = init_knob(new->b, 262, 190, 8, 8);
	knobs->rot = init_knob(new->r, 191, 310, 8, 8);
	knobs->zoom = init_knob(new->s, 191, 230, 8, 8);
	knobs->w_zoom = init_knob(new->s, 191, 230, 8, 8);
	knobs->w_posx = init_knob(new->g, 122, 190, 8, 8);
	knobs->w_posy = init_knob(new->b, 262, 190, 8, 8);
	knobs->w_rot = init_knob(new->r, 191, 310, 8, 8);
	knobs->iter = init_knob(new->s, 192, 424, 8, 8);
}

t_control	*make_controls(void *m_con)
{
	t_control	*new;

	new = (t_control *)malloc(sizeof(t_control));
	if (!new)
		return (NULL);
	null_init2(new);
	
	if (!(new->color_con = import_png(m_con, \
	"gui_imgs/currcon.png", &new->m_width, &new->m_height)))
		goto fail;

	if (!(new->r = import_png(m_con, \
	"gui_imgs/red.png", &new->k_width, &new->k_height)))
		goto fail;

	if (!(new->g = import_png(m_con, \
	"gui_imgs/green.png", &new->k_width, &new->k_height)))
		goto fail;

	if (!(new->b = import_png(m_con, \
	"gui_imgs/blue.png", &new->k_width, &new->k_height)))
		goto fail;

	if (!(new->s = import_png(m_con, \
	"gui_imgs/silver.png", &new->k_width, &new->k_height)))
		goto fail;
	
	if (!(new->nav = import_png(m_con, \
	"gui_imgs/nav.png", &new->nav_w, &new->nav_h)))
		goto fail;

	if (!(new->w_nav = import_png(m_con, \
	"gui_imgs/w_nav.png", &new->nav_w, &new->nav_h)))
		goto fail;

	init_knobs(new, &new->knobs);
	return (new);

fail:
    destroy_controls(m_con, new);   // free images + struct
    perror("make_controls() has failed. \n");
	return NULL;

}