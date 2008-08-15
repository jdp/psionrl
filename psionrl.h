#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <iniparser.h>
#include <libtcod/libtcod.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

/* Useful macros */
#define GETCH() getkey()

/* Color definitions */

/*
enum {
	_C_WHITE = 0,
	_C_LIGHT_GREY,
	_C_GREY,
	_C_DARK_GREY,
	_C_BLACK,
	MAX_COLORS
};

TCOD_color_t color_table[MAX_COLORS] = {
	{ 255, 255,	255 }, // white
	{ 149, 149,	149 }, // light grey
	{ 108, 108,	108 }, // grey
	{  68,  68,  68 }, // dark grey
	{   0,   0,   0 }  // black
};
*/

typedef TCOD_color_t color;

#define C_WHITE 		(color){255, 255, 255}
#define C_LIGHT_GREY	(color){149, 149, 149}
#define C_GREY			(color){108, 108, 108}
#define C_DARK_GREY		(color){ 68,  68,  68}
#define C_BLACK			(color){  0,   0,   0}
#define C_BROWN         (color){111,  79,  37}
#define C_MSG			C_LIGHT_GREY

/* Nothing worse than long function names */
#define ini_open(f)          iniparser_load(f)
#define ini_getstr(f, v, d)	 iniparser_getstring(f, v, d)
#define ini_getint(f, v, d)  iniparser_getint(f, v, d)
#define ini_getbool(f, v, d) iniparser_getboolean(f, v, d)

/* Player structure */

typedef struct {
	char* name;
	int   x, y;
	int   fov_radius;
} player_t;

/* Item stuff */

typedef struct {
	char* name;
	bool  stackable;
	int   count;
	void* next;
} item_t;

typedef struct {
	item_t *head, *tail;
} item_list_t;

/* Tile stuff */

#define OK_TILES	"@#."

enum {
	TILE_EMPTY = 0,
	TILE_FLOOR,
	TILE_WALL,
	MAX_TILES
};

/* Tile structure */

typedef struct {
	int           type;
	unsigned char glyph;
	TCOD_color_t  fg_lit, fg_dark;
	bool          transparent, walkable;
} tile_t;

/* Map structure */

typedef struct {
	int        width, height;
	tile_t*    tiles;
	TCOD_map_t fov;
} map_t;

/* Configuration prototypes */

int config(void);

/* Player prototypes */

int  init_player(void);
void rename_player(const char *);
void move_player(int, int);
void blink_player(map_t*);

/* Map prototypes */

map_t*  map_new(int, int);
map_t*  map_load_static(const char*);
void    map_fov_build(map_t*);
void    map_fov_do(map_t*, int, int);
void    map_fill_rect(map_t*, tile_t, int, int, int, int);
tile_t* tile_at(map_t*, int, int);

void    generate_forest(map_t*);
void    generate_cave(map_t*);
void    generate_dungeon(map_t*);

/* Item prototypes */

item_list_t* item_list_new(void);
int          item_list_add(item_list_t*, item_t*);
item_t*      item_new(const char*, bool, int);

/* Gameplay prototypes */

void play(void);
void inventory(void);
void character(void);
bool quit(void);
bool is_walkable(map_t*, int, int);

/* Miscellaneous prototypes */

TCOD_key_t getkey(void);
char       choice(char*);
void       fgcolor(TCOD_color_t);
void       bgcolor(TCOD_color_t);
void       putstr(int, int, char*);
void       putstrf(int, int, char*, ...);
void       putch(int, int, unsigned char);
void       clear(void);
void       update(void);

/* Scripting prototypes */

int   init_scripting(void);
int   run_script(char*);
char* from_script(char*);

/* API prototypes */

void       init_api(void);
static int api_move_player(lua_State*);

/* External variables */

#ifndef _NO_EXTERNS

extern lua_State *L;

extern player_t *player;

extern item_list_t *inv;

extern char* font_file;
extern int   font_glyph_width;
extern int   font_glyph_height;
extern int   font_orientation;

extern char* ui_caption;
extern int   ui_width;
extern int   ui_height;
extern int   ui_viewport_x;
extern int   ui_viewport_y;
extern int   ui_viewport_width;
extern int   ui_viewport_height;
extern int   ui_msg_x;
extern int   ui_msg_y;
extern int   ui_msg_width;
extern int   ui_msg_height;

extern tile_t tileset[];

#endif
