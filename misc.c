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

/* A generic variable-length string write function */
void putstrf(TCOD_console_t *con, int x, int y, char *fmt, ...) {
	va_list vl;
	char buffer[80];
	
	va_start(vl, fmt);
	vsprintf(buffer, fmt, vl);
	va_end(vl);
	
	putstr(con, x, y, buffer);
}

