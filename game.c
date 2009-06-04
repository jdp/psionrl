#include "psionrl.h"

/* Returns whether or not a player can walk on a tile */
bool is_walkable(map_t *m, int x, int y) {
	return(TCOD_map_is_walkable(m->fov, x, y)
	       && (x >= 0) && (x < m->width)
		   && (y >= 0) && (y < m->height));
}

/* The main game loop */
void play(void) {
	int playing = 1;
	int x, y, vpx, vpy;
	
	/* A messy way to do the map, works for now */
	map_t *map = map_new(50, 50);
	generate_cave(map);
	map_fov_build(map);
	
	init_player();
	rename_player("Gu the Cabeboy");
	blink_player(map);
	
	/* Initialize the inventory */
	inv = item_list_new();
	
	/* Enter the main game loop! */
	while (playing) {
		/* Prepare the screen for drawing */
		clear();
		
		/* Calculate the position of the viewport */
		vpx = player->x - ui_viewport_width/2 > 0 ?
			  player->x - ui_viewport_width/2 : 0;
		vpy = player->y - ui_viewport_height/2 > 0 ?
			  player->y - ui_viewport_height/2 : 0;
		if ((vpx + ui_viewport_width) > map->width) {
			vpx = map->width - ui_viewport_width;
		}
		if ((vpy + ui_viewport_height) > map->height) {
			vpy = map->height - ui_viewport_height;
		}
		if (ui_viewport_width > map->width) {
			vpx = 0;
		}
		if (ui_viewport_height > map->height) {
			vpy = 0;
		}
			
		/* Calculate the field of view */
		map_fov_do(map, player->x, player->y);
		
		/* Only display the part of the map inside the viewport */
		for (y = 0; y < ui_viewport_height; y++) {
			for (x = 0; x < ui_viewport_width; x++) {
				if (((vpy+y) < map->height) && ((vpx+x) < map->width)) {
					tile_t *tile = tile_at(map, vpx+x, vpy+y);
					if (TCOD_map_is_in_fov(map->fov, vpx+x, vpy+y)) {
						fgcolor(tile->fg_lit);
					}
					else {
						fgcolor(tile->fg_dark);
					}
					putch(ui_viewport_x+x, ui_viewport_y+y, tile->glyph);
				}
			}
		}
		
		/* Draw the player and status */
		fgcolor(C_WHITE);
		putch(ui_viewport_x+player->x-vpx, ui_viewport_y+player->y-vpy, '@');
		fgcolor(C_MSG);
		putstr(1, 23, player->name);
		
		/* Redraw the screen */
		update();
		
		/* Handle keypress */
		TCOD_key_t k = getkey();
		if (k.c == 0) {
			switch (k.vk) {
				/* Move around */
				case TCODK_KP8:
				case TCODK_UP:
					if (is_walkable(map, player->x, player->y-1)) {
						player->y--;
					}
					break;
				
				case TCODK_KP2:
				case TCODK_DOWN:
					if (is_walkable(map, player->x, player->y+1)) {
						player->y++;
					}
					break;
				
				case TCODK_KP4:
				case TCODK_LEFT:
					if (is_walkable(map, player->x-1, player->y)) {
						player->x--;
					}
					break;
				
				case TCODK_KP6:
				case TCODK_RIGHT:
					if (is_walkable(map, player->x+1, player->y)) {
						player->x++;
					}
					break;
				
				/* View character summary */
				case TCODK_TAB:
					character();
					break;
				
				default:
					printf("%d %d\n", TCODK_TAB, k.vk);
					break;
			}
		}
		else {
			switch (k.c) {
				
				/* Quit the game */
				case 'Q':
					playing = quit();
					break;
				
				/* Open up the inventory */
				case 'i':
					inventory();
					break;
				
				/* View character summary */
				case 'c':
					character();
					break;
					
				/* Use scan ability */
				case 's':
					psion_scan(map);
					break;
				
				default:
					break;
			}
		}
	}
	
	/* Clean up, the game is over */
	map_destroy(map);
}

bool quit(void) {
	fgcolor(C_MSG);
	putstr(1, 1, "Really quit? [yn]");
	update();
	return (choice("yn") == 'n');
}

void inventory(void) {
	clear();
	fgcolor(C_WHITE);
	putstr(1, 1, "Inventory");
	item_t *item = inv->head;
	if (item == NULL) {
		fgcolor(C_GREY);
		putstr(1, 3, "Empty");
	}
	else {
		int y = 3;
		while (item != NULL) {
			putstr(1, y++, item->name);
			item = item->next;
		}
	}
	fgcolor(C_GREY);
	putstr(1, 23, "[? for help]");
	update();
	getkey();
}

void character(void) {
	int line = 3;
	
	clear();
	
	/* Show statistics */
	fgcolor(C_WHITE);
	putstrf(1, 1, "%s", player->name);
	fgcolor(C_LIGHT_GREY);
	putstrf(1, line++, "Level:  %d", 1);
	putstrf(1, line++, "Health: %d/%d", 7, 7);
	putstrf(1, line++, "Mana:   %d/%d", 5, 5);
	putstrf(1, line++, "Power:  %d", 3);
	putstrf(1, line++, "Wisdom: %d", 1);
	putstrf(1, line++, "Speed:  %d", 2);
	
	/* Show intrinsics */
	line++;
	fgcolor(C_WHITE);
	putstrf(1, line++, "Intrinsics");
	fgcolor(C_LIGHT_GREY);
	line++;
	putstrf(1, line++, "Cold resistant");
	
	update();
	getkey();
}
