#ifndef _EXTERNS_H

#define _EXTERNS_H

/* Handle to the Lua interpreter */
extern lua_State *L;

extern int player_x;
extern int player_y;

/* Globalize the font variables */
extern const char *font_file;
extern int font_glyph_width;
extern int font_glyph_height;
extern int font_orientation;

/* Globalize the color variables */
extern TCOD_color_t color_white;
extern TCOD_color_t color_grey;
extern TCOD_color_t color_dark_grey;
extern TCOD_color_t color_black;

/* Globalize the interface variables */
extern const char *ui_caption;
extern int ui_width;
extern int ui_height;
extern int ui_viewport_x;
extern int ui_viewport_y;
extern int ui_viewport_width;
extern int ui_viewport_height;

#endif
