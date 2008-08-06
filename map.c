#include "psionrl.h"

/* Returns a new map instance */
map_t *map_new(int width, int height) {
	map_t *map;
	
	if ((map = (map_t *)malloc(sizeof(map_t))) == NULL) {
		return NULL;
	}
	
	map->width = width;
	map->height = height;
	map->tiles = (tile_t *)malloc(sizeof(tile_t)*width*height);
	map->fov = TCOD_map_new(width, height);
	
	int x, y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			map->tiles[y*width+x] = tileset[TILE_FLOOR];
		}
	}
	
	return map;
}

/* Returns a static map */
map_t *map_load_static(const char *name) {
	/* Make sure the map exists */
	lua_getglobal(L, name);
	if (lua_isnoneornil(L, -1)) {
		return NULL;
	}
	
	/* Fill in the map dimensions */
	lua_getfield(L, -1, "width");
	if (lua_isnoneornil(L, -1)) {
		return NULL;
	}
	int width = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	lua_getfield(L, -1, "height");
	if (lua_isnoneornil(L, -1)) {
		return NULL;
	}
	int height = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	/* Create the map as usual */
	map_t *map = map_new(width, height);
	
	/* Get the tiles */
	lua_getfield(L, -1, "tiles");
	if (lua_isnoneornil(L, -1)) {
		return NULL;
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
					player->x = cur_tile % map->width;
					player->y = (cur_tile-(cur_tile%map->width))/map->width;
					map->tiles[cur_tile++] = tileset[TILE_FLOOR];
				default:
					break;
			}
		}
	}
	
	/* Run an optional initialization function */
	lua_getfield(L, -1, "init");
	if (!lua_isnoneornil(L, -1)) {
		lua_call(L, 0, 0);
	}
	
	/* Clean up the stack */
	lua_pop(L, 2);
	
	return map;
}

/* Generates the fov settings from map tiles */
void map_fov_build(map_t *map) {
	int x, y;
	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			tile_t *tile = tile_at(map, x, y);
			TCOD_map_set_properties(map->fov, x, y, tile->transparent,
									tile->walkable);
		}
	}
}

/* Computes the fov */
void map_fov_do(map_t *map, int x, int y) {
	TCOD_map_compute_fov(map->fov, x, y, player->fov_radius);
}

/* Returns a tile at the given position */
tile_t *tile_at(map_t *map, int x, int y) {
	if ((x < 0) || (x > map->width-1)) {
		return NULL;
	}
	if ((y < 0) || (y > map->height-1)) {
		return NULL;
	}
	return &map->tiles[y*map->width+x];
}

/* Generates a lame-ass forest */
void generate_forest(map_t *map) {
	int x, y;
	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			if ((rand() % 100) < 10) {
				map->tiles[y*map->width+x].glyph = 'T';
				map->tiles[y*map->width+x].fg_lit = (TCOD_color_t){88,141,67};
				map->tiles[y*map->width+x].fg_dark = C_DARK_GREY;
				//map->tiles[y*map->width+x].walkable = false;
				map->tiles[y*map->width+x].transparent = false;
			}
			else {
				map->tiles[y*map->width+x].glyph = '.';
				map->tiles[y*map->width+x].fg_lit = (TCOD_color_t){67,57,0};
			}
		}
	}
}

/* Generates a cave */
void generate_cave(map_t *map) {
	
	/*
	Algorithm from RogueBasin
	R(s,t) -- number of neighbors s spaces away from tile t
	Winit(p) = rand[0,100) < 40
	Repeat 4: W(p) = R(1,p) >= 5 || R(2,p) == 2
	Repeat 3: W(p) = R(1,p) >= 5
	*/
	
	int i, p, x, y;
	srand(time(NULL));
	
	/* Current and intermediate caves */
	int *cave, *tmp_cave;
	
	/* Linear map length from cartesian */
	int map_size = map->width * map->height;
	
	cave = (int *)malloc(sizeof(int) * map_size);
	tmp_cave = (int *)malloc(sizeof(int) * map_size);
	
	/* 45% chance that a tile starts as a wall */
	for (i = 0; i < map_size; i++) {
		cave[i] = (rand() % 100) < 45;
	}
	
	/* Offsets to get neighboring cells */
	int nb_x, nb_xo, nb_y, nb_yo;
	
	/* 4x W(p) == R(1,p) >= 5 || R(2,p) == 2 */
	for (i = 0; i < 4; i++) {
		for (p = 0; p < map_size; p++) {
			int R = 0;
			for (nb_yo = -1; nb_yo < 2; nb_yo++) {
				for (nb_xo = -1; nb_xo < 2; nb_xo++) {
					nb_x = (p % map->width) + nb_xo;
					nb_y = ((p - (p % map->width)) / map->width) + nb_yo;
					if ((nb_x < 0) || (nb_x >= map->width)) {
						R++;
					}
					else if ((nb_y < 0) || (nb_y >= map->height)) {
						R++;
					}
					else {
						if (cave[nb_y*map->width+nb_x]) {
							R++;
						}
					}
				}
			}
			tmp_cave[p] = R >= 5;
		}
		for (p = 0; p < map_size; p++) {
			cave[p] = tmp_cave[p];
		}
	}
	
	/* Put the cave on the map */
	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			if (cave[y*map->width+x]) {
				map->tiles[y*map->width+x].glyph = '#';
				map->tiles[y*map->width+x].fg_lit = C_GREY;
				map->tiles[y*map->width+x].transparent = false;
				map->tiles[y*map->width+x].walkable = false;
			}
		}
	}
	
	free(cave);
	free(tmp_cave);
}
