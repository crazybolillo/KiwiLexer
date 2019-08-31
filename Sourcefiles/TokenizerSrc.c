#include "Tokenizer.h"

/*
Adds the node to the list. It adds it to the end of the list.
*/
void dev_appendToAlph(struct AlphList *data, struct AlphList **head)
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

/*
Skips chars (moves the pointer) in a char array until it finds the char
passed trough the parameters.
*/
int skipchar(char *ptr, char until, int limit)
{
	int x = 0;
	for (; x < limit; ptr++, x++) {
		if (*ptr == until)
			return x;
	}
	return x;
}

/*
Creates a lexeme (node of a linked list). Allocates all necessary memory.
Returns NULL if there was not enough memory.
*/
struct AlphList *dev_newLexeme(char *id, int idsize, 
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

/*
Reads a text file and creates an alphabet with all the lexems found in
it.
*/
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
			struct AlphList *tok = dev_newLexeme(grammar, len, mem);
			len++; 
			x += len;
			grammar += len;
			if (tok == NULL) {
				return retval;
			}
			else {
				dev_appendToAlph(tok, &retval);
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
Returns a pointer to the string if its contained in the alphabet. Returns
NULL if it could not be found. 
*/
char *dev_alphabetContains(char *str, struct AlphList *tokenizer)
{
	struct AlphList *tmphead = tokenizer;
	char* retval = NULL;
	while(tokenizer != NULL) {
		if (strcmp(tokenizer->value, str) == 0) {
			retval = tokenizer->value;
			tokenizer = tmphead;
			return retval;
		}
		tokenizer = tokenizer->next;
	}
	tokenizer = tmphead;
	return retval;
}

/*
Prints the alphabet lexemes. Mainly used for debugging.
*/
void dev_printAlphabet(struct AlphList *head)
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



