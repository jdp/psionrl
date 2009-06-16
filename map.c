#include "psionrl.h"

/* Returns a new map instance */
map_t *map_new(int width, int height) {
	int x, y;
	map_t *map;
	
	if ((map = (map_t *)malloc(sizeof(map_t))) == NULL) {
		return NULL;
	}
	
	map->width = width;
	map->height = height;
	map->tiles = (tile_t *)malloc(sizeof(tile_t)*width*height);
	map->fov = TCOD_map_new(width, height);
	
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			map->tiles[y*width+x] = tileset[TILE_WALL];
		}
	}
	
	return map;
}

/* Destroys a map instance */
void map_destroy(map_t* map) {
	free(map->tiles);
	TCOD_map_delete(map->fov);
	free(map);
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

/* Fills a rectangle width a tile type */
void map_fill_rect(map_t* map, tile_t tile, int x, int y, int w, int h) {
	int scan_x, scan_y;
	for (scan_y = y; scan_y < y + h; scan_y++) {
		for (scan_x = x; scan_x < x + w; scan_x++) {
			map->tiles[scan_y*map->width+scan_x] = tile;
		}
	}
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

/* Returns whether or not a player can walk on a tile */
bool is_walkable(map_t *m, int x, int y) {
	return(TCOD_map_is_walkable(m->fov, x, y)
	       && (x >= 0) && (x < m->width)
		   && (y >= 0) && (y < m->height));
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
				map->tiles[y*map->width+x].walkable = false;
				map->tiles[y*map->width+x].transparent = false;
			}
			else {
				map->tiles[y*map->width+x].glyph = '.';
				map->tiles[y*map->width+x].fg_lit = (TCOD_color_t){67,57,0};
				map->tiles[y*map->width+x].walkable = true;
				map->tiles[y*map->width+x].transparent = true;
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
			else {
				map->tiles[y*map->width+x].glyph = '.';
				map->tiles[y*map->width+x].fg_lit = C_LIGHT_GREY;
				map->tiles[y*map->width+x].transparent = true;
				map->tiles[y*map->width+x].walkable = true;
			}
		}
	}
	
	free(cave);
	free(tmp_cave);
}

/* Generate some ruins */
void generate_ruins(map_t *map) {
	/* Start off with a cave */
	generate_cave(map);
	
	/* Special room wall tiles */
	tile_t room_wall = tileset[TILE_WALL];
	room_wall.glyph = '%';
	room_wall.fg_lit = C_BROWN;
	
	/* Put some random rooms onto the map */
	int room, rooms = rand() % 5 + 3;
	int room_x, room_y, room_width, room_height;
	
	/* Build each room */
	for (room = 0; room < rooms; room++) {
		room_width = rand() % 8 + 4;
		room_height = rand() % 6 + 4;
		room_x = rand() % (map->width - room_width);
		room_y = rand() % (map->height - room_height);
		map_fill_rect(map, room_wall, room_x, room_y, room_width, room_height);
		map_fill_rect(map, tileset[TILE_FLOOR], room_x+1, room_y+1,
					  room_width-2, room_height-2);
	}
}

/* Generate a simple dungeon */
void generate_dungeon(map_t* map) {
	map_fill_rect(map, tileset[TILE_FLOOR], map->width/2-3, map->height/2-3,
				  6, 6);
}
