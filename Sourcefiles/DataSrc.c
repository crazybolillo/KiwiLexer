#include "Data.h"

/*
Creates a new LinkedList.
*/
struct LinkList newLinkList(void *value)
{
	struct LinkList retval;
	retval.value = value;
	retval.next = NULL;
	return retval;
}

/*
Adds the LinkedList to the head. 
*/
void appendToList(struct LinkList *data, struct LinkList **head)
{
	if (*head == NULL) {
		*head = data;
	}
	else if ((*head)->next == NULL) {
		(*head)->next = data;
	}
	else {
		struct LinkList *tmp = (*head)->next;
		data->next = tmp;
		(*head)->next = data;
	}
}

/*
Frees all memory used by the list.
*/
void destroyList(struct LinkList *list)
{
	free(list->value);
	free(list);
}

/*
Frees a whole LinkedList by iterating trough all its nodes and freeing
them. Stops once it finds a node that is NULL which signifies the end
of the list.
*/
void destroyAllList(struct LinkList *list)
{
	struct LinkList *next;
	while (list != NULL) {
		next = list->next;
		destroyList(list);
		list = next;
	}
}