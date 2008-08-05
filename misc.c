#include "psionrl.h"

/* Forces player to pick a key within a list */
char choice(char *options) {
	TCOD_key_t k;
	int found = 0;
	while (!found) {
		k = GETCH();
		if ((strchr(options, k.c) != NULL) && (k.c > 0)) {
			found = 1;
		}
	}
	return(k.c);
}

/* Change the current foreground color */
void fgcolor(TCOD_color_t c) {
	TCOD_console_set_foreground_color(NULL, c);
}

/* Change the current background color */
void bgcolor(TCOD_color_t c) {
	TCOD_console_set_background_color(NULL, c);
}

/* A generic string write function */
void write(int x, int y, const char *str) {
	TCOD_console_print_left(NULL, x, y, TCOD_BKGND_SET, str);
}

/* Clear the screen */
void clear(void) {
	TCOD_console_clear(NULL);
}

/* Update the screen */
void update(void) {
	TCOD_console_flush();
}
