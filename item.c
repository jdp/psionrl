#include "psionrl.h"

/* Returns an empty item list */
item_list_t *item_list_new(void) {
	item_list_t *ilist;
	ilist = (item_list_t *)malloc(sizeof(item_list_t));
	ilist->head = NULL;
	ilist->tail = NULL;
	return(ilist);
}

/* Adds an item to the list */
int item_list_add(item_list_t *ilist, item_t *item) {
	if (ilist->head == NULL) {
		ilist->head = item;
	}
	if (ilist->tail == NULL) {
		ilist->tail = item;
	}
	else {
		item_t *tail = ilist->tail;
		tail->next = item;
	}
	return(1);
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
