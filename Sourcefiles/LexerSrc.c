#include "Lexer.h"

char *DOUBLE_ID = "DBL";
char *INTEGER_ID = "INT";
char *STRING_ID = "STR";
char *CONST_STRING_ID = "CONST_STR";

char *NO_MATCH_ERR = "KNME"; 
char *EOF_ID = "KEOF";

/*Can't  use "char *BUILT_IN_TYPES[]" because apparently the
corresponding values arent constant enough so I use 
memory adresses instead which somehow are more constant.*/
char **BUILT_IN_TYPES[] = {&DOUBLE_ID, &INTEGER_ID, &STRING_ID,
 &CONST_STRING_ID, &NO_MATCH_ERR, &EOF_ID};

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
				return NO_MATCH_ERR;
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
struct Token *newToken(char *type, char *value, struct MemBlock *mem)
{
	struct Token *retval = kimalloc(sizeof(struct Token), mem);
	if (retval == NULL) {
		return NULL;
	}
	retval->type = type;
	retval->value = value;
	retval->next = NULL;
	return retval;
}

/*
Creates a new Token where the values of the value ptr ARE COPIED 
(memcpy) and the type of the token just points to the one passed 
trough the parameters.
*/
struct Token *newValToken(char *type, char *value, int valsize,
	struct MemBlock *mem)
{
	struct Token *retval = kimalloc(sizeof(struct Token), mem);
	if (retval == NULL) {
		return NULL;
	}
	retval->type = type;
	retval->value = kicalloc(valsize + 1 * sizeof(char), mem);
	if (retval->value == NULL) {
		popMemory(mem, sizeof(struct Token));
		return NULL;
	}
	memcpy(retval->value, value, valsize);
	retval->next = NULL;
	return retval;
}

/*
Creates a new Token where both its nextToken and values point to the
pointer passed trough the parameters. Commonly used when the Token
its is own Lexeme.
*/
struct Token *newTypeToken(char *type, struct MemBlock *mem)
{
	struct Token *retval = kimalloc(sizeof(struct Token), mem);
	if (retval == NULL) {
		return NULL;
	}
	retval->type = type;
	retval->value = type;
	retval->next = NULL;
	return retval;
}

/*
Prints the TokenStream, used for debugging. Modes should be passed
as ints. Mode one prints just the nextToken IDs, mode two prints just
the nextToken values and mode three prints both IDs and their values.
*/
void printTokenStream(struct Token *ptr, char format)
{
	struct Token *tmphead;
	tmphead = ptr;
	if ((format == 1) || (format == '1')) {
		format = 0;
		while (ptr != NULL) {
			printf("<TYPE: \"%s\"> ", ptr->type);
			ptr = ptr->next;
			format++;
			if (format > 6) {
				printf("\n");
				format = 0;
			}
		}
	}
	else if ((format == 2) || (format == '2')) {
		format = 0;
		while (ptr != NULL) {
			printf("<VALUE: \" %s \"> ", ptr->value);
			ptr = ptr->next;
			format++;
			if (format > 5) {
				printf("\n");
				format = 0;
			}
		}
	}
	else if ((format == 3) || (format == '3')) {
		format = 0;
		while (ptr != NULL) {
			printf("<TYPE: \"%s\" --- VALUE: \" %s \"> ",
				ptr->type, ptr->value);
			ptr = ptr->next;
			format++;
			if (format > 3) {
				printf("\n");
				format = 0;
			}
		}
	}
	ptr = tmphead;
}

/*
Adds the node to the ending of the LinkedList
*/
void appendToken(struct Token **head, struct Token *node) {
	if (*head == NULL) {
		*head = node;
	}
	else if ((*head)->next == NULL) {
		(*head)->next = node;
	}
	else {
		struct Token *tmphead = *head;
		while ((*head)->next != NULL) {
			(*head) = (*head)->next;
		}
		(*head)->next = node;
		(*head) = tmphead;
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
	return NO_MATCH_ERR;
}

/*
Starts lexing just one char and increases the size to be lexed by one
with each iteration. If a match is found it still increases the size
to be lexed by one. It stops until a match is no longer found. Once a
match is no longer found the last match to be found is consumed. If 
memory allocation fails during the lexing a NULL pointer is returned 
and the pointer inside the input is not moved.
*/
struct Token *lexNext(struct KiwiInput *input,
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
	struct Token *retval;
		
	while (1) {
		if (nsize > input->textSize - input->readSize) {
			if (input->textSize - input->readSize <= 0) {
				return newTypeToken(EOF_ID, mem);
			}
			if (input->textSize - input->readSize == 1) {
				if (matchflag != 0x00) {
					if (matchflag == 0xAA) {
						retval = newTypeToken(prevToken, mem);
						if (retval == NULL) {
							return NULL;
						}
						input->readSize++;
						input->text++;
						return newTypeToken(prevToken, mem);
					}
					if (matchflag == 0xFF) {
						retval = newValToken(prevToken,
							input->text, 1, mem);
						if (retval == NULL) {
							return NULL;
						}
						input->readSize++;
						input->text++;
						return retval;
					}
					return newTypeToken(EOF_ID, mem);
				}
			}
			else {
				input->text++;
				input->readSize++;
				nsize = 1;
			}
		}
		nextToken = tokenOnlyMatch(input->text, nsize, tokenizer);
		if (nextToken == NULL) {
			return NULL;
		}
		if (strcmp(nextToken, NO_MATCH_ERR) != 0) {
			matchflag = 0xAA;
			prevToken = nextToken;
			nsize++;
			continue;
		}
		nextToken = builtInMatch(input->text, nsize);
		if (nextToken == NULL) {
			return NULL;
		}
		if (strcmp(nextToken, NO_MATCH_ERR) != 0) {
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
		retval = newTypeToken(prevToken, mem);
		if (retval == NULL) {
			return NULL;
		}
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
		if (retval == NULL) {
			return NULL;
		}
		input->text += len;
		input->readSize += len;
		len = skipWhiteSpace(&(input->text),
			input->textSize - input->readSize);
		input->readSize += len;
		return retval;
	}
	return newTypeToken(EOF_ID, mem);
}

/*
Lexes the whole input by repeteadly calling "lexNext
*/
struct Token *lexAll(struct KiwiInput *input,
	struct LinkList *tok, struct MemBlock *mem)
{
	struct Token *head = NULL;
	struct Token *token;
	while (1) {
		token = lexNext(input, tok, mem);
		if (token == NULL) {
			return head;
		}
		if ((strcmp(token->type, EOF_ID) != 0) && 
			(strcmp(token->type, NO_MATCH_ERR) != 0))
			appendToken(&head, token);
		else
			return head;
	}
}
