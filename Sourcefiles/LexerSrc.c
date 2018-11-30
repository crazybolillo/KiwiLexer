#include "Lexer.h"

char *DOUBLE_ID = "DBL";
char *INTEGER_ID = "INT";
char *STRING_ID = "STR";
char *CONST_STRING_ID = "CONST_STR";

char *NO_MATCH_ERR = "NME";
char *EOF_ID = "KEOF";


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
string or not. Valid strings may only contain letters and underscores. 
Aa - Zz. Returns one if the char stream is a valid string
otherwise it returns cero.
*/
int isString(char *value, int size) 
{
	if (size <= 0)
		return 0;

	for (int x = 0; x < size; x++) {
		if ((isalpha(*(value + x)) == 0) && (*(value + x) != '_'))
			return 0;
	}
	return 1;
}

/*
Checks whether a char stream with n size is a string literal or not. 
String literals are considered everything between two double or single
quotes. It may contain double or single quotes inside as long as they are
properly escaped. eg ("\"");
*/
int isLiteral(char *value, int size)
{
    char delimitor;
	if (size <= 1)
		return 0;
		
	if (*value == '"') {
	    delimitor = '"';
    }
    else if (*value == '\'') {
        delimitor = '\'';
    }
    else return 0;
    
    /*If the end of the string is not equal to the delimitor or the
    delimitor is escaped by a backlash which is not escaped.*/
    if ((*(value + size - 1) != delimitor)) {
        return 0;
    }
    if ((*(value + size - 2) == '\\')) {
        if (size < 4) {
            return 0;
        }
        else {
            int slashcount = 1;
            for(int x = size - 3; x > 0; x--) {
                if (*(value + x) == '\\') {
                    slashcount++;
                }     
                else {
                    break;
                }
            }
            if(slashcount % 2 != 0)
                return 0;
        }
    }
    
    value++;
    for (int x = 1; x < (size - 1); x++, value++) {
        if((*value == delimitor) && (*(value - 1) != '\\'))
            return 0;
    }
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
			return newTypeToken(INTEGER_ID);
		else
			return newTypeToken(DOUBLE_ID);
	}
	else {
		flag = isString(value, size);
		if (flag != 0) {
			return newTypeToken(STRING_ID);
		}
		else {
			flag = isLiteral(value, size);
			if (flag != 0)
				return newTypeToken(CONST_STRING_ID);
			else
				return newTypeToken(NO_MATCH_ERR);
		}
	}
}

/*
Skips all whitespace (tabs, newlines and spaces) until it finds
a non white space character or reaches the limit.  Returns how many chars were skipped.
MODIFIES the char pointer passed trough the parameters so it points
to the next non-whitespace char. 
*/
int skipWhiteSpace(char **str, int limit)
{
	int x = 0;
	for (; x < limit; x++, (*str)++) {
		if ((**str != '\n') && (**str != '\t') && (**str != ' '))
			return x;
	}
	return x;
}

