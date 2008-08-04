#include "common.h"

/* Make the inventory accessible */
item_list_t *inv = NULL;

/* Returns whether or not a player can walk on a tile */
bool is_walkable(map_t *m, int x, int y) {
	return(TCOD_map_is_walkable(m->fov, x, y)
		   && (x >= 0) && (x < m->width)
		   && (y >= 0) && (y < m->height));
}

/* The main game loop */
void play(void) {
	int playing = 1;
	player_x = player_y = 0;
	int x, y, vpx, vpy;
	
	/* A messy way to do the map, works for now */
	map_t *map = map_new_static("hq_floor_1");
	//map_t *map = map_new(50, 50);
	map_build_fov(map);
	
	/* Initialize the inventory */
	inv = item_list_new();
	
	/* Enter the main game loop! */
	while (playing) {
		/* Prepare the screen for drawing */
		clear();
		
		/* Calculate the position of the viewport */
		vpx = player_x - ui_viewport_width/2 > 0 ? player_x - ui_viewport_width/2 : 0; //ui_viewport_width/2;
		vpy = player_y - ui_viewport_height/2 > 0 ? player_y - ui_viewport_height/2 : 0; //ui_viewport_height/2;
		if ((vpx + ui_viewport_width) > map->width)
			vpx = map->width - ui_viewport_width;
		if ((vpy + ui_viewport_height) > map->height)
			vpy = map->height - ui_viewport_height;
		if (ui_viewport_width > map->width)
			vpx = 0;
		if (ui_viewport_height > map->height)
			vpy = 0;
			
		/* Calculate the field of view */
		TCOD_map_compute_fov(map->fov, player_x, player_y, 5);

		/* Only display the part of the map inside the viewport */
		for (y = 0; y < ui_viewport_height; y++) {
			for (x = 0; x < ui_viewport_width; x++) {
				if (TCOD_map_is_in_fov(map->fov, vpx+x, vpy+y))
					fgcolor(color_grey);
				else
					fgcolor(color_dark_grey);
				
				if (((vpy+y) < map->height) && ((vpx+x) < map->width)) {
					switch (tile_at(map, vpx+x, vpy+y)) {
						case TILE_FLOOR:
							write(ui_viewport_x+x, ui_viewport_y+y, ".");
							break;
						case TILE_WALL:
							write(ui_viewport_x+x, ui_viewport_y+y, "#");
							break;
						default:
							write(ui_viewport_x+x, ui_viewport_y+y, "?");
							//printf("unknown tile: %d\n", tile_at(map, vpy+y, vpx+x));
							break;
					}
				}
			}
		}
		
		/* Draw the player and status */
		fgcolor(color_white);
		write(ui_viewport_x+player_x-vpx, ui_viewport_y+player_y-vpy, "@");
		fgcolor(color_grey);
		write(1, 23, "Gu the Cabeboy");
		
		/* Redraw the screen */
		update();
		
		/* Handle keypress */
		TCOD_key_t k = GETCH();
		if (k.c == 0) {
			switch (k.vk) {
				/* Move around */
				case TCODK_UP:
					if (is_walkable(map, player_x, player_y-1))
						player_y--;
					break;
				case TCODK_DOWN:
					if (is_walkable(map, player_x, player_y+1))
						player_y++;
					break;
				case TCODK_LEFT:
					if (is_walkable(map, player_x-1, player_y))
						player_x--;
					break;
				case TCODK_RIGHT:
					if (is_walkable(map, player_x+1, player_y))
						player_x++;
					break;
				default:
					break;
			}
		}
		else {
			switch (k.c) {
				/* Quit the game */
				case 'Q':
					fgcolor(color_grey);
					write(1, 1, "Really quit? [yn]");
					update();
					if (choice("yn") == 'y') {
						playing = 0;
					}
					break;
				
				/* Open up the inventory */
				case 'i':
					inventory();
					break;
				default:
					break;
			}
		}
	}
}

void inventory(void) {
	clear();
	fgcolor(color_white);
	write(1, 1, "Inventory");
	item_t *item = inv->head;
	if (item == NULL) {
		fgcolor(color_grey);
		write(1, 3, "Empty");
	}
	else {
		int y = 3;
		while (item != NULL) {
			write(1, y++, item->name);
			item = item->next;
		}
	}
	fgcolor(color_grey);
	write(1, 23, "[? for help]");
	update();
	GETCH();
}
