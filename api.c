#include "psionrl.h"

/* Register all API functions */
void init_api(void) {
	lua_register(L, "move_player", api_move_player);
}

/* Move the player to x, y */
static int api_move_player(lua_State *LS) {
	if (lua_gettop(LS) < 2) {
		lua_pushstring(LS, "move_player takes 2 arguments");
		lua_error(LS);
		return(0);
	}
	
	int x = lua_tonumber(LS, -2);
	int y = lua_tonumber(LS, -1);
	lua_pop(LS, 2);
	
	player_x = x;
	player_y = y;
	printf("moved player to %d,%d\n", x, y);
	
	return(1);
}