/*
Tries to match a stream of chars with the built in datatypes,
integer, double and string. Returns a nextToken with the respective
id and value if a match is found or one with a NO_MATCH_ERR id
if no match is found.
*/
struct Token newUnknownToken(char *word, int size)
{
	int flag = isNumber(word, size);
	if (flag > 0) {
		if (flag == INT_TYPE)
			return newValToken(INTEGER_ID, word, size);
		else
			return newValToken(DOUBLE_ID, word, size);
	}
	else {
		flag = isString(word, size);
		if (flag == 1) {
			return newValToken(STRING_ID, word, size);
		}
	    flag = isLiteral(word, size);
	    if(flag == 1)
		    return newValToken(CONST_STRING_ID, word, size);
		else
		    return newTypeToken(NO_MATCH_ERR);
			       
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
(memcpy) and the type of the token just points to the one passed 
trough the parameters.
*/
struct Token newValToken(char *type, char *value, int valsize)
{
	struct Token retval;
	retval.type = type;
	retval.value = calloc(valsize + 1, sizeof(char));
	memcpy(retval.value, value, valsize);
	return retval;
}

/*
Creates a new Token where both its nextToken and values point to the
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
Frees all the memory used by a single Token. 
*/
void destroyToken(struct Token *ptr)
{
	if ((strcmp(ptr->type, DOUBLE_ID) == 0) ||
		(strcmp(ptr->type, INTEGER_ID) == 0) ||
		(strcmp(ptr->type, STRING_ID) == 0) ||
		(strcmp(ptr->type, CONST_STRING_ID) == 0)) {
		free(ptr->value);
	}
}

/*
Frees all the memory used by the TokenStream. It just frees the
pointers, not the values pointed to except where the nextToken has the
nextToken of a built in datatype in which case it does free the
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
as ints. Mode one prints just the nextToken IDs, mode two prints just
the nextToken values and mode three prints both IDs and their values.
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
			printf("< %s > ", (ptr->tokens + x)->type);
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
			printf("< %s > ", (ptr->tokens + x)->value);
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
			printf(" VALUE: %s > ", (ptr->tokens + x)->value);
			format++;
		}
	}
}

/*
Tries to match a char stream of n size ONLY with the tokens found inside
the tokenizer.
*/
struct Token mem_tokenOnlyMatch(char *word, int wrdsize, 
	struct mem_LinkToken *tok)
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


/*
Adds the token passed trough the parameters to the TokenStream. 
This memory handles all necessary memory allocation. 
*/
void appendToken(struct TokenStream *stream,
	struct Token value)
{
	stream->size++;
	stream->tokens = realloc(stream->tokens, 
		sizeof(struct Token) * stream->size);
	*(stream->tokens + stream->size - 1) = value;
}

/*
Starts lexing just one char and increases the size to be lexed by one
with each iteration. If a match is found it still increases the size
to be lexed by one. It stops until a match is no longer found. Once a 
match is no longer found the last match to be found is consumed and
the process starts all over again. This ensures that the consumed match
is the biggest match possible. If no match is found and the size that
is trying to be lexed is equal to the input size then the first char of
the input is discarded and the process begins all over again until the
starting index is the same as the input size. It also skips over any
trailing whitespaces.
*/
struct TokenStream *mem_lexAll(struct KiwiInput *input,
	struct mem_LinkToken *tok)
{
	struct TokenStream *stream = malloc(sizeof(struct TokenStream));
	stream->size = 0;
	stream->tokens = NULL;

	struct Token token;
	while (1) {
		token = mem_lexNext(input, tok);
		if (strcmp(token.type, EOF_ID) != 0) 
			appendToken(stream, token);
		else
			return stream;
	}
	return stream;
	
}

/*Returns the next match found inside the text passed trough the 
paremeters. Updates the char pointer and readsize inside KiwiInput
accordingly.*/      
struct Token mem_lexNext(struct KiwiInput *input,
	struct mem_LinkToken *tokenizer)
{
	/*
	0x00 means there is no match.
	0xAA means the match belongs to the alphabet.
	0xFF means the match belongs to one of the built in datatypes.
	*/
	unsigned char matchflag = 0x00;
	unsigned int nsize = 1;
	struct Token nextToken;
	struct Token prevToken;
		
	while (1) {
		if (nsize > input->textSize - input->readSize) {
			if (input->textSize - input->readSize <= 0) {
				return newTypeToken(EOF_ID);
			}
			if (input->textSize - input->readSize == 1) {
				if (matchflag != 0x00) {
					if (matchflag == 0xAA) {
						input->readSize++;
						input->text++;
						return prevToken;
					}
					if (matchflag == 0xFF) {
						prevToken =  newValToken(prevToken.type,
							input->text, 1);
						input->readSize++;
						input->text++;
						return prevToken;
					}
					return newTypeToken(EOF_ID);
				}
			}
			else {
				input->text++;
				input->readSize++;
				nsize = 1;
			}
		}
		nextToken = mem_tokenOnlyMatch(input->text, nsize, tokenizer);
		if (strcmp(nextToken.type, NO_MATCH_ERR) != 0) {
			matchflag = 0xAA;
			prevToken = nextToken;
			nsize++;
			continue;
		}
		nextToken = builtInMatch(input->text, nsize);
		if (strcmp(nextToken.type, NO_MATCH_ERR) != 0) {
			matchflag = 0xFF;
			prevToken = nextToken;
			nsize++;
			continue;
		}
		if (matchflag != 0x00)
			break;
		else
			nsize++;
	}
	if (matchflag == 0xAA) {
		int len = nsize - 1;
		input->text += len;
		input->readSize += len;
		len = skipWhiteSpace(&(input->text),
			input->textSize - input->readSize);
		input->readSize += len;
		return prevToken;
	}
	else if (matchflag == 0xFF) {
		int len = nsize - 1;
		prevToken = newValToken(prevToken.type, input->text, len);
		input->text += len;
		input->readSize += len;
		len = skipWhiteSpace(&(input->text),
			input->textSize - input->readSize);
		input->readSize += len;
		return prevToken;
	}
	return newTypeToken(EOF_ID);
}
