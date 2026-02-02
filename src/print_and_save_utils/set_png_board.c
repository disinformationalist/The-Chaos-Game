#include "chaos.h"

char	*get_boolstr(bool x)
{
	char *s;

	if (x)
		s = "[ON]";
	else
		s = "OFF ";
	return (s);
}

char	*safe_ft_strdup(const char *s)
{
	char *dup;

	dup = ft_strdup(s);
	if (!dup)
	{
		perror("ft_strdup malloc failure.");
		exit(EXIT_FAILURE);
	}
	return (dup);
}

void	set_pretext(png_text **text)
{
	int	i;

	i = -1;
	while (++i < 33)
		(*text)[i].compression = PNG_TEXT_COMPRESSION_NONE;
	(*text)[0].key = safe_ft_strdup("FRACTALS_BY_DISINFORMATIONALIST");
	(*text)[0].text = safe_ft_strdup("+-------------------------------------------+");
	(*text)[1].key = safe_ft_strdup("_NEVER");
	(*text)[1].text = safe_ft_strdup("|       * * *  THE CHAOS GAME * * *         |");
	(*text)[2].key = safe_ft_strdup("_GONNA");
	(*text)[2].text = safe_ft_strdup("+-------------------------------------------+");
	(*text)[3].key = safe_ft_strdup("__GIVE");
	(*text)[3].text = safe_ft_strdup("           +--------------------+           ");
	(*text)[4].key = safe_ft_strdup("___YOU");
	(*text)[4].text = safe_ft_strdup("+----------| RULES AND SETTINGS |-----------+");
	(*text)[5].key = safe_ft_strdup("____UP");
	(*text)[5].text = safe_ft_strdup("|          +--------------------+           |");
}

static inline int	set_spacelen(char* param, int case_num)
{
	int len;

	if (case_num == 1)
		len = 10 - ft_strlen(param);
	else if (case_num == 2)
		len = 12 - ft_strlen(param);
	else if (case_num == 3)
		len = 14 - ft_strlen(param);
	else if (case_num == 4)
		len = 12 - ft_strlen(param);
	else if (case_num == 5)
		len = 12 - ft_strlen(param);
	else if (case_num == 6)
		len = 12 - ft_strlen(param);
	else if (case_num == 7)
		len = 8 - ft_strlen(param);
	else if (case_num == 8)
		len = 12 - ft_strlen(param);
	else if (case_num == 9)
		len = 8 - ft_strlen(param);
	else if (case_num == 10)
		len = 12 - ft_strlen(param);
	else
		len = 0;
	return (len);
}

char	*set_spaces(char *param, int case_num)
{
	int		i;
	int 	len;
	char	*spaces_str;

	len = set_spacelen(param, case_num);
	if (case_num % 2 == 0)
		spaces_str = (char *)malloc((len + 2) * sizeof(char));
	else
		spaces_str = (char *)malloc((len + 1) * sizeof(char));
	if (!spaces_str)
		return (NULL);
	i = -1;
	while (++i < len)
		spaces_str[i] = 32;
	if (case_num % 2 == 0)
	{
		spaces_str[i] = '|';
		spaces_str[i + 1] = '\0';
	}
	else
		spaces_str[i] = '\0';
	return (spaces_str);
}

