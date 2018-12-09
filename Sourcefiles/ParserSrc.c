#include "Parser.h"

char *PARSE_ERR = "KPER";
char *PROD_SIGNAL = "->";
char *PROD_END = ";";

struct NestLinkList *newProduction(char *prodname, int namesize, struct
	MemBlock *mem)
{
	struct NestLinkList *retval = kimalloc(sizeof(struct NestLinkList),
		mem);
	if (retval == NULL) {
		return NULL;
	}
	retval->value = kicalloc(sizeof(char) * namesize + 1, mem);
	if (retval->value == NULL) {
		popMemory(mem, sizeof(struct NestLinkList));
		return NULL;
	}
	memcpy(retval->value, prodname, namesize);
	retval->next = NULL;
	retval->nestval = NULL;
	return retval;
}

struct LinkList *newGrammar(char *type, int typesz,
	struct LinkList *tokenizer, struct MemBlock *mem)
{
	struct LinkList *retval = kimalloc(sizeof(struct LinkList),
		mem);
	if (retval == NULL) {
		return NULL;
	}
	retval->value = contains(type, typesz, tokenizer);
	if (retval->value == NULL) {
		freeMemory(mem);
		return NULL;
	}
	retval->next = NULL;
	return retval;
}

struct NestLinkList *newParser(struct KiwiInput *input,
	struct LinkList *tokenizer, struct MemBlock *mem)
{
	struct NestLinkList *retval;
	struct LinkList *gram;
	struct Token *token = lexNext(input, tokenizer, mem);
	if (strcmp(token->type, CONST_STRING_ID) != 0) {
		return NULL;
	}
	retval = newProduction(token->value, strlen(token->value), mem);
	retval->next = NULL;
	if (retval == NULL) {
		return NULL;
	}
	token = lexNext(input, tokenizer, mem);
	if (strcmp(token->type, PROD_SIGNAL) != 0) {
		freeMemory(mem);
		return NULL;
	}
	while(1) {
		token = lexNext(input, tokenizer, mem);
		if (strcmp(token->type, CONST_STRING_ID) == 0) {
			gram = newLinkList(token->type, mem);
		}
		else if (strcmp(token->type, PROD_END) == 0) {
			return retval;
		}
		else {
			freeMemory(mem);
			return NULL;
		}
	}
}