#include "psionrl.h"

/* Wait for and return a keypress */
TCOD_key_t getkey(void) {
	return(TCOD_console_wait_for_keypress(1));
}

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
void putstr(int x, int y, char *str) {
	TCOD_console_print_left(NULL, x, y, TCOD_BKGND_SET, str);
}

/* A generic variable-length string write function */
void putstrf(int x, int y, char *fmt, ...) {
	va_list vl;
	char buffer[80];
	
	va_start(vl, fmt);
	vsprintf(buffer, fmt, vl);
	va_end(vl);
	
	putstr(x, y, buffer);
}

/* Place a character on the screen */
void putch(int x, int y, unsigned char c) {
	TCOD_console_put_char(NULL, x, y, c, TCOD_BKGND_SET);
}

/* Clear the screen */
void clear(void) {
	TCOD_console_clear(NULL);
}

/* Update the screen */
void update(void) {
	TCOD_console_flush();
}
