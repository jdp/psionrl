#define _NO_EXTERNS
#include "psionrl.h"

/* Handle to the Lua interpreter */
lua_State *L = NULL;

/* The player */
player_t *player = NULL;

/* The inventory */
item_list_t *inv = NULL;

/* Font variables */
char *font_file = "res/font8x12.bmp";
int font_glyph_width = 8;
int font_glyph_height = 12;

/* Interface variables */
char *ui_caption = "PsionRL";
int ui_width = 40;
int ui_height = 25;
int ui_viewport_x = 1;
int ui_viewport_y = 3;
int ui_viewport_width = 38;
int ui_viewport_height = 19;
int ui_msg_x = 1;
int ui_msg_y = 1;
int ui_msg_width = 38;
int ui_msg_height = 1;

/* The tileset */
tile_t tileset[MAX_TILES] = {
	/* type, glyph, fg_lit, fg_dark, transparent, walkable */
	{ TILE_EMPTY, '?', C_DARK_GREY,  C_DARK_GREY, 0, 0 },
	{ TILE_FLOOR, '.', C_LIGHT_GREY, C_DARK_GREY, 1, 1 },
	{ TILE_WALL,  '#', C_GREY,       C_DARK_GREY, 0, 0 }
};
