#include "psionrl.h"

TCOD_console_t *map_layer = NULL;
TCOD_console_t *psion_layer = NULL;

/* Attempts to move, processes a turn */
void attempt_move(map_t* m, int newx, int newy) {
	if (is_walkable(m, newx, newy)) {
		move_player(newx, newy);
	}
}

/* The main game loop */
void play(void) {
	int playing = 1;
	int x, y, vpx, vpy;
	
	/* A messy way to do the map, works for now */
	map_t *map = map_new(50, 50);
	generate_cave(map);
	map_fov_build(map);
	
	/* Set up the offscreen consoles */
	map_layer = TCOD_console_new(ui_viewport_width, ui_viewport_height);
	psion_layer = TCOD_console_new(ui_viewport_width, ui_viewport_height);
	TCOD_console_set_key_color(psion_layer, C_KEY);
	bgcolor(psion_layer, C_KEY);
	
	/* Set up the player */
	init_player();
	rename_player("Gu the Cabeboy");
	blink_player(map);
	
	/* Initialize the inventory */
	
	
	/* Enter the main game loop! */
	while (playing) {
	
		/* Prepare the screen for drawing */
		clear(NULL);
		
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
						fgcolor(map_layer, tile->fg_lit);
					}
					else {
						fgcolor(map_layer, tile->fg_dark);
					}
					putch(map_layer, x, y, tile->glyph);
				}
			}
		}
		
		TCOD_console_blit(map_layer, 0, 0, ui_viewport_width,
		                  ui_viewport_height, NULL, ui_viewport_x,
		                  ui_viewport_y, 255);
		
		/* Draw the player and status */
		fgcolor(NULL, C_WHITE);
		putch(NULL, ui_viewport_x+player->x-vpx, ui_viewport_y+player->y-vpy, '@');
		fgcolor(NULL, C_MSG);
		putstr(NULL, 1, 23, player->name);
		
		/* Redraw the screen */
		update();
		
		/* Handle keypress */
		TCOD_key_t k = getkey();
		if (k.c == 0) {
			switch (k.vk) {
				/* Move around */
				case TCODK_KP7: /* up left */
					attempt_move(map, player->x-1, player->y-1);
					break;
					
				case TCODK_KP8: /* up */
				case TCODK_UP:
					attempt_move(map, player->x, player->y-1);
					break;
					
				case TCODK_KP9: /* up right */
					attempt_move(map, player->x+1, player->y-1);
					break;
					
				case TCODK_KP1: /* down left */
					attempt_move(map, player->x-1, player->y+1);
					break;
				
				case TCODK_KP2: /* down */
				case TCODK_DOWN:
					attempt_move(map, player->x, player->y+1);
					break;
					
				case TCODK_KP3: /* down right */
					attempt_move(map, player->x+1, player->y+1);
					break;
				
				case TCODK_KP4: /* left */
				case TCODK_LEFT:
					attempt_move(map, player->x-1, player->y);
					break;
				
				case TCODK_KP6: /* right */
				case TCODK_RIGHT:
					attempt_move(map, player->x+1, player->y);
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
	fgcolor(NULL, C_MSG);
	putstr(NULL, 1, 1, "Really quit? [yn]");
	update();
	return (choice("yn") == 'n');
}

void inventory(void) {
	clear(NULL);
	fgcolor(NULL, C_WHITE);
	putstr(NULL, 1, 1, "Inventory");
	/*
	item_t *item = inv->head;
	if (item == NULL) {
		fgcolor(NULL, C_GREY);
		putstr(NULL, 1, 3, "Empty");
	}
	else {
		int y = 3;
		while (item != NULL) {
			putstr(NULL, 1, y++, item->name);
			item = item->next;
		}
	}
	*/
	fgcolor(NULL, C_GREY);
	putstr(NULL, 1, 23, "[? for help]");
	update();
	getkey();
}

void character(void) {
	int line = 3;
	
	clear(NULL);
	
	/* Show statistics */
	fgcolor(NULL, C_WHITE);
	putstrf(NULL, 1, 1, "%s", player->name);
	fgcolor(NULL, C_LIGHT_GREY);
	putstrf(NULL, 1, line++, "Level:  %d", 1);
	putstrf(NULL, 1, line++, "Energy: %d/%d", 5, 5);
	putstrf(NULL, 1, line++, "Focus:  %d", 3);
	
	/* Show intrinsics */
	line++;
	fgcolor(NULL, C_WHITE);
	putstrf(NULL, 1, line++, "Intrinsics");
	fgcolor(NULL, C_LIGHT_GREY);
	line++;
	putstrf(NULL, 1, line++, "Cold resistant");
	
	update();
	getkey();
}
