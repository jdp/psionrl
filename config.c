#define _NO_CONFIG_EXTERNS
#include "psionrl.h"

/* Configure the game */
int config(void) {
	/* Run the configuration script */
	if (!run_script("lua/config.lua")) {
		fprintf(stderr, "Couldn't run configuration script.\n");
		return(0);
	}
	
	/* Set up the custom game font */
	if (from_script("font_file") != NULL)
		font_file = from_script("font_file");
	if (from_script("font_glyph_width") != NULL)
		font_glyph_width = atoi(from_script("font_glyph_width"));
	if (from_script("font_glyph_height") != NULL)
		font_glyph_height = atoi(from_script("font_glyph_height"));
	if (from_script("font_orientation") != NULL)
		font_orientation = atoi(from_script("font_orientation"));
		
	/* Set up the user interface */
	if (from_script("ui_caption") != NULL)
		ui_caption = from_script("ui_caption");
	if (from_script("ui_width") != NULL)
		ui_width = atoi(from_script("ui_width"));
	if (from_script("ui_height") != NULL)
		ui_height = atoi(from_script("ui_height"));
	if (from_script("ui_viewport_x") != NULL)
		ui_viewport_x = atoi(from_script("ui_viewport_x"));
	if (from_script("ui_viewport_y") != NULL)
		ui_viewport_y = atoi(from_script("ui_viewport_y"));
	if (from_script("ui_viewport_width") != NULL)
		ui_viewport_width = atoi(from_script("ui_viewport_width"));
	if (from_script("ui_viewport_height") != NULL)
		ui_viewport_height = atoi(from_script("ui_viewport_height"));
		
	/* Load static content */
	if (!run_script("lua/static.lua")) {
		fprintf(stderr, "Couldn't load static content.\n");
		return(0);
	}
	
	return(1);
}
