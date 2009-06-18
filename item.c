#include "psionrl.h"

TCOD_list_t inv;

/* Adds an item to the list */
int inventory_add(item_t *item) {
	TCOD_list_push(inv, (const void *)item);
}

/* Returns a new item */
item_t *item_new(const char *name, bool stackable, int count) {
	item_t *item;
	item = (item_t *)malloc(sizeof(item_t));
	item->name = (char *)malloc(sizeof(char)*strlen(name));
	strcpy(item->name, name);
	item->stackable = stackable;
	item->count = count;
	item->next = NULL;
	return item;
}
