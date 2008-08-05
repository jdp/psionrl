#include "psionrl.h"

/* Handle to the Lua interpreter */
lua_State *L;

int player_x;
int player_y;

/* Font variables */
const char *font_file = "res/font8x12.bmp";
int font_glyph_width = 8, font_glyph_height = 12;
int font_orientation = 0;

/* Interface variables */
const char *ui_caption = "PsionRL";
int ui_width = 40, ui_height = 25;
int ui_viewport_x = 1, ui_viewport_y = 3;
int ui_viewport_width = 38, ui_viewport_height = 19;

/* The tileset */
tile_t tileset[MAX_TILES] = {
	/* type, glyph, fg_lit, fg_dark, walkable, transparent */
	{ TILE_EMPTY, '?', C_DARK_GREY, C_DARK_GREY, 0, 0 },
	{ TILE_FLOOR, '.', C_GREY,		C_DARK_GREY, 1, 1 },
	{ TILE_WALL,  '#', C_DARK_GREY, C_DARK_GREY, 0, 0 }
};
