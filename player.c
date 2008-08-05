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