void	set_bottom(png_text **text, t_game *r)
{
	char	ratio_start[10];
	char 	dist_ratio[10];
	char 	*sides;
	char 	*spaces;
	char	*spaces_2;
	char	*iters;
	char	zoom[15];
	char	move_x[15];
	char	move_y[15];
	char	*s_kern;

	gcvt(r->ratio_start, 5, ratio_start);
	if (r->ratio_start > -0.001 && r->ratio_start < 0.001)
		r->ratio_start = 0;
	gcvt(r->dist_ratio, 5, dist_ratio);
	if (r->dist_ratio > -0.001 && r->dist_ratio < 0.001)
		r->dist_ratio = 0;

	gcvt(r->zoom, 9, zoom);
	gcvt(r->move_x, 9, move_x);
	gcvt(r->move_y, 9, move_y);

	iters = ft_itoa(r->iters);//needs to be conveted from long use snprintf
	sides = ft_itoa(r->sides);
	s_kern = ft_itoa(r->s_kernel);

	spaces = set_spaces(iters, 3);
	spaces_2 = set_spaces(zoom, 4);
	(*text)[24].key = safe_ft_strdup("ITERS_AND_ZOOM");
	(*text)[24].text = ft_strjoin_n(6, "|    ITERS: ", iters, spaces, "ZOOM: ", zoom, spaces_2);
	free(spaces);
	free(spaces_2);
	free(iters);
	spaces = set_spaces(move_x, 5);
	spaces_2 = set_spaces(move_y, 6);
	(*text)[25].key = safe_ft_strdup("MOVE_X_AND_MOVE_Y");
	(*text)[25].text = ft_strjoin_n(6, "|   MOVE X: ", move_x, spaces, "MOVE Y: ", move_y, spaces_2);
	free(spaces);
	free(spaces_2);
	spaces = set_spaces(ratio_start, 7);
	spaces_2 = set_spaces(s_kern, 8);
	(*text)[26].key = safe_ft_strdup("__NEVER");
	(*text)[26].text = ft_strjoin_n(6, "|   RATIO START: ", ratio_start, spaces, "SUPER: ", s_kern, spaces_2);
	free(spaces);
	free(spaces_2);
	free(s_kern);
	spaces = set_spaces(dist_ratio, 9);
	spaces_2 = set_spaces(sides, 10);
	(*text)[27].key = safe_ft_strdup("__GONNA");
	(*text)[27].text = ft_strjoin_n(6, "|    DIST START: ", dist_ratio, spaces, "SIDES: ", sides, spaces_2);
	free(sides);
	free(spaces);
	free(spaces_2);
	(*text)[28].key = safe_ft_strdup("____LET");
	(*text)[28].text = safe_ft_strdup("|             BROUGHT TO YOU BY             |");
	(*text)[29].key = safe_ft_strdup("____YOU");
	(*text)[29].text = safe_ft_strdup("|            DISINFORMATIONALIST            |");
	(*text)[30].key = safe_ft_strdup("___DOWN");
	(*text)[30].text = safe_ft_strdup("+-------------------------------------------+");
	//SET GOD MODE-----------
	(*text)[31].key = safe_ft_strdup("GOD");
	(*text)[31].text = serialize_game_data(r);
	//------------------------

	(*text)[32].key = safe_ft_strdup("NULL TERMINATION");
	(*text)[32].text = NULL;
}

void set_type(png_text **text, t_game *r)
{
	char	*mode;
	char	*ft_str;
	char	*spaces;
	char	*spaces_2;

	mode = mode_str(r);
	spaces = set_spaces(mode, 1);
	if (r->function_id == 0)
		ft_str = "NONE";
	else
		ft_str = ft_itoa(r->function_id);
	spaces_2 = set_spaces(ft_str, 2);
	(*text)[23].key = safe_ft_strdup("MODE_AND_FUNCTION");
	(*text)[23].text = ft_strjoin_n(6, "|     MODE: ", mode, spaces, "FUNCTION: ", ft_str, spaces_2); 
	free(spaces);
	free(spaces_2);
	if (r->function_id != 0)
		free(ft_str);
}

png_text	*build_chaos_text(t_game *r)
{
	png_text 	*text;
	int			i;
	char		*num;
	char		*num_2;
	char		*bool1;
	char		*bool2;
	char  		*str;

	text = (png_text *)malloc(33 * sizeof(png_text));
	if (!text)
	{
		perror("png text malloc failure");
		close_screen(r);
	}
	set_pretext(&text);
	i = 0;
	while (++i < 16)
	{
		num = ft_itoa(i);
		if (!num)
		{
			//free_all_txt(text);
			close_screen(r);
		}
		num_2 = ft_itoa(i + 15);
		bool1 = get_boolstr(r->rules[i - 1]);
		bool2 = get_boolstr(r->rules[i + 14]);
		if (i < 10)
			str = ft_strjoin_n(9, "|      RULE ", num, ": ", bool1,"       RULE ", num_2, ": ", bool2, "     |");
		else
			str = ft_strjoin_n(9, "|     RULE ", num, ": ", bool1,"       RULE ", num_2, ": ", bool2, "     |");
		if (i < 10)
			text[i + 5].key = ft_strjoin_n(4, "RULES___", num, "__AND__", num_2);
		else
			text[i + 5].key = ft_strjoin_n(4, "RULES__", num, "__AND__", num_2);
		text[i + 5].text = safe_ft_strdup(str);
		free(str);
		free(num);
		free(num_2);
	}

	bool1 = get_boolstr(r->disinfo_1);
	bool2 = get_boolstr(r->disinfo_2);
	text[21].key = safe_ft_strdup("DISINFORMATION_RULES");
	text[21].text = ft_strjoin_n(5, "|   DISINFO 1: ", bool1, "     DISINFO 2: ", bool2, "     |");

	//TO center and sides...
	bool1 = get_boolstr(r->jump_to_center);
	bool2 = get_boolstr(r->jump_to_sides);
	text[22].key = safe_ft_strdup("TO_CENTER_AND_TO_SIDES");
	text[22].text = ft_strjoin_n(5, "|   TO CENTER: ", bool1, "      TO SIDES: ", bool2, "     |");
	set_type(&text, r);
	set_bottom(&text, r);

	return (text);
}
