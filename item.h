#ifndef _ITEM_H

#define _ITEM_H

/* An individual item */
typedef struct {
	char *name;
	bool stackable;
	int count;
	void *next;
} item_t;

/* A list of items */
typedef struct {
	item_t *head, *tail;
} item_list_t;

/* Function prototypes */
item_list_t *item_list_new(void);
int item_list_add(item_list_t*, item_t*);
item_t *item_new(const char*, bool, int);

#endif
