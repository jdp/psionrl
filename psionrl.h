#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libtcod/libtcod.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

/* Useful macros */
#define PUTCH(x, y, c)	TCOD_console_put_char(NULL, x, y, c, TCOD_BKGND_SET)
#define GETCH()			TCOD_console_wait_for_keypress(1)

/* Color definitions */
#define C_WHITE 	(TCOD_color_t){255, 255, 255}
#define C_GREY		(TCOD_color_t){197, 197, 197}
#define C_DARK_GREY	(TCOD_color_t){99, 99, 99}
#define C_BLACK		(TCOD_color_t){0, 0, 0}

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
	char glyph;
	TCOD_color_t fg_lit, fg_dark;
	bool walkable, transparent;
} tile_t;

/* Map structure */
typedef struct {
	int width, height;
	tile_t *tiles;
	TCOD_map_t fov;
} map_t;

/* Map prototypes */

map_t*	map_new(int, int);
map_t*	map_load_static(const char*);
void	map_build_fov(map_t*);
void	map_dig(map_t*, TCOD_map_t);
tile_t*	tile_at(map_t*, int, int);

/* Item prototypes */

item_list_t* item_list_new(void);
int			 item_list_add(item_list_t*, item_t*);
item_t*		 item_new(const char*, bool, int);

/* Gameplay prototypes */

void play(void);
void inventory(void);
bool is_walkable(map_t*, int, int);

/* Miscellaneous prototypes */

char choice(char*);
void fgcolor(TCOD_color_t);
void bgcolor(TCOD_color_t);
void write(int, int, const char*);
void clear(void);
void update(void);

/* Configuration prototypes */

int config(void);

/* Scripting prototypes */

int			init_scripting(void);
int			run_script(const char *);
const char*	from_script(char*);

/* API prototypes */

void	   init_api(void);
static int api_move_player(lua_State*);

/* External variables */

extern lua_State *L;

extern int player_x;
extern int player_y;

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
