#ifndef DATA_HEADER_GUARD
#define DATA_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>

struct LinkList {
	void *value;
	struct LinkList *next;
};

struct LinkList newLinkList(void *value);
void appendToList(struct LinkList *data, struct LinkList **head);
void destroyList(struct LinkList *list);
void destroyAllList(struct LinkList *list);

#endif
