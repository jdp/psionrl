#include "psionrl.h"

int config(void) {

	dictionary *parser;

	parser = iniparser_load(INI_FILE);

	font_file = ini_getstr(parser, "font:file", "res/font8x12.bmp");
	font_glyph_width = ini_getint(parser, "font:glyph_width", 8);
	font_glyph_height = ini_getint(parser, "font:glyph_height", 12);

	ui_caption = strdup(ini_getstr(parser, "window:caption", "PsionRL"));
	ui_width = ini_getint(parser, "window:width", 40);
	ui_height = ini_getint(parser, "window:height", 25);

	ui_viewport_x = ini_getint(parser, "viewport:x", 1);
	ui_viewport_y = ini_getint(parser, "viewport:y", 3);
	ui_viewport_width = ini_getint(parser, "viewport:width", 38);
	ui_viewport_height = ini_getint(parser, "viewport:height", 19);

	ui_msg_x = ini_getint(parser, "message:x", 1);
	ui_msg_y = ini_getint(parser, "message:y", 1);
	ui_msg_width = ini_getint(parser, "message:width", 38);
	ui_msg_height = ini_getint(parser, "message:height", 1);

	iniparser_freedict(parser);

	return 1;
}
