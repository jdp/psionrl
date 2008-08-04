/* Include all header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libtcod/libtcod.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
#include "externs.h"
#include "config.h"
#include "misc.h"
#include "item.h"
#include "map.h"
#include "game.h"
#include "script.h"
#include "api.h"

#define GAME_NAME	"Aphelion"

/* Color palette */
/*
TCOD_color_t color_black		= {0,		0,		0};
TCOD_color_t color_grey			= {157,		157,	157};
TCOD_color_t color_white		= {255,		255,	255};
TCOD_color_t color_red			= {190,		38,		51};
TCOD_color_t color_pink			= {224,		111,	139};
TCOD_color_t color_darkbrown	= {73,		60,		43};
TCOD_color_t color_brown		= {164,		100,	34};
TCOD_color_t color_orange		= {235,		137,	49};
TCOD_color_t color_yellow		= {247,		226,	107};
TCOD_color_t color_darkgreen	= {47,		72,		78};
TCOD_color_t color_green		= {68,		137,	26};
TCOD_color_t color_lightgreen	= {163,		206,	39};
TCOD_color_t color_slate		= {27,		38,		50};
TCOD_color_t color_darkblue		= {0,		87,		132};
TCOD_color_t color_blue			= {49,		162,	242};
TCOD_color_t color_lightblue	= {178,		220,	239};
*/
