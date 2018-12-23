#ifndef DATA_HEADER_GUARD
#define DATA_HEADER_GUARD

#include <stdlib.h>
#include "Memory.h"

struct LinkList {
	char *value;
	struct LinkList *next;
};

struct LinkList *newLinkList(void *value, struct MemBlock *mem);
void appendToList(struct LinkList *data, struct LinkList **head);
void destroyList(struct LinkList *list);
void destroyAllList(struct LinkList *list);

#endif
