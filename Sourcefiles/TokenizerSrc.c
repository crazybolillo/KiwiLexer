#include "Tokenizer.h"


struct Token createToken(char *id, int idsize)
{
	struct Token retval;
	retval.id = calloc(sizeof(char) * (idsize + 1), 1);
	memcpy(retval.id, id, idsize);
	retval.size = 0;
	retval.words = NULL;
	return retval;
}

void addWord(struct Token *token, char *lexeme, int lexsize) {
	token->size++;
	token->words = realloc(token->words, sizeof(char *) * token->size);
	char *nword = calloc(sizeof(char) * (lexsize + 1), 1);
	memcpy(nword, lexeme, lexsize);
	*(token->words + (token->size - 1)) = nword;
}


/*
Reads a whole file and returns a char buffer with the text it contained. The
FILE pointer must be opened in text form. Sets the integer pointer passed
trough the parameters to the numbers of chars read.
*/
char *readAll(FILE *fl, int *size)
{
	char *rdata = NULL;
	int sector = 512;
	int count = 1;
	int dataread;

	do {
		fseek(fl, 0, SEEK_SET);
		rdata = realloc(rdata, sizeof(char) * (sector * count));
		dataread = fread(rdata, 1, (sector * count), fl);
		count++;
	} while (feof(fl) == 0);

	rdata = realloc(rdata, dataread);
	return rdata;
}

/*
Skips all the chars in the char pointer until it finds the char
passed trough the parameters or it reaches the limit passed trough the
parameters. Returns the amount of characters it skipped over if it 
eventually found the char or -1 if it did not find it but reached
the limit. DOES NOT change the pointer passed trough the parameters as
this just recieves a copy of it.
*/
int skipchar(char *ptr, char until)
{
	for (int x = 0;; ptr++, x++) {
		if (*ptr == until)
			return x;
	}
}

struct Token parseToken(char **grammar)
{
	int idlen = skipchar(*grammar, TOKEN_SEP_COMMA);
	struct Token retroot = createToken(*grammar, idlen);
	idlen++; //Skip comma.
	*grammar += idlen;

	int len = 0;
	for (; **grammar != TOKEN_LIMIT; (*grammar)++, len++) {
		if ((**grammar == TOKEN_SEP_COMMA)) {
			addWord(&retroot, *grammar - len, len);
			(*grammar)++;
			len = 0;
			continue;
		}
	}
	addWord(&retroot, *grammar - len, len);
	(*grammar)++;
	return retroot;

}

struct Tokenizer *createTokenizer(char *grammar)
{
	struct Tokenizer *retval = malloc(sizeof(struct Tokenizer));
	retval->tokenCount = 1;

	int len = skipchar(grammar, TOKEN_LIMIT);
	len++;
	grammar += len;
	struct Token *tokenptr = malloc(sizeof(struct Token) * retval->tokenCount);
	*(tokenptr + (retval->tokenCount - 1)) = parseToken(&grammar);

	for (; *grammar != TOKEN_END; grammar++) {
		if (*grammar == TOKEN_LIMIT) {
			grammar++;
			retval->tokenCount++;
			tokenptr = realloc(tokenptr, sizeof(struct Token) 
				* retval->tokenCount);
			*(tokenptr + (retval->tokenCount - 1)) = parseToken(&grammar);
		}
	}
	retval->tokens = tokenptr;
	return retval;
}

void destroyTokenizer(struct Tokenizer *tokenizer)
{
	for (int x = 0; x < tokenizer->tokenCount; x++) {
		for (int z = 0; z < (tokenizer->tokens + x)->size; z++) {
			free(*((tokenizer->tokens + x)->words + z));
		}
		free((tokenizer->tokens + x )->id);
	}
	free(tokenizer->tokens);
	free(tokenizer);
}

void printTokenizer(struct Tokenizer *tokenizer)
{
	for (int x = 0; x < tokenizer->tokenCount; x++) {
		printf("%s\n", (tokenizer->tokens + x)->id);
		for (int z = 0; z < (tokenizer->tokens + x)->size; z++) {
			printf("%s ", *((tokenizer->tokens + x)->words + z));
		}
		printf("\n-------------------\n");
	}
}
