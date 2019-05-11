#include "Tokenizer.h"

struct AlphList *newAlph(void *value, struct MemBlock *mem)
{
	struct AlphList *retval = kimalloc(sizeof(struct AlphList), mem);
	if (retval == NULL) {
		return NULL;
	}
	retval->value = value;
	retval->next = NULL;
	return retval;
}


void appendToAlph(struct AlphList *data, struct AlphList **head)
{
	if (*head == NULL) {
		*head = data;
	}
	else if ((*head)->next == NULL) {
		(*head)->next = data;
	}
	else {
		struct AlphList *tmp = (*head)->next;
		data->next = tmp;
		(*head)->next = data;
	}
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

struct AlphList *newLexeme(char *id, int idsize, 
	struct MemBlock *mem)
{
	struct AlphList *retval = kimalloc(sizeof(struct AlphList),
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


struct AlphList *newAlphabet(char *grammar, int gramsize,
	struct MemBlock *mem)
{
	struct AlphList *retval = NULL;
	int x = 0; 
	while (x < gramsize) {
		if (*grammar == TOKEN_LIMIT) {
			grammar++;
			x++;
			int len = skipchar(grammar, TOKEN_LIMIT, gramsize - x);
			struct AlphList *tok = newLexeme(grammar, len, mem);
			len++; 
			x += len;
			grammar += len;
			if (tok == NULL) {
				return retval;
			}
			else {
				appendToAlph(tok, &retval);
			}
		}
		else {
			grammar++;
			x++;
		}
	}
	return retval;
}

/*
Returns a pointer to the string inside the tokenizer if it exists
or NULL if it does not.
*/
char *alphabetContains(char *val, int valsz, 
	struct AlphList *tokenizer)
{
	while (tokenizer != NULL) {
		if ((strncmp(val, tokenizer->value, valsz) == 0) &&
			(strlen(tokenizer->value) == valsz)) {
			return tokenizer->value;
		}
		tokenizer = tokenizer->next;
	}
	return NULL;
}

char *nul_alphabetContains(char *str, struct AlphList *tokenizer)
{
	struct AlphList *tmphead = tokenizer;
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

void printAlphabet(struct AlphList *head)
{
	struct AlphList *tmphead = head;
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



