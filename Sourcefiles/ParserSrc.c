#include "Parser.h"

char *PARSE_ERR = "KPER";

void addGrammar(struct Production *prod, struct Grammar *grammar)
{
	if (prod->gram == NULL) {
		prod->gram = grammar;
		return;
	}
	else if (prod->gram->next == NULL) {
		prod->gram->next = grammar;
	}
}


struct Production *newProduction(char *prodname, int namesize)
{
	struct Production *retval = malloc(sizeof(struct Production));
	retval->prodname = calloc(namesize + 1, sizeof(char));
	memcpy(retval->prodname, prodname, namesize);
	retval->next = NULL;
	return retval;
}

struct Grammar newGrammar(char *type, int typesz, 
	struct LinkList *tokenizer)
{
	char *ntype = contains(type, typesz, tokenizer);
	struct Grammar retval;
	if (ntype != NULL) {
		retval.type = ntype;
		retval.next = NULL;
	}
	else {
		retval.type = NULL;
		retval.next = NULL;
	}
	return retval;
}

struct Production *parseNextProduction(struct KiwiInput *input, 
	struct LinkList *tokenizer)
{
	unsigned char errflag = 0x00; 
	struct Token lextok;
	struct Production *retval = NULL;
	while (1) {
		lextok = mem_lexNext(input, tokenizer);
		if (strcmp(lextok.type, EOF_ID) == 0) {
			return NULL;
		}
		else if (lextok.type == CONST_STRING_ID) {
			
		}
	}
}