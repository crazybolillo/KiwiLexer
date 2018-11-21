#include "Tokenizer.h"

char *readAll(FILE *fl, unsigned int *sizeread, int sector)
{
	char *rdata = NULL;
	int count = 1;
	do {
		fseek(fl, 0, SEEK_SET);
		rdata = realloc(rdata, sizeof(char) * (sector * count));
		*sizeread = fread(rdata, 1, (sector * count), fl);
		count++;
	} while (feof(fl) == 0);

	rdata = realloc(rdata, *sizeread);
	return rdata;
}

int skipchar(char *ptr, char until, int limit)
{
	int x = 0;
	for (; x < limit; ptr++, x++) {
		if (*ptr == until)
			return x;
	}
	return x;
}

struct mem_LinkToken *mem_newLinkToken(char *id, int idsize)
{
	struct mem_LinkToken *retval = malloc(sizeof(struct mem_LinkToken));
	retval->id = calloc(idsize + 1, sizeof(char));
	memcpy(retval->id, id, idsize);
	retval->next = NULL;
	return retval;
}

void mem_insertToken(struct mem_LinkToken *branch, 
	struct mem_LinkToken **head) 
{
	if ((*head) == NULL) {
		(*head) = branch;
	}
	else if ((*head)->next == NULL) {
		(*head)->next = branch;
	}
	else {
		struct mem_LinkToken *tmp = (*head)->next;

		branch->next = tmp;
		(*head)->next = branch;
	}
}


struct mem_LinkToken *mem_createTokenizer(char *grammar, int gramsize)
{
	struct mem_LinkToken *retval = NULL;
	int x = 0; 
	for (; x < gramsize; grammar++, x++) {
		if (*grammar == TOKEN_LIMIT) {
			grammar++;
			x++;
			int len = skipchar(grammar, TOKEN_LIMIT, gramsize - x);
			struct mem_LinkToken *tok = mem_newLinkToken(grammar, len);
			len++; 
			x += len;
			grammar += len;
			mem_insertToken(tok, &retval);
		}
	}
	return retval;
}

void mem_printTokenizer(struct mem_LinkToken *head)
{
	struct mem_LinkToken *tmphead = head;
	char prntcount = 0;
	while (head != NULL) {
		if (prntcount >= 6) {
			prntcount = 0;
			printf("\n");
		}
		printf("<TOKEN: \"%s\"> ", head->id);
		head = head->next;
		prntcount++;
	}
	head = tmphead;
}

void mem_destroyTokenizer(struct mem_LinkToken *head)
{
	struct mem_LinkToken *nxthead;
	while (head != NULL) {
		nxthead = head->next;
		free(head->id);
		free(head);
		head = nxthead;
	}
}



