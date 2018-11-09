#include "TreeData.h"


/*
Creates a LinkedChild 
*/
struct LinkedChild *create_linkchild(char *value)
{
	struct LinkedChild *retval = malloc(sizeof(struct LinkedChild));
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
struct LinkedParent *create_linkedparent(char *id, int idsize)
{
	struct LinkedParent *retval = malloc(sizeof(struct LinkedParent));
	retval->next = NULL;
	retval->id = malloc((sizeof(char) * idsize) + 1);
	retval->sons = NULL;
	memset(retval->id, 0x00, idsize + 1);
	_memccpy(retval->id, id, 1, idsize);
	return retval;
}

/*
Returns the index where the LinkedParent with the identifier passed trough the parameters
is found inside the LinkedParent list. Returns -1 if the branch was not found.
*/
int branch_index(char *id, int idsize, struct LinkedParent head)
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
int add_branch(struct LinkedParent *branch, struct LinkedParent *head)
{
	if (head == NULL) {
		head = branch;
	}
	else if (head->next == NULL) {
		head->next = branch;
		return 1;
	}
	else {
		struct LinkedParent *temp = head;
		branch->next = temp;
		head = branch;
	}

	return 1;
}

/*
Adds a new word to the branch. Returns 1 if it was able to add the word to the branch.
Cero if it was not. If the method is not able to add the word it means that the 
word already exists.
*/
int add_word(char *word, int wordsize, struct LinkedParent *branch)
{
	char *allocword = malloc(sizeof(char) * (wordsize + 1));
	memset(allocword, 0x00, wordsize + 1);
	_memccpy(allocword, word, 1, wordsize);

	if (branch->sons == NULL) {
		branch->sons = create_linkchild(allocword);
	}
	else if (branch->sons->next == NULL) {
		branch->sons->next = create_linkchild(allocword);
	}
	else {
		struct LinkedChild *temp = malloc(sizeof(struct LinkedChild));
		temp->value = branch->sons->value;
		temp->next = branch->sons->next;
		free(branch->sons);

		struct LinkedChild *head = create_linkchild(allocword);
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
char *readfile(FILE *fl, int *size)
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
struct LinkedParent *parsetoken(char **grammar) 
{
	int idlen = skipchar(*grammar, ',');
	struct LinkedParent *retroot = create_linkedparent(*grammar, idlen);
	idlen++; //Skip comma.
	*grammar += idlen;

	while (1) {
		for (int len = 0;; (*grammar)++, len++) {
			if (**grammar == ',') {
				add_word(*grammar - len, len, retroot);
				(*grammar)++;
				break;
			}
			else if (**grammar == '|') {
				add_word(*grammar - len, len, retroot);
				(*grammar)++;
				return retroot;
			}
		}
	}

}

/*
*/
struct LinkedParent *parse_grammar(char *grammar)
{
	struct LinkedParent *retroot = malloc(sizeof(struct LinkedParent));
	retroot ->id = "Parent";
	retroot->next = NULL;
	retroot->sons = NULL;
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
		add_branch(parsetoken(&grammar), retroot);
	}
}

void debug_print_token(struct LinkedParent holder) 
{
	while (holder.next != NULL) {
		printf("TOKENS: %s\n", holder.next->id);
		while (holder.next->sons != NULL) {
			printf("%s\n", holder.next->sons->value);
			holder.next->sons = holder.next->sons->next;
		}
		printf("----------");
		holder.next = holder.next->next;
	}
}