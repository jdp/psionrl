#include "psionrl.h"

/* Moved configuration to INI files, Lua is for scripting */
int config(void) {
	
	dictionary *cfg;
	if ((cfg = ini_open("config.ini")) == NULL) {
		return 0;
	}
	
	font_file = strdup(ini_getstr(cfg, "font:file", "res/font8x12.bmp"));
	font_glyph_width = ini_getint(cfg, "font:glyph_width", 8);
	font_glyph_height = ini_getint(cfg, "font:glyph_height", 12);
	font_orientation = ini_getbool(cfg, "font:orientation", 0);
	
	ui_caption = strdup(ini_getstr(cfg, "window:caption", "PsionRL"));
	ui_width = ini_getint(cfg, "window:width", 40);
	ui_height = ini_getint(cfg, "window:height", 25);
	
	ui_viewport_x = ini_getint(cfg, "viewport:x", 1);
	ui_viewport_y = ini_getint(cfg, "viewport:y", 3);
	ui_viewport_width = ini_getint(cfg, "viewport:width", 38);
	ui_viewport_height = ini_getint(cfg, "viewport:height", 19);
	
	ui_msg_x = ini_getint(cfg, "message:x", 1);
	ui_msg_y = ini_getint(cfg, "message:y", 1);
	ui_msg_width = ini_getint(cfg, "message:width", 38);
	ui_msg_height = ini_getint(cfg, "message:height", 19);
	
	iniparser_freedict(cfg);
	
	return 1;
}
