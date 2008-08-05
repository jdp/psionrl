#include <stdio.h>
#include <stdlib.h>
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

#define C_WHITE 		(TCOD_color_t){ 255, 255, 255 }
#define C_LIGHT_GREY	(TCOD_color_t){ 149, 149, 149 }
#define C_GREY			(TCOD_color_t){ 108, 108, 108 }
#define C_DARK_GREY		(TCOD_color_t){  68,  68,  68 }
#define C_BLACK			(TCOD_color_t){   0,   0,   0 }

#define C_MSG			C_LIGHT_GREY

/* Player structure */
typedef struct {
	char *name;
	int x, y;
	int fov_radius;
} player_t;

/* Item structure */
typedef struct {
	char *name;
	bool stackable;
	int count;
	void *next;
} item_t;

/* Item list structure */
typedef struct {
	item_t *head, *tail;
} item_list_t;

/* Valid tile types */
#define OK_TILES	"@#."
enum {
	TILE_EMPTY = 0,
	TILE_FLOOR,
	TILE_WALL,
	MAX_TILES
};

/* Tile structure */
typedef struct {
	int type;
	unsigned char glyph;
	TCOD_color_t fg_lit, fg_dark;
	bool transparent, walkable;
} tile_t;

/* Map structure */
typedef struct {
	int width, height;
	tile_t *tiles;
	TCOD_map_t fov;
} map_t;

/* Player prototypes */

int  init_player(void);
void rename_player(const char *);

/* Map prototypes */

map_t*  map_new(int, int);
map_t*  map_load_static(const char*);
void    map_fov_build(map_t*);
void    map_fov_do(map_t*, int, int);
tile_t* tile_at(map_t*, int, int);

/* Item prototypes */

item_list_t* item_list_new(void);
int          item_list_add(item_list_t*, item_t*);
item_t*      item_new(const char*, bool, int);

/* Gameplay prototypes */

void play(void);
void inventory(void);
bool is_walkable(map_t*, int, int);

/* Miscellaneous prototypes */

TCOD_key_t getkey(void);
char choice(char*);
void fgcolor(TCOD_color_t);
void bgcolor(TCOD_color_t);
void putstr(int, int, const char*);
void putch(int, int, unsigned char);
void clear(void);
void update(void);

/* Configuration prototypes */

int config(void);

/* Scripting prototypes */

int         init_scripting(void);
int         run_script(const char *);
const char* from_script(char*);

/* API prototypes */

void       init_api(void);
static int api_move_player(lua_State*);

/* External variables */

extern lua_State *L;

extern player_t *player;

extern item_list_t *inv;

extern const char *font_file;
extern int font_glyph_width;
extern int font_glyph_height;
extern int font_orientation;

extern const char *ui_caption;
extern int ui_width;
extern int ui_height;
extern int ui_viewport_x;
extern int ui_viewport_y;
extern int ui_viewport_width;
extern int ui_viewport_height;

extern tile_t tileset[];
