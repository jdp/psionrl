#include "psionrl.h"

/* Initialize the player */
int init_player(void) {
	player = (player_t *)realloc(player, sizeof(player_t));
	if (player == NULL) {
		return(0);
	}
	player->name = NULL;
	player->x = 0;
	player->y = 0;
	player->fov_radius = 5;
	return(1);
}

/* Rename the player */
void rename_player(const char *name) {
	player->name = (char *)realloc(player->name, strlen(name));
	strcpy(player->name, name);
}

/* Move the player */
void move_player(int x, int y) {
	player->x = x;
	player->y = y;
}

/* Move player to a random walkable tile */
void blink_player(map_t *map) {
	int x, y;
	bool found = false;
	srand(time(NULL));
	do {
		x = rand() % map->width;
		y = rand() % map->height;
		found = map->tiles[y*map->width+x].walkable;
	} while (!found);
	move_player(x, y);
}
