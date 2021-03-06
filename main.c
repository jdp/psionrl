#include "psionrl.h"

void splash(void) {
	clear(NULL);
	fgcolor(NULL, C_WHITE);
	putstr(NULL, 1, 1, "PsionRL");
	fgcolor(NULL, C_MSG);
	putstr(NULL, 1, 2, "The Poppenkast");
	putstr(NULL, 1, 23, "beam '08-09");
	update();
}

void menu(void) {
	clear(NULL);
	fgcolor(NULL, C_MSG);
	putstr(NULL, 1, 1, "This will be a menu.");
	update();
}

int main(int argc, char *argv[]) {
	/* Open up the Lua environment */
	if (!init_scripting()) {
		fprintf(stderr, "Error starting Lua environment\n");
		return 1;
	}
	
	/* Initialize the API */
	init_api();
	
	/* Configure the game */
	if (!config()) {
		fprintf(stderr, "Configuration error\n");
		return 1;
	}
	
	TCOD_console_set_custom_font(font_file, font_glyph_width,
								 font_glyph_height,
								 TCOD_FONT_LAYOUT_ASCII_INCOL);
	
	/* Open up the console window */
	TCOD_console_init_root(ui_width, ui_height, ui_caption, false);
	
	/* Why is this necessary? */
	/*
	splash();
	getkey();
	menu();
	getkey();
	*/
	
	/*
	run_script("test");
	lua_getglobal(L, "monsters");
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		printf("%s - %s\n",
			lua_typename(L, lua_type(L, -2)),
			lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	printf("TOS: %d\n", lua_gettop(L));
	*/
	
	/* Enter the main game loop */
	play();
	
	/* Clean & successful exit */
	return 0;
}
