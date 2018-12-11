#include "Lexer.h"

char *DOUBLE_ID = "DBL";
char *INTEGER_ID = "INT";
char *STRING_ID = "STR";
char *CONST_STRING_ID = "CONST_STR";

char *ERR_ID = "KNME"; 
char *EOF_ID = "KEOF";

/*Can't  use "char *BUILT_IN_TYPES[]" because apparently the
corresponding values arent constant enough so I use 
memory adresses instead which somehow are more constant.*/
char **BUILT_IN_TYPES[] = {&DOUBLE_ID, &INTEGER_ID, &STRING_ID,
 &CONST_STRING_ID, &ERR_ID, &EOF_ID};

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
match is found. Otherwise it returns the ERR_ID ID.
*/
char *builtInMatch(char *value, int size)
{
	int flag;
	flag = isNumber(value, size);
	if (flag > 0) {
		if (flag == INT_TYPE)
			return INTEGER_ID;
		else
			return DOUBLE_ID;
	}
	else {
		flag = isString(value, size);
		if (flag != 0) {
			return STRING_ID;
		}
		else {
			flag = isLiteral(value, size);
			if (flag != 0)
				return CONST_STRING_ID;
			else
				return ERR_ID;
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
struct Token newValToken(char *type, char *value, int valsize,
	struct MemBlock *mem)
{
	struct Token retval;
	retval.type = type;
	retval.value = symbolTableContains(value, valsize, mem);
	if (retval.value == NULL) {
		retval.value = kicalloc(sizeof(char) * valsize + 1, mem);
		if (retval.value == NULL) {
			return newTypeToken(ERR_ID);
		}
		memcpy(retval.value, value, valsize);
	}
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
Prints the TokenArray, used for debugging. Modes should be passed
as ints. Mode one prints just the nextToken IDs, mode two prints just
the nextToken values and mode three prints both IDs and their values.
*/
void printTokenStream(struct TokenArray *ptr, char format)
{
	if ((format == 1) || (format == '1')) {
		format = 0;
		for (unsigned int x = 0; x < ptr->size; x++, format++) {
			printf("<TYPE: \"%s\">", (ptr->token + x)->type);
			if (format > 6) {
				printf("\n");
				format = 0;
			}
		}
	}
	else if ((format == 2) || (format == '2')) {
		format = 0;
		for (unsigned int x = 0; x < ptr->size; x++, format++) {
			printf("<VALUE: %s >", (ptr->token + x)->value);
			if (format > 4) {
				printf("\n");
				format = 0;
			}
		}
	}
	else if ((format == 3) || (format == '3')) {
		format = 0;
		for (unsigned int x = 0; x < ptr->size; x++, format++) {
			printf("<TYPE: \"%s\" VALUE: %s >",
				(ptr->token + x)->type, (ptr->token + x)->value);
			if (format > 3) {
				printf("\n");
				format = 0;
			}
		}
	}
}

/*
Tries to add the token to the memory block. Returns 0 if it was not
able to add it.*/
char appendToken(struct TokenArray *stream, struct Token node, 
	struct MemBlock *mem) 
{
	struct Token *tok = kimalloc(sizeof(struct Token), mem);
	if (tok == NULL) {
		return 0;
	}
	*tok = node;
	stream->size++;
	return 1;
}

/*
Iterates troughout the symbol table to see if the symbol has already 
been allocated in memory. If it has been allocated in memory it 
returns a pointer to the first element (char) of the symbol (string). 
Strings are NUL terminated and the end of valid strings is signaled
by two NUL (0x00) characters one after the other.
*/
char *symbolTableContains(char *value, int size, struct MemBlock *mem) 
{
	if (mem->used == 0) {
		return NULL;
	}
	char *string = mem->memory - mem->used;
	if (*string == 0x00) {
		return NULL;
	}
	char *lookahead = string;
	int x = 0;
	while(1){
		for (; x < size; x++, lookahead++, value++) {
			if (*lookahead != *value) {
				SKIP_TIL_NUL(lookahead);
				if (*(lookahead + 1) == 0x00) {
					return NULL;
				}
				else {
					lookahead++;
					string = lookahead;
					value -= x;
					x = 0;
					continue;
				}
			}
		}
		if (*(lookahead + 1) == 0x00) {
			return string;
		}
		else {
			SKIP_TIL_NUL(lookahead);
			if (*(lookahead + 1) == 0x00) {
				return NULL;
			}
			else {
				lookahead++;
				string = lookahead;
				value -= size;
				x = 0; 
				continue;
			}
		}
	}
}


/*
Tries to match a char stream of n size ONLY with the tokens found inside
the tokenizer.
*/
char *tokenOnlyMatch(char *word, int wrdsize, 
	struct LinkList *tok)
{
	struct LinkList *tmphead = tok;
	while (tok != NULL) {
		if ((strncmp(tok->value, word, wrdsize) == 0) &&
			(strlen(tok->value) == wrdsize))
			return tok->value;
		else
			tok = tok->next;
	}
	tok = tmphead;
	return ERR_ID;
}

/*
Starts lexing just one char and increases the size to be lexed by one
with each iteration. If a match is found it still increases the size
to be lexed by one. It stops until a match is no longer found. Once a
match is no longer found the last match to be found is consumed. If 
memory allocation fails during the lexing a NULL pointer is returned 
and the pointer inside the input is not moved.
*/
struct Token lexNext(struct KiwiInput *input,
	struct LinkList *tokenizer, struct MemBlock *mem)
{
	/*
	0x00 means there is no match.
	0xAA means the match belongs to the alphabet.
	0xFF means the match belongs to one of the built in datatypes.
	*/
	unsigned char matchflag = 0x00;
	unsigned int nsize = 1;
	char *nextToken;
	char *prevToken = NULL;
	struct Token retval;
		
	while (1) {
		if (nsize > input->textSize - input->readSize) {
			if (input->textSize - input->readSize <= 0) {
				return newTypeToken(EOF_ID);
			}
			if (input->textSize - input->readSize == 1) {
				if (matchflag != 0x00) {
					if (matchflag == 0xAA) {
						retval = newTypeToken(prevToken);
					}
					if (matchflag == 0xFF) {
						retval = newValToken(prevToken,
							input->text, 1, mem);
					}
					input->readSize++;
					input->text++;
					return retval;
				}
				input->readSize++;
				input->text++;
				return newTypeToken(EOF_ID);
			}
			else {
				input->text++;
				input->readSize++;
				nsize = 1;
			}
		}
		nextToken = tokenOnlyMatch(input->text, nsize, tokenizer);
		if (strcmp(nextToken, ERR_ID) != 0) {
			matchflag = 0xAA;
			prevToken = nextToken;
			nsize++;
			continue;
		}
		nextToken = builtInMatch(input->text, nsize);
		if (strcmp(nextToken, ERR_ID) != 0) {
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
		retval = newTypeToken(prevToken);
		input->text += len;
		input->readSize += len;
		len = skipWhiteSpace(&(input->text),
			input->textSize - input->readSize);
		input->readSize += len;
		return retval;
	}
	else if (matchflag == 0xFF) {
		int len = nsize - 1;
		retval = newValToken(prevToken, input->text, len, mem);
		input->text += len;
		input->readSize += len;
		len = skipWhiteSpace(&(input->text),
			input->textSize - input->readSize);
		input->readSize += len;
		return retval;
	}
	return newTypeToken(EOF_ID);
}

/*
Lexes the whole input by repeteadly calling "lexNext
*/
struct TokenArray *lexAll(struct KiwiInput *input,
	struct LinkList *tok, struct MemBlock *tokenmem, 
	struct MemBlock *symbolmem)
{
	struct TokenArray *retval = kimalloc(sizeof(struct TokenArray),
		tokenmem);
	if (retval == NULL) {
		return NULL;
	}
	retval->token = tokenmem->memory;
	/*if (retval->token == NULL) {
		return NULL;
	}
	popMemory(sizeof(struct Token), tokenmem);*/
	struct Token token;
	char append;
	while (1) {
		token = lexNext(input, tok, symbolmem);
		if ((strcmp(token.type, EOF_ID) == 0) ||
			(strcmp(token.type, ERR_ID) == 0)) {
			return retval;
		}
		append = appendToken(retval, token, tokenmem);
		if (append == 0) {
			return retval;
		}
	}
}
