#include "Lexer.h"

char *DOUBLE_ID = "DBL";
char *INT_ID = "INT";
char *STRING_ID = "STR";
char *LIT_STRING_ID = "LIT_STR";
char *NO_MATCH_ERR = "NME";

/*
lexInput and mem_lexInput are the core of the lexer for their respective
tokenizer model. They work in exactly the same way with the only difference
being the tokenizer. The function looks for the biggest match left to
right inside the string. It does this by trying to match the whole
string and decreasing its size by one each time it does not find a
match. Decreasing the size each time by one ensures that whenever a match
is found it will be the biggest one possible. Once a match is found it is
"consumed" and the starting index of the string is moved to the right 'n'
spaces where 'n' is the size of the match found (if "Hi" is found the
starting index will be moved two spaces to the right). If  no match is
found after iterating trough the whole string then the string's starting
index is moved one space to the right and the character to the left is
discarded and the process begins all over again until the starting index
is the same as the string's size. This lexer will only match the strings
found inside the tokenizer and the built in datatypes like int, double,
string and string literal.
*/

/*
Checks whether a stream of characters of n size is 
a number or not. Numbers validated by this method may
have up to one minus sign and a decimal point. This method
returns INT_TYPE if the char stream is an integer or 
DOUBLE_TYPE if the char stream is a double. Otherwise it
returns cero if no match is found.
*/
int isNumber(char *value, int size)
{
	if (size <= 0)
		return 0;

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
Checks whether a stream of characters of n size is a
string or not. Valid strings may only contain letters. 
Aa - Zz. Returns one if the char stream is a valid string
otherwise it returns cero.
*/
int isString(char *value, int size) 
{
	if (size <= 0)
		return 0;

	for (int x = 0; x < size; x++) {
		if (isalpha(*(value + x)) == 0)
			return 0;
	}
	return 1;
}

/*
Checks whether a char stream with n size is a string literal or not. 
String literals are considered as everything between two '"'.
*/
int isLiteral(char *value, int size)
{
	if (size <= 1)
		return 0;

	if ((*value != '"') || (*(value + size - 1) != '"'))
		return 0;

	return 1;
}

/*
Tries to match a char stream of n size with one of
the built in data types consisting of integers, doubles
or strings and literals. Returns their respective IDs if a 
match is found. Otherwise it returns the NO_MATCH_ERR ID.
*/
struct Token builtInMatch(char *value, int size)
{
	int flag;
	flag = isNumber(value, size);
	if (flag > 0) {
		if (flag == INT_TYPE)
			return newValToken(INT_ID, value, size);
		else
			return newValToken(DOUBLE_ID, value, size);
	}
	else {
		flag = isString(value, size);
		if (flag != 0) {
			return newValToken(STRING_ID, value, size);
		}
		else {
			flag = isLiteral(value, size);
			if (flag != 0)
				return newValToken(LIT_STRING_ID, value,
					size);
			else
				return newTypeToken(NO_MATCH_ERR);
		}
	}
}

/*
Tries to match a stream of chars with the built in datatypes,
integer, double and string. Returns a token with the respective
id and value if a match is found or one with a NO_MATCH_ERR id
if no match is found.
*/
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
			return newValToken(NO_MATCH_ERR, word, size);
		}
	}
}

/*
Creates a new Token that simply points to the char ptrs
passed trough the parameters. Pointer values are NOT copied,
the struct just points to those pointers.
*/
struct Token newToken(char *type, char *value)
{
	struct Token retval;
	retval.type = type;
	retval.value = value;
	return retval;
}

/*
Creates a new Token where the values of the value ptr ARE COPIED
and the token just points to the token pointer passed trough the
parameters.
*/
struct Token newValToken(char *type, char *value, int valsize)
{
	char *nval = calloc(valsize + 1, sizeof(char));
	memcpy(nval, value, valsize);

	struct Token retval;
	retval.type = type;
	retval.value = nval;
	return retval;
}

/*
Creates a new Token where both its token and values point to the
pointer passed trough the parameters. Commonly used when the Token
its is own Lexeme.
*/
struct Token newTypeToken(char *type)
{
	struct Token retval;
	retval.type = type;
	retval.value = type;
	return retval;
}


/*
Frees all the memory used by a single token.
*/
void destroyToken(struct Token *ptr)
{
	if ((strcmp(ptr->type, DOUBLE_ID) == 0) ||
		(strcmp(ptr->type, INT_ID) == 0) ||
		(strcmp(ptr->type, STRING_ID) == 0) ||
		(strcmp(ptr->type, NO_MATCH_ERR) == 0)) {
		free(ptr->value);
	}
}

/*
Frees all the memory used by the TokenStream. It just frees the
pointers, not the values pointed to except where the token has the
token of a built in datatype in which case it does free the
char array the value points to (a copy of the original value).
*/
void destroyTokenStream(struct TokenStream *ptr)
{
	for (int x = 0; x < ptr->size; x++) {
		destroyToken((ptr->tokens + x));
	}
	free(ptr->tokens);
	free(ptr);
}

