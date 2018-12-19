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

struct LinkList *newLexeme(char *id, int idsize, 
	struct MemBlock *mem)
{
	struct LinkList *retval = kimalloc(sizeof(struct LinkList),
		mem);
	if (retval == NULL)
		return NULL;
	retval->value = kicalloc(sizeof(char) * (idsize + 1), mem);
	if (retval->value == NULL)
		return NULL;
	memcpy(retval->value, id, idsize);
	retval->next = NULL;
	return retval;
}


struct LinkList *newAlphabet(char *grammar, int gramsize,
	struct MemBlock *mem)
{
	struct LinkList *retval = NULL;
	int x = 0; 
	for (; x < gramsize; grammar++, x++) {
		if (*grammar == TOKEN_LIMIT) {
			grammar++;
			x++;
			int len = skipchar(grammar, TOKEN_LIMIT, gramsize - x);
			struct LinkList *tok = newLexeme(grammar, len, mem);
			len++; 
			x += len;
			grammar += len;
			if (tok == NULL)
				return retval;
			appendToList(tok, &retval);
		}
	}
	return retval;
}

/*
Returns a pointer to the string inside the tokenizer if it exists
or NULL if it does not.
*/
char *alphabetContains(char *val, int valsz, struct LinkList *tokenizer)
{
	struct LinkList *tmphead = tokenizer;
	char *retval;
	while (tokenizer != NULL) {
		if ((strncmp(val, tokenizer->value, valsz) == 0) &&
			(strlen(tokenizer->value) == valsz)) {
			retval = tokenizer->value;
			tokenizer = tmphead;
			return retval;
		}
		tokenizer = tokenizer->next;
	}
	tokenizer = tmphead;
	return NULL;
}

char *nul_alphabetContains(char *str, struct LinkList *tokenizer)
{
	struct LinkList *tmphead = tokenizer;
	char *retval;
	while(tokenizer != NULL) {
		if (strcmp(tokenizer->value, str) == 0) {
			retval = tokenizer->value;
			tokenizer = tmphead;
			return retval;
		}
		tokenizer = tokenizer->next;
	}
	tokenizer = tmphead;
	return NULL;
}

void printAlphabet(struct LinkList *head)
{
	struct LinkList *tmphead = head;
	char prntcount = 0;
	while (head != NULL) {
		if (prntcount >= 6) {
			prntcount = 0;
			printf("\n");
		}
		printf("<TOKEN: \"%s\"> ", (char *)head->value);
		head = head->next;
		prntcount++;
	}
	head = tmphead;
}



