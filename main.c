#include "psionrl.h"

void splash(void) {
	clear();
	fgcolor(C_WHITE);
	putstr(1, 1, "Aphelion");
	fgcolor(C_MSG);
	putstr(1, 2, "The Poppenkast");
	putstr(1, 23, "beam '08");
	update();
}

void menu(void) {
	clear();
	fgcolor(C_MSG);
	putstr(1, 1, "This will be a menu.");
	update();
}

int main(int argc, char *argv[])
{
	/* Open up the Lua environment */
	if (!init_scripting()) {
		fprintf(stderr, "Error starting Lua environment.\n");
		return(1);
	}
	
	/* Initialize the API */
	init_api();
	
	/* Configure the game */
	if (!config()) {
		fprintf(stderr, "Configuration error.\n");
		return(1);
	}
	
	/* Set the game font */
	TCOD_console_set_custom_font(font_file, font_glyph_width,
								 font_glyph_height, 16, 16, font_orientation,
								 C_BLACK);
	
	/* Open up the console window */
	TCOD_console_init_root(ui_width, ui_height, ui_caption, false);
	
	/* Why is this necessary? */
	/*
	splash();
	getkey();
	menu();
	getkey();
	*/
	
	/* Enter the main game loop */
	play();
	
	/* Clean & successful exit */
	return(0);
}
