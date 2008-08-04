#ifndef _MISC_H

#define _MISC_H

/* Right now these are macros */
#define PUTCH(x, y, c)	TCOD_console_set_char(NULL, x, y, c)
#define GETCH()			TCOD_console_wait_for_keypress(1)

/* Function prototypes */
char choice(char*);
const char* from_script(char*);
void fgcolor(TCOD_color_t);
void bgcolor(TCOD_color_t);
void write(int, int, const char*);
void clear(void);
void update(void);

#endif
