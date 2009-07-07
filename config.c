#include "psionrl.h"

int config(void) {

	if (luaL_dofile(L, "config.lua") == 1) {
		fprintf(stderr, "Error in config.lua:\n");
		fprintf(stderr, "\t%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return 0;
	}
	
	/* Get the config table */
	lua_getfield(L, LUA_GLOBALSINDEX, "config");
	
	/* Get configuration from config.window */
	lua_getfield(L, -1, "window");
	lua_getfield(L, -1, "caption");
	ui_caption = strdup(lua_tostring(L, -1));
	printf("%s\n", ui_caption);
	lua_pop(L, 2);
	
	/* Get configuration from config.font */
	lua_getfield(L, -1, "font");
	lua_getfield(L, -1, "file");
	font_file = strdup(lua_tostring(L, -1));
	lua_pop(L, 1);
	lua_getfield(L, -1, "glyph_width");
	font_glyph_width = lua_tointeger(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, -1, "glyph_height");
	font_glyph_height = lua_tointeger(L, -1);
	lua_pop(L, 3);

	return 1;
}
