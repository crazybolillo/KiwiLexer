#include "Lexer.h"

/*
Keeps reading until it finds one of the two characters passed trough
the parameters or reaches the imposed limit used to avoid illegal
memory access.
*/
int skipUntilLimit(char *ptr, int limit) {
	int x = 0; 
	for (x; x < limit; x++) {
		if ((*(ptr + x) == SPACE) || (*(ptr + x) == COMMA) ||
			(*(ptr + x) == TAB))
			return x;
	}
	return x;
}

struct Token createToken(char *type, char *value, int valsize)
{
	char *ntype = calloc(strlen(type) + 1, sizeof(char));
	memcpy(ntype, type, strlen(type));

	char *nval = calloc(valsize + 1, sizeof(char));
	memcpy(nval, value, valsize);

	struct Token retval;
	retval.type = ntype;
	retval.value = nval;
	return retval;
}

/*
Compares the char pointer passed trough the parameters with all the
known tokens and returns the token category type if a match is found. 
Otherwise it returns a VALUE type.
*/
struct Token tokenizeWord(char *word, int size, struct LinkedToken *tokenizer)
{
	struct LinkedToken *tmphead = tokenizer;
	while (tokenizer != NULL) {
		struct LinkedLexeme *tmpsons = tokenizer->sons;
		while (tokenizer->sons != NULL) {
			if (strncmp(tokenizer->sons->value, word, size) == 0 && 
					strlen(tokenizer->sons->value) == size) {
				struct Token retval = createToken(tokenizer->id, word, size);
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
	
	int digit = isNumber(word, size);
	if (digit > 0) {
		if (digit == INT_TYPE)
			return createToken(INT_ID, word, size);
		else
			return createToken(DOUBLE_ID, word, size);
	}
	else {
		int string = isString(word, size);
		if (string == 1) {
			return createToken(STRING_ID, word, size);
		}
		else{
			return createToken(MIXED_STRING_ID, word, size);
		}
	}
}



/*
Reads a whole string of characters until it reaches the limit passed
trough the parameters and returns an array of Tokens.
*/
struct TokenStream *tokenizeAll(char *sentence, int sensize, 
	struct LinkedToken *tokenizer) 
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
			tokens = realloc(tokens, sizeof(struct Token) * retval->size);
			*(tokens + retval->size - 1) = tokenizeWord(sentence, len, tokenizer);
			
			x += len;
			sentence += len;
		}
	}
	retval->tokens = tokens;
	return retval;
}

/*
Iterates trough the char pointer up n times where
n is the integer passed trough the parameters. Returns 0 
if it is not a string. INT_TYPE(1) if it is an Integer and 
DOUBLE_TYPE(2) if it is a double (has a decimal point). Any 
sequence of characters that represents number and hast at max
one '.' and one '-' will be considered numbers by this function.
*/
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

/*
Returns 0 if the char pointer up to the limit n where n is the
int passed trough the parameters is NOT a string. Returns 1
if the char pointer up to the limit contains ONLY characters Aa-Zz.
*/
int isString(char *value, int size) {
	for (int x = 0; x < size; x++) {
		if (isalpha(*(value + x)) == 0)
			return 0;
	}
	return 1;
}

void destroyTokenStream(struct TokenStream *ptr)
{
	for (int x = 0; x < ptr->size; x++) {
		free((ptr->tokens + x)->type);
		free((ptr->tokens + x)->value);
	}
	free(ptr->tokens);
	free(ptr);
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
