#include "common.h"

extern lua_State *L;

/* Returns a new map instance */
map_t *map_new(int width, int height) {
	map_t *map;
	
	if ((map = (map_t *)malloc(sizeof(map_t))) == NULL)
		return(NULL);
	
	map->width = width;
	map->height = height;
	map->tiles = (tile_t *)malloc(sizeof(tile_t)*width*height);
	memset(map->tiles, TILE_FLOOR, width*height);
	map->fov = TCOD_map_new(width, height);
	
	return(map);
}

/* Returns a static map */
map_t *map_new_static(const char *name) {
	/* Make sure the map exists */
	lua_getglobal(L, name);
	if (lua_isnoneornil(L, -1)) {
		return(NULL);
	}
	
	/* Fill in the map dimensions */
	lua_getfield(L, -1, "width");
	if (lua_isnoneornil(L, -1)) {
		return(NULL);
	}
	int width = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	lua_getfield(L, -1, "height");
	if (lua_isnoneornil(L, -1)) {
		return(NULL);
	}
	int height = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	/* Create the map as usual */
	map_t *map = map_new(width, height);
	
	/* Get the tiles */
	lua_getfield(L, -1, "tiles");
	if (lua_isnoneornil(L, -1)) {
		return(NULL);
	}
	const char *raw_tiles = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	/* Fill in the tiles based on the map */
	printf("%s\n", raw_tiles);
	int cur_tile = 0, scan = 0;
	while (raw_tiles[scan++]) {
		if (raw_tiles[scan] == '#' || raw_tiles[scan] == '.') {
			printf("%c", raw_tiles[scan]);
			switch (raw_tiles[scan]) {
				case '.':
					map->tiles[cur_tile++] = TILE_FLOOR;
					break;
				case '#':
					map->tiles[cur_tile++] = TILE_WALL;
					break;
				default:
					break;
			}
			if (!(cur_tile % width)) {
				printf("\n");
			}
		}
	}
	
	/* Run an option initialization function */
	lua_getfield(L, -1, "init");
	if (!lua_isnoneornil(L, -1)) {
		lua_call(L, 0, 0);
	}
	
	/* Clean up the stack */
	lua_pop(L, 2);
	
	return(map);
}

/* Returns a tile at the given position */
tile_t tile_at(map_t *map, int x, int y) {
	if ((x < 0) || (x > map->width-1))
		return(TILE_EMPTY);
	if ((y < 0) || (y > map->height-1))
		return(TILE_EMPTY);
	return(map->tiles[y*map->width+x]);
}

/* Calculates the map's fov based on tiles */
void map_build_fov(map_t *map) {
	int cx, cy;
	for (cy = 0; cy < map->height; cy++) {
		for (cx = 0; cx < map->width; cx++) {
			switch (tile_at(map, cx, cy)) {
				case TILE_EMPTY:
					TCOD_map_set_properties(map->fov, cx, cy, false, false);
					break;
				case TILE_FLOOR:
					TCOD_map_set_properties(map->fov, cx, cy, true, true);
					break;
				case TILE_WALL:
					TCOD_map_set_properties(map->fov, cx, cy, false, false);
					break;
				default:
					break;
			}
		}
	}
}

/* Simple dig function */
void map_dig(map_t *map, TCOD_map_t fov_overlay)
{
	/* Fill the map with floor tiles */
	memset(map->tiles, TILE_FLOOR, map->width * map->height);
	
	/* Generate the field-of-view overlay based on map */
	int x, y;
	for (y = 0; y < map->height; y++)
	{
		for (x = 0; x < map->width; x++)
		{
			switch (tile_at(map, x, y))
			{
				case TILE_EMPTY:
					TCOD_map_set_properties(fov_overlay, x, y, 0, 0);
					break;
				case TILE_FLOOR:
					TCOD_map_set_properties(fov_overlay, x, y, 1, 1);
					break;
				case TILE_WALL:
					TCOD_map_set_properties(fov_overlay, x, y, 0, 0);
					break;
				default:
					break;
			}
		}
	}
}
