#include "Lexer.h"


/*
Keeps reading until it finds one of the two characters passed trough
the parameters or reaches the imposed limit used to avoid illegal
memory access.
*/
int skipUntilLimit(char *ptr, int limit) {
	int x = 0; 
	for (x; x < limit; x++) {
		if ((*(ptr + x) == SPACE) || (*(ptr + x) == TOKEN_SEP_COMMA) ||
			(*(ptr + x) == TOKEN_SEP_TAB))
			return x;
	}
	return x;
}

struct TokenVal createTokenVal(char *type, char *value, int valsize)
{
	char *ntype = calloc(strlen(type) + 1, sizeof(char));
	memcpy(ntype, type, strlen(type));

	char *nval = calloc(valsize + 1, sizeof(char));
	memcpy(nval, value, valsize);

	struct TokenVal retval;
	retval.type = ntype;
	retval.value = nval;
	return retval;
}

/*
Compares the char pointer passed trough the parameters with all the
known tokens and returns the token category type if a match is found. 
Otherwise it returns a VALUE type.
*/
struct TokenVal tokenizeWord(char *word, int size, struct Tokenizer tokenizer)
{
	for (int x = 0; x < tokenizer.tokenCount; x++) {
		for (int z = 0; z < (tokenizer.tokens + x)->size; z++) {
			if (strncmp(word, *((tokenizer.tokens + x)->words + z), size) == 0)
				return createTokenVal((tokenizer.tokens + x)->id, word, size);
		}
	}
	return createTokenVal("VALUE", word, size);
}



/*
Reads a whole string of characters until it reaches the limit passed
trough the parameters and returns an array of Tokens.
*/
struct TokenVal *tokenizeAll(char *sentence, int sensize, int *size,
	struct Tokenizer tokenizer) 
{
	int x = 0; 
	int tokencount = 0;
	struct TokenVal *retval = NULL;
	for (; x < sensize; x++, sentence++) {
		if ((*sentence != SPACE) && (*sentence != TOKEN_SEP_COMMA) &&
		        (*sentence != TOKEN_SEP_TAB) && (*sentence != TOKEN_SEP_NLINE)) {
			int len = 
				skipUntilLimit(sentence, sensize - x);
			tokencount++;
			retval = realloc(retval, sizeof(struct TokenVal) * tokencount);
			*(retval + tokencount - 1) = tokenizeWord(sentence, len, tokenizer);
			
			x += len;
			sentence += len;
		}
	}
	*size = tokencount;
	return retval;
}

void destroyTokenStream(struct TokenVal *ptr, int size)
{
	for (int x = 0; x < size; x++) {
	    free((ptr + x)->type);
	    free((ptr + x)->value);
	}
	free(ptr);
}

void debug_print_tokens(struct TokenVal *sentence, int size)
{
	for (int x = 0; x < size; x++) {
		printf("%s ---> ", (sentence + x)->type);
		printf("%s\n", (sentence + x)->value);
	}
}
