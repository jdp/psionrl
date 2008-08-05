#include "psionrl.h"

/* Initialize the Lua environment */
int init_scripting(void) {
	if ((L = luaL_newstate()) == NULL) {
		return 0;
	}
	
	luaL_openlibs(L);
	
	return 1;
}

/* Run a script */
int run_script(const char *file) {
	if (luaL_dofile(L, file) == 1) {
		fprintf(stderr, "Error in %s:\n", file);
		fprintf(stderr, "\t%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return(0);
	}
	return(1);
}

/* Returns a global variable from the Lua environment */
const char *from_script(char *option) {
	const char *value = NULL;
	lua_getglobal(L, option);
	if (!lua_isnoneornil(L, -1)) {
		value = lua_tostring(L, -1);
		lua_pop(L, 1);
		return(value);
	}
	else {
		return(NULL);
	}
}