/*
Prints the TokenStream, used for debugging. Modes should be passed
as ints. Mode one prints just the token IDs, mode two prints just
the token values and mode three prints both IDs and their values.
*/
void printTokenStream(struct TokenStream *ptr, char format)
{
	if ((format == 1) || (format == '1')) {
		format = 0;
		for (int x = 0, y = 0; x < ptr->size; x++, y++) {
			if (format == 12) {
				printf("\n");
				format = 0;
			}
			printf("<\"%s\"> ", (ptr->tokens + x)->type);
			format++;
		}
	}
	else if ((format == 2) || (format == '2')) {
		format = 0;
		for (int x = 0, y = 0; x < ptr->size; x++, y++) {
			if (format == 6) {
				printf("\n");
				format = 0;
			}
			printf("<\"%s\"> ", (ptr->tokens + x)->value);
			format++;
		}
	}
	else if ((format == 3) || (format == '3')) {
		format = 0;
		for (int x = 0, y = 0; x < ptr->size; x++, y++) {
			if (format == 4) {
				printf("\n");
				format = 0;
			}
			printf("<TYPE: \"%s\"", (ptr->tokens + x)->type);
			printf(" VALUE: \"%s\"> ", (ptr->tokens + x)->value);
			format++;
		}
	}
}
 
/*------------------------------------------
Normal memory usage functions where tokens
can have lexemes.
---------------------------------------------*/
/*
Tries to match a char stream of nsize ONLY with the token ID and its
lexemes. Returns a token with NO_MATCH_ERR ID if no match is found.
*/
struct Token tokenOnlyMatch(char *word, int wrdsize, 
	struct LinkToken *tok) 
{
	struct LinkToken *tmphead = tok;
	struct LinkLex *tmpchild;
	while (tok != NULL) {
		if ((strncmp(word, tok->id, wrdsize) == 0) &&
			(strlen(tok->id) == wrdsize)) {
			return newTypeToken(tok->id);
		}
		tmpchild = tok->sons;
		while (tok->sons != NULL) {
			if ((strncmp(word, tok->sons->value, wrdsize) == 0) &&
				(strlen(tok->sons->value) == wrdsize)) {
				return newToken(tok->id, tok->sons->value);
			}
			tok->sons = tok->sons->next;
		}
		tok->sons = tmpchild;
		tok = tok->next;
	}
	tok = tmphead;
	return newTypeToken(NO_MATCH_ERR);
}

/*See the huge comment at the top.*/
struct TokenStream *lexInput(char *word, int wrdsize,
	struct LinkToken *tok)
{
	struct TokenStream *stream = malloc(sizeof(struct TokenStream));
	stream->size = 0;
	stream->tokens = NULL;
	int nstart = 0;
	int nsize = wrdsize;

	struct Token token;
	while (nstart < wrdsize) {
		if (nsize == 0) {
			word++;
			nstart++;
			nsize = wrdsize - nstart;
		}
		token = tokenOnlyMatch(word, nsize, tok);
		if (strcmp(token.type, NO_MATCH_ERR) != 0) {
			stream->size++;
			stream->tokens = realloc(stream->tokens,
				sizeof(struct TokenStream) * stream->size);
			*(stream->tokens + stream->size - 1) = token;
			word += nsize;
			nstart += nsize;
			nsize = wrdsize - nstart;
		}
		else {
			token = builtInMatch(word, nsize);
			if (strcmp(token.type, NO_MATCH_ERR) != 0) {
				stream->size++;
				stream->tokens = realloc(stream->tokens,
					sizeof(struct TokenStream) * stream->size);
				*(stream->tokens + stream->size - 1) = token;
				word += nsize;
				nstart += nsize;
				nsize = wrdsize - nstart;
			}
			else
				nsize--;
		}
	}
	return stream;
}

/*---------------------------------------
Less memory consuming functions. (mem_)
---------------------------------------*/
/*
Tries to match a char stream of n size ONLY with the tokens found inside
the tokenizer.
*/
struct Token mem_tokenOnlyMatch(char *word, int wrdsize, struct mem_LinkToken *tok)
{
	struct mem_LinkToken *tmphead = tok;
	while (tok != NULL) {
		if ((strncmp(tok->id, word, wrdsize) == 0) &&
			(strlen(tok->id) == wrdsize))
			return newTypeToken(tok->id);
		else
			tok = tok->next;
	}
	tok = tmphead;
	return newTypeToken(NO_MATCH_ERR);
}

/*See the huge comment at the top of this header.*/
struct TokenStream *mem_lexInput(char *word, int wrdsize,  
	struct mem_LinkToken *tok)
{
	struct TokenStream *stream = malloc(sizeof(struct TokenStream));
	stream->size = 0;
	stream->tokens = NULL;
	int nstart = 0;
	int nsize = wrdsize;

	struct Token token;
	while (nstart < wrdsize) {
		if (nsize == 0) {
			word++;
			nstart++;
			nsize = wrdsize - nstart;
		}
		token = mem_tokenOnlyMatch(word, nsize, tok);
		if (strcmp(token.type, NO_MATCH_ERR) != 0) {
			stream->size++;
			stream->tokens = realloc(stream->tokens, 
				sizeof(struct TokenStream) * stream->size);
			*(stream->tokens + stream->size - 1) = token;
			word += nsize;
			nstart += nsize;
			nsize = wrdsize - nstart;
		}
		else {
			token = builtInMatch(word, nsize);
			if (strcmp(token.type, NO_MATCH_ERR) != 0) {
				stream->size++;
				stream->tokens = realloc(stream->tokens,
					sizeof(struct TokenStream) * stream->size);
				*(stream->tokens + stream->size - 1) = token;
				word += nsize;
				nstart += nsize;
				nsize = wrdsize - nstart;
			}
			else
				nsize--;
		}
	}
	return stream;
}
