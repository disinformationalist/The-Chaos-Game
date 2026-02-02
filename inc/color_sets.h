#ifndef COLOR_SETS_H
# define COLOR_SETS_H

# include "chaos.h"
# include <stdint.h>//uint8_t

typedef struct s_4color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	t;

}	t_4color;

/*****FIRE*****/
# define DEEP_RED 0xFFFF0000
# define ORANGE 0xFFFFA500
# define COL_YELLOW 0xFFFFFF00
# define INTENSE_PINK 0xFFFF1493

/*****ICE*****/
# define DEEP_PURPLE 0xFF4B0082
# define ICY_BLUE 0xFF00BFFF
# define LIGHT_CYAN 0xFFE0FFFF
# define SILVER 0xFFC0C0C0

/*****CANDY*****/
# define BUBBLEGUM_PINK 0xFFFF69B4
# define LEMON_YELLOW 0xFFFFF44F
# define MINT_GREEN 0xFF98FB98
# define SKY_BLUE 0xFF87CEEB

/*****SLIME*****/
# define TOXIC_GREEN 0xFF7FFF00
# define RADIOACTIVE_YELLOW 0XFFDAAF20
# define ALIEN_BLUE 0xFF00BFFF 
# define SLIMY_PURPLE 0xFF800080

/***UNNAMED_1***/
# define COL_CYAN 0xFF00FFFF
# define MALACHITE_GREEN 0xFF1fd655
# define GOLD 0xFFD4AF37 
# define SINISTER_RED 0xFF8B0000

/***ORIGINAL***/

//COL_CYAN
# define COL_MAGENTA 0xFFFF00FF
//COL_YELLOW
# define COL_BLUE 0xFF0000FF

/***NEBULA***/

# define INDIGO_PURPLE 0xFF4B0082
# define DARK_ORCHID 0xFF9932CC
# define GOLD_2 0xFFFFD700
# define ORANGE_RED 0xFFFF4500

/***ETERNAL_SPRING***/
# define CHARTREUSE 0xFF7FFF00
# define HOT_PINK 0xFFFF69B4
# define DARK_TURQUOISE 0xFF00CED1
//GOLD_2

/***HIGH_SUMMER***/

# define TOMATO_RED 0xFFFF6347
//GOLD_2
# define LIME_GREEN 0xFF32CD32
# define STEEL_BLUE 0xFF4682B4

/***WAXING AUTUMN***/

# define CHOCOLATE 0xFFD2691E
# define ORANGE_RED 0xFFFF4500
//ORANGE
# define SADDLE_BROWN 0xFF8B4513

/***DEEP WINTER***/

//SKY_BLUE
//STEEL_BLUE
# define COL_WHITE 0xFFFFFFFF
# define SLATE_GRAY 0xFF708090

/***UNNAMED_2***/
# define EMERALD 0xFF50C878
# define COPPER 0xFFB87333
# define INDIGO 0xFF4B0082
# define GUNMETAL 0xFF483C32

/***PSYCHEDELIC***/
//COL_CYAN
# define NAME1 0xFF5948a6
# define GOLD2 0xFFffb400 
# define NAME2 0xFFffc800

/***UNNAMED_3***/

# define TEAL 0xFF008080
# define PLUM 0xFF8E4585
# define AMBER 0xFFFFBF00
# define CHARCOAL 0xFF36454F

/****GUN*****/
# define STEEL_GRAY 0xFF757575
//SILVER
# define BRONZE 0xFFCD7F32
# define GUNMETAL_2 0xFF2C3539

# define BLACK1 0xFF000000

typedef struct s_colors
{
	int				color_1;
	int				color_2;
	int				color_3;
	int				color_4;
	unsigned int	background;
}	t_colors;

extern const t_colors FIRE;
extern const t_colors ICE;
extern const t_colors CANDY;
extern const t_colors SLIME;
extern const t_colors UNNAMED_1;
extern const t_colors ORIGINAL;
extern const t_colors NEBULA;
extern const t_colors ETERNAL_SPRING;

extern const t_colors DUSK;

extern const t_colors HIGH_SUMMER;
extern const t_colors WAXING_AUTUMN;
extern const t_colors DEEP_WINTER;
extern const t_colors PSYCHEDELIC;
extern const t_colors UNNAMED_3;
extern const t_colors GUN;


#endif
