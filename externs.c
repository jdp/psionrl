#include "common.h"

/* Handle to the Lua interpreter */
lua_State *L;

/* Font control variables */
const char *font_file = "res/font8x12.bmp";
int font_glyph_width = 8, font_glyph_height = 12;
int font_orientation = 0;

/* Global color variables */
TCOD_color_t color_black = {0, 0, 0};
TCOD_color_t color_dark_grey = {99, 99, 99};
TCOD_color_t color_grey = {197, 197, 197};
TCOD_color_t color_white = {255, 255, 255};

/* Global interface variables */
const char *ui_caption = "A Roguelike Game";
int ui_width = 40, ui_height = 25;
int ui_viewport_x = 1, ui_viewport_y = 3;
int ui_viewport_width = 38, ui_viewport_height = 19;
