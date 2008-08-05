#include "psionrl.h"

/* Returns a new map instance */
map_t *map_new(int width, int height) {
	map_t *map;
	
	if ((map = (map_t *)malloc(sizeof(map_t))) == NULL) {
		return(NULL);
	}
	
	map->width = width;
	map->height = height;
	map->tiles = (tile_t *)malloc(sizeof(tile_t)*width*height);
	map->fov = TCOD_map_new(width, height);
	
	return(map);
}

/* Returns a static map */
map_t *map_load_static(const char *name) {
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
	const char *ok_tiles = OK_TILES;
	int cur_tile = 0, scan = 0;
	while (raw_tiles[scan++]) {
		if (strchr(ok_tiles, raw_tiles[scan])) {
			switch (raw_tiles[scan]) {
				case '.':
					map->tiles[cur_tile++] = tileset[TILE_FLOOR];
					break;
				case '#':
					map->tiles[cur_tile++] = tileset[TILE_WALL];
					break;
				case '@':
					player_x = cur_tile % map->width;
					player_y = (cur_tile-(cur_tile%map->width))/map->width;
					map->tiles[cur_tile++] = tileset[TILE_FLOOR];
				default:
					break;
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

/* Calculates the map's fov based on tiles */
void map_build_fov(map_t *map) {
	int cx, cy;
	for (cy = 0; cy < map->height; cy++) {
		for (cx = 0; cx < map->width; cx++) {
			tile_t *tile = tile_at(map, cx, cy);
			TCOD_map_set_properties(map->fov, cx, cy, tile->walkable,
									tile->transparent);
		}
	}
}

/* Returns a tile at the given position */
tile_t *tile_at(map_t *map, int x, int y) {
	if ((x < 0) || (x > map->width-1)) {
		return(NULL);
	}
	if ((y < 0) || (y > map->height-1)) {
		return(NULL);
	}
	return(&(map->tiles[y*map->width+x]));
}



/* Simple dig function */
/*
void map_dig(map_t *map, TCOD_map_t fov_overlay)
{
	memset(map->tiles, TILE_FLOOR, map->width * map->height);
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
*/
