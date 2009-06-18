#include "psionrl.h"

void psion_scan(map_t* map) {
	int scanning, try_x, try_y, scan_x, scan_y;
	TCOD_key_t k;
	
	scanning = 1;
	scan_x = try_x = player->x;
	scan_y = try_y = player->y;
	
	while (scanning) {
		k = getkey();
		switch (k.vk) {
			case TCODK_ESCAPE:
				scanning = 0;
				break;
			default:
				break;
		}
	}
	
}
