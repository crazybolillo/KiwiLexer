#include "Tokenizer.h"


/*
Creates a LinkedChild 
*/
struct TokenValue *create_linkchild(char *value)
{
	struct TokenValue *retval = malloc(sizeof(struct TokenValue));
	retval->value = value;
	retval->next = NULL;
	return retval;
}

/*
Creates an empty LinkedParent list with an identifier. Branches represent
tokens so the identifier will indicate which type of tokens
this branch contains. This method copies the id passed trough
the parameters up until the point indicated by the 'idsize' variable
and terminates it with a NUL character.
*/
struct Token *createToken(char *id, int idsize)
{
	struct Token *retval = malloc(sizeof(struct Token));
	retval->next = NULL;
	retval->id = calloc(idsize + 1, sizeof(char));
	retval->sons = NULL;
	memcpy(retval->id, id, idsize);
	return retval;
}

/*
Returns the index where the LinkedParent with the identifier passed trough the parameters
is found inside the LinkedParent list. Returns -1 if the branch was not found.
*/
int tokenIndex(char *id, int idsize, struct Token head)
{
	for (int x = 0; head.next != NULL; x++) {
		if (strncmp(head.id, id, idsize) == 0)
			return x;
		head = *head.next;
	}
	return -1;
}

/*
Tries to add the branch to the tree structure. Returns one if it was able
to add it to the tree and cero if it WAS NOT able to add it. If the method
was not able to add the branch into the structure it is very probable that
a branch with the same identifier already exists in the tree structure.
*/
int insertToken(struct Token *branch, struct Token *head)
{
	if (head->next == NULL) {
		head->next = branch;
		return 1;
	}
	else {
		struct Token *temp = malloc(sizeof(struct Token));
		temp->id = head->next->id;
		temp->next = head->next->next;
		temp->sons = head->next->sons;
		free(head->next);

		branch->next = temp;
		head->next = branch;
	}

	return 1;
}

/*
Adds a new word to the branch. Returns 1 if it was able to add the word to the branch.
Cero if it was not. If the method is not able to add the word it means that the 
word already exists.
*/
int insertTokenValue(char *word, int wordsize, struct Token *branch)
{
	char *allocword = calloc(wordsize + 1, sizeof(char));
	memcpy(allocword, word, wordsize);

	if (branch->sons == NULL) {
		branch->sons = create_linkchild(allocword);
	}
	else if (branch->sons->next == NULL) {
		branch->sons->next = create_linkchild(allocword);
	}
	else {
		struct TokenValue *temp = malloc(sizeof(struct TokenValue));
		temp->value = branch->sons->value;
		temp->next = branch->sons->next;
		free(branch->sons);

		struct TokenValue *head = create_linkchild(allocword);
		head->next = temp;
		branch->sons = head;
	}
	return 1;
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

/*
*/
struct Token *parsetoken(char **grammar) 
{
	int idlen = skipchar(*grammar, ',');
	struct Token *retroot = createToken(*grammar, idlen);
	idlen++; //Skip comma.
	*grammar += idlen;

	while (1) {
		for (int len = 0;; (*grammar)++, len++) {
			if (**grammar == ',') {
				insertTokenValue(*grammar - len, len, retroot);
				(*grammar)++;
				break;
			}
			else if (**grammar == '|') {
				insertTokenValue(*grammar - len, len, retroot);
				(*grammar)++;
				return retroot;
			}
		}
	}

}

/*
*/
struct Token *createTokenizer(char *grammar)
{
	int len = skipchar(grammar, '|');
	len++;
	grammar += len;
	struct Token *retroot = parsetoken(&grammar);

	while(1){
		for (;; grammar++) {
			if (*grammar == '|') {
				grammar++;
				break;
			}
			else if (*grammar == '`') {
				return retroot;
			}
		}
		insertToken(parsetoken(&grammar), retroot);
	}
}

/*
Frees all memory used by the tokenizer passed trough the parameters.
*/
void destroyTokenizer(struct Token *head)
{
	while (head != NULL) {
		while (head->sons != NULL) {
			struct TokenValue *next = head->sons->next;
            free(head->sons->value);
			free(head->sons);
			head->sons = next;
		}
		struct Token *nextHead = head->next;
		free(head->id);
        free(head);
		head = nextHead;
	}
}

void debug_print_token(struct Token *holder) 
{
	struct Token *head = holder;
	while (holder != NULL) {
		printf("TOKENS: %s\n", holder->id);
		struct TokenValue *childhead = holder->sons;
		while (holder->sons != NULL) {
			printf("%s\n", holder->sons->value);
			holder->sons = holder->sons->next;
		}
		holder->sons = childhead;
		printf("----------\n");
		holder = holder->next;
	}
	holder = head;
}
