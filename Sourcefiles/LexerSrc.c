#include "Lexer.h"

char *DOUBLE_ID = "DBL";
char *INT_ID = "INT";
char *STRING_ID = "STR";
char *MIX_STRING_ID = "MIX";

int isNumber(char *value, int size)
{
	int pointflag = 0;
	int minusflag = 0;
	for (int x = 0; x < size; x++) {
		if (*(value + x) == '.') {
			pointflag++;
			if (pointflag > 1)
				return 0;
		}
		else if (*(value + x) == '-') {
			minusflag++;
			if (minusflag > 1)
				return 0;
		}
		else if (isdigit(*(value + x)) == 0) {
			return 0;
		}
	}
	if (pointflag == 0)
		return INT_TYPE;
	else
		return DOUBLE_TYPE;
}

int isString(char *value, int size) {
	for (int x = 0; x < size; x++) {
		if (isalpha(*(value + x)) == 0)
			return 0;
	}
	return 1;
}

int skipUntilLimit(char *ptr, int limit) {
	int x = 0; 
	for (x; x < limit; x++) {
		if ((*(ptr + x) == SPACE) || (*(ptr + x) == COMMA) ||
			(*(ptr + x) == TAB) || (*(ptr + x) == NLINE))
			return x;
	}
	return x;
}

struct Token newUnknownToken(char *word, int size)
{
	int digit = isNumber(word, size);
	if (digit > 0) {
		if (digit == INT_TYPE)
			return newValToken(INT_ID, word, size);
		else
			return newValToken(DOUBLE_ID, word, size);
	}
	else {
		int string = isString(word, size);
		if (string == 1) {
			return newValToken(STRING_ID, word, size);
		}
		else {
			return newValToken(MIX_STRING_ID, word, size);
		}
	}
}

struct Token newToken(char *type, char *value)
{
	struct Token retval;
	retval.type = type;
	retval.value = value;
	return retval;
}

struct Token newValToken(char *type, char *value, int valsize)
{
	char *nval = calloc(valsize + 1, sizeof(char));
	memcpy(nval, value, valsize);

	struct Token retval;
	retval.type = type;
	retval.value = nval;
	return retval;
}

struct Token newTypeToken(char *type)
{
	struct Token retval;
	retval.type = type;
	retval.value = type;
	return retval;
}

struct Token lexWord(char *word, int size, struct LinkToken *tokenizer)
{
	struct LinkToken *tmphead = tokenizer;
	struct LinkLex *tmpsons;
	while (tokenizer != NULL) {
		tmpsons = tokenizer->sons;
		if (strncmp(word, tokenizer->id, strlen(tokenizer->id)) == 0
				&& (strlen(tokenizer->id) == size)) {
			struct Token retval = newTypeToken(tokenizer->id);
			tokenizer = tmphead;
			return retval;
		}
		while (tokenizer->sons != NULL) {
			if (strncmp(tokenizer->sons->value, word, size) == 0 && 
					strlen(tokenizer->sons->value) == size) {
				struct Token retval = newToken(tokenizer->id, 
					tokenizer->sons->value);
				tokenizer->sons = tmpsons;
				tokenizer = tmphead;
				return retval;
			}
			else {
				tokenizer->sons = tokenizer->sons->next;
			}
		}
		tokenizer->sons = tmpsons;
		tokenizer = tokenizer->next;
	}
	tokenizer = tmphead;
	return newUnknownToken(word, size);
}



/*
Reads a whole string of characters until it reaches the limit passed
trough the parameters and returns an array of Tokens.
*/
struct TokenStream *LexInput(char *sentence, int sensize, 
	struct LinkToken *tokenizer) 
{
	struct TokenStream *retval = malloc(sizeof(struct TokenStream));
	retval->size = 0;
	int x = 0; 
	struct Token *tokens = NULL;
	for (; x < sensize; x++, sentence++) {
		if ((*sentence != SPACE) && (*sentence != COMMA) &&
		        (*sentence != TAB) && (*sentence != NLINE)) {
			int len = 
				skipUntilLimit(sentence, sensize - x);
			retval->size++;
			tokens = realloc(tokens, sizeof(struct Token) *
				retval->size);
			*(tokens + retval->size - 1) = lexWord(sentence, len, 
				tokenizer);
			
			x += len;
			sentence += len;
		}
	}
	retval->tokens = tokens;
	return retval;
}

void destroyTokenStream(struct TokenStream *ptr)
{
	for (int x = 0; x < ptr->size; x++) {
		if ((strcmp((ptr->tokens + x)->type, DOUBLE_ID) == 0) ||
				(strcmp((ptr->tokens + x)->type, INT_ID) == 0) ||
				(strcmp((ptr->tokens + x)->type, STRING_ID) == 0) ||
				(strcmp((ptr->tokens + x)->type, MIX_STRING_ID) == 0)) {
			free((ptr->tokens + x)->value);
		}
	}
	free(ptr->tokens);
	free(ptr);
}

void destroyToken(struct Token *ptr)
{
	if ((strcmp(ptr->type, DOUBLE_ID) == 0) ||
			(strcmp(ptr->type, INT_ID) == 0) ||
			(strcmp(ptr->type, STRING_ID) == 0) ||
			(strcmp(ptr->type, MIX_STRING_ID) == 0)) {
		free(ptr->value);
	}
}

void printTokenStream(struct TokenStream *ptr)
{
	for (int x = 0, y = 0; x < ptr->size; x++, y++) {
		printf("%s", (ptr->tokens + x)->type);
		printf("(%s), ", (ptr->tokens + x)->value);
		if (y > 10) {
			y = 0;
			printf("\n");
		}
	}
}

/*
Less memory consuming functions. (mem_)
*/
struct Token mem_lexWord(char *word, int size, struct mem_LinkToken 
	*tokenizer)
{
	struct mem_LinkToken *tmphead = tokenizer;
	while (tokenizer != NULL) {
		if ((strncmp(tokenizer->id, word, size) == 0) &&
				(strlen(tokenizer->id) == size)) {
			struct Token retval = newTypeToken(tokenizer->id);
			tokenizer = tmphead;
			return retval;
		}
		tokenizer = tokenizer->next;
	}
	tokenizer = tmphead;
	return newUnknownToken(word, size);
}

struct TokenStream *mem_LexInput(char *sentence, int sensize,
	struct mem_LinkToken *tokenizer)
{
	struct TokenStream *retval = malloc(sizeof(struct TokenStream));
	retval->size = 0;
	int x = 0;
	struct Token *tokens = NULL;
	for (; x < sensize; x++, sentence++) {
		if ((*sentence != SPACE) && (*sentence != COMMA) &&
			(*sentence != TAB) && (*sentence != NLINE)) {
			int len =
				skipUntilLimit(sentence, sensize - x);
			retval->size++;
			tokens = realloc(tokens, sizeof(struct Token) *
				retval->size);
			*(tokens + retval->size - 1) = mem_lexWord(sentence, len,
				tokenizer);

			x += len;
			sentence += len;
		}
	}
	retval->tokens = tokens;
	return retval;
}

