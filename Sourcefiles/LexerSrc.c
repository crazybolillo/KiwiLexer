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
	return createToken("VALUE", word, size);
}



/*
Reads a whole string of characters until it reaches the limit passed
trough the parameters and returns an array of Tokens.
*/
struct Token *tokenizeAll(char *sentence, int sensize, int *size,
	struct LinkedToken *tokenizer) 
{
	int x = 0; 
	int tokencount = 0;
	struct Token *retval = NULL;
	for (; x < sensize; x++, sentence++) {
		if ((*sentence != SPACE) && (*sentence != COMMA) &&
		        (*sentence != TAB) && (*sentence != NLINE)) {
			int len = 
				skipUntilLimit(sentence, sensize - x);
			tokencount++;
			retval = realloc(retval, sizeof(struct Token) * tokencount);
			*(retval + tokencount - 1) = tokenizeWord(sentence, len, tokenizer);
			
			x += len;
			sentence += len;
		}
	}
	*size = tokencount;
	return retval;
}

void destroyTokenStream(struct Token *ptr, int size)
{
	for (int x = 0; x < size; x++) {
	    free((ptr + x)->type);
	    free((ptr + x)->value);
	}
	free(ptr);
}

void debug_print_tokens(struct Token *sentence, int size)
{
	for (int x = 0; x < size; x++) {
		printf("%s ---> ", (sentence + x)->type);
		printf("%s\n", (sentence + x)->value);
	}
}
