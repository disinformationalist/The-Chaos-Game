#ifndef COLOR_SETS_H
# define COLOR_SETS_H

# include "chaos.h"
# include <stdint.h>//uint8_t

typedef struct s_3color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}	t_3color;

/*****FIRE*****/
# define DEEP_RED 0xFF0000
# define ORANGE 0xFFA500
# define COL_YELLOW 0xFFFF00
# define INTENSE_PINK 0xFF1493

/*****ICE*****/
# define DEEP_PURPLE 0x4B0082
# define ICY_BLUE 0x00BFFF
# define LIGHT_CYAN 0xE0FFFF
# define SILVER 0xC0C0C0

/*****CANDY*****/
# define BUBBLEGUM_PINK 0xFF69B4
# define LEMON_YELLOW 0xFFF44F
# define MINT_GREEN 0x98FB98
# define SKY_BLUE 0x87CEEB

/*****SLIME*****/
# define TOXIC_GREEN 0x7FFF00
# define RADIOACTIVE_YELLOW 0XDAAF20
# define ALIEN_BLUE 0x00BFFF 
# define SLIMY_PURPLE 0x800080

/***UNNAMED_1***/
# define COL_CYAN 0x00FFFF
# define MALACHITE_GREEN 0x1fd655
# define GOLD 0xD4AF37 
# define SINISTER_RED 0x8B0000

/***ORIGINAL***/

//COL_CYAN
# define COL_MAGENTA 0xFF00FF
//COL_YELLOW
# define COL_BLUE 0x0000FF

/***NEBULA***/

# define INDIGO_PURPLE 0x4B0082
# define DARK_ORCHID 0x9932CC
# define GOLD_2 0xFFD700
# define ORANGE_RED 0xFF4500

/***ETERNAL_SPRING***/
# define CHARTREUSE 0x7FFF00
# define HOT_PINK 0xFF69B4
# define DARK_TURQUOISE 0x00CED1
//GOLD_2

/***HIGH_SUMMER***/

# define TOMATO_RED 0xFF6347
//GOLD_2
# define LIME_GREEN 0x32CD32
# define STEEL_BLUE 0x4682B4

/***WAXING AUTUMN***/

# define CHOCOLATE 0xD2691E
# define ORANGE_RED 0xFF4500
//ORANGE
# define SADDLE_BROWN 0x8B4513

/***DEEP WINTER***/

//SKY_BLUE
//STEEL_BLUE
# define COL_WHITE 0xFFFFFF
# define SLATE_GRAY 0x708090

/***UNNAMED_2***/
# define EMERALD 0x50C878
# define COPPER 0xB87333
# define INDIGO 0x4B0082
# define GUNMETAL 0x483C32

/***UNNAMED_3***/

# define TEAL 0x008080
# define PLUM 0x8E4585
# define AMBER 0xFFBF00
# define CHARCOAL 0x36454F

/****GUN*****/
# define STEEL_GRAY 0x757575
//SILVER
# define BRONZE 0xCD7F32
# define GUNMETAL_2 0x2C3539

# define BLACK1 0x000000

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
extern const t_colors UNNAMED_2;
extern const t_colors UNNAMED_3;
extern const t_colors GUN;


#endif
