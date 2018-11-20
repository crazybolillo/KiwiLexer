#include "Tokenizer.h"

char *readAll(FILE *fl, unsigned int *sizeread, int sector)
{
	char *rdata = NULL;
	int count = 1;
	do {
		fseek(fl, 0, SEEK_SET);
		rdata = realloc(rdata, sizeof(char) * (sector * count));
		*sizeread = fread(rdata, 1, (sector * count), fl);
		count++;
	} while (feof(fl) == 0);

	rdata = realloc(rdata, *sizeread);
	return rdata;
}

int skipchar(char *ptr, char until)
{
	for (int x = 0;; ptr++, x++) {
		if (*ptr == until)
			return x;
	}
}

struct LinkToken *newLinkToken(char *id, int idsize)
{
	struct LinkToken *retval = malloc(sizeof(struct LinkToken));
	retval->next = NULL;
	retval->sons = NULL;
	retval->id = calloc(sizeof(char), idsize + 1);
	memcpy(retval->id, id, idsize);
	return retval;
}

void insertToken(struct LinkToken *branch, struct LinkToken **head)
{
	if (*head == NULL) {
		*head = branch;
	}
	else if ((*head)->next == NULL) {
		(*head)->next = branch;
	}
	else {
		struct LinkToken *temp = malloc(sizeof(struct LinkToken));
		temp->id = (*head)->next->id;
		temp->next = (*head)->next->next;
		temp->sons = (*head)->next->sons;
		free((*head)->next);

		branch->next = temp;
		(*head)->next = branch;
	}
}


void insertLexeme(char *word, int wordsize, struct LinkToken *branch)
{
	char *allocword = calloc(sizeof(char), wordsize + 1);;
	memcpy(allocword, word, wordsize);

	struct LinkLex *nlexeme = malloc(sizeof(struct LinkLex));
	nlexeme->value = allocword;
	nlexeme->next = NULL;

	if (branch->sons == NULL) {
		branch->sons = nlexeme;
	}
	else if (branch->sons->next == NULL) {
		branch->sons->next = nlexeme;
	}
	else {
		struct LinkLex *temp = malloc(sizeof(struct LinkLex));
		temp->value = branch->sons->value;
		temp->next = branch->sons->next;
		free(branch->sons);

		struct LinkLex *head = nlexeme;
		head->next = temp;
		branch->sons = head;
	}
}

struct LinkToken *parseToken(char **grammar) 
{
	int len = 0;
	struct LinkToken *retroot;
	for(; (*(*grammar + len) != TOKEN_SEPARATOR) && (*(*grammar + len) != TOKEN_LIMIT); len++);
	if(*(*grammar + len) == TOKEN_LIMIT){
	    retroot = newLinkToken(*grammar, len);
	    len++;
        *grammar += len;
        return retroot;
	}
	
	retroot = newLinkToken(*grammar, len);
	len++; //Skip comma.
	*grammar += len;
    
    len = 0;
	for (; **grammar != TOKEN_LIMIT; (*grammar)++, len++) {
		if (**grammar == TOKEN_SEPARATOR) {
			insertLexeme(*grammar - len, len, retroot);
			(*grammar)++;
			len = 0;
		    continue;
		}
	}	
	if (len > 0) {
		insertLexeme(*grammar - len, len, retroot);
	}
	(*grammar)++;
	return retroot;

}

struct LinkToken *createTokenizer(char *grammar)
{
	struct LinkToken *retroot = NULL;
	for(; *grammar != TOKEN_END; grammar++){
		if (*grammar == TOKEN_LIMIT) {
			grammar++;
			insertToken(parseToken(&grammar), &retroot);
		}
	}
	return retroot;
}

void destroyTokenizer(struct LinkToken *head)
{
	struct LinkToken *nextHead;
	struct LinkLex *nextSon;
	while (head != NULL) {
		while (head->sons != NULL) {
			nextSon = head->sons->next;
            free(head->sons->value);
			free(head->sons);
			head->sons = nextSon;
		}
		nextHead = head->next;
		free(head->id);
        free(head);
		head = nextHead;
	}
}

void printTokenizer(struct LinkToken *holder) 
{
	struct LinkToken *head = holder;
	struct LinkLex *childhead;
	char nlineflag = 0;
	while (holder != NULL) {
		printf("TOKEN: %s\n", holder->id);
		childhead = holder->sons;
		while (holder->sons != NULL) {
			if (nlineflag >= 6) {
				nlineflag = 0;
				printf("\n");
			}
			printf("<\"%s\"> ", holder->sons->value);
			holder->sons = holder->sons->next;
			nlineflag++;
		}
		holder->sons = childhead;
		printf("\n----------\n");
		holder = holder->next;
	}
	holder = head;
}

/*----------------------------------------
_mem (More memory efficient) functions defined.
-----------------------------------------*/
struct mem_LinkToken *mem_newLinkToken(char *id, int idsize)
{
	char *nid = calloc(sizeof(char), idsize + 1);
	memcpy(nid, id, idsize);
	struct mem_LinkToken *retval = malloc(sizeof(struct mem_LinkToken));
	retval->id = nid;
	retval->next = NULL;
	return retval;
}

void mem_insertToken(struct mem_LinkToken *branch, 
	struct mem_LinkToken **head) 
{
	if ((*head) == NULL) {
		(*head) = branch;
	}
	else if ((*head)->next == NULL) {
		(*head)->next = branch;
	}
	else {
		struct mem_LinkToken *tmp = malloc(sizeof(struct mem_LinkToken));
		tmp->id = (*head)->next->id;
		tmp->next = (*head)->next->next;
		free((*head)->next);

		branch->next = tmp;
		(*head)->next = branch;
	}
}

struct mem_LinkToken *mem_parseToken(char **grammar) 
{
	int len = skipchar(*grammar, TOKEN_LIMIT);
	struct mem_LinkToken *retval = mem_newLinkToken(*grammar, len);
	len++;
	*grammar += len;
	return retval;
}

struct mem_LinkToken *mem_createTokenizer(char *grammar)
{
	struct mem_LinkToken *retval = NULL;
	for (; *grammar != TOKEN_END; grammar++) {
		if (*grammar == TOKEN_LIMIT) {
			grammar++;
			mem_insertToken(mem_parseToken(&grammar), &retval);
		}
	}
	return retval;
}

void mem_printTokenizer(struct mem_LinkToken *head)
{
	struct mem_LinkToken *tmphead = head;
	char prntcount = 0;
	while (head != NULL) {
		if (prntcount >= 6) {
			prntcount = 0;
			printf("\n");
		}
		printf("<TOKEN: \"%s\"> ", head->id);
		head = head->next;
		prntcount++;
	}
	head = tmphead;
}

void mem_destroyTokenizer(struct mem_LinkToken *head)
{
	struct mem_LinkToken *nxthead;
	while (head != NULL) {
		nxthead = head->next;
		free(head->id);
		free(head);
		head = nxthead;
	}
}



