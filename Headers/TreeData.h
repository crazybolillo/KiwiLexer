#ifndef TREE_HEADER_GUARD
#define TREE_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

struct LinkedChild {
	char *value;
	struct LinkedChild *next;
};

struct LinkedParent {
	char *id;
	struct LinkedChild *sons;
	struct LinkedParent *next;
};

struct LinkedParent *create_linkedparent(char *id, int idsize);

int branch_index(char *id, int idsize, struct LinkedParent root);
int add_branch(struct LinkedParent branch, struct LinkedParent *root);

void free_doubleptr(void **doubleptr, int size);
int remove_branch(char *id, int idsize, struct LinkedParent *root);

int word_exists(struct treeBranch branch);
int add_word(char *word, int wordsize, struct LinkedParent *branch);
int remove_word(struct treeBranch branch);

char *readfile(FILE *fl, int *size);
int skipchar(char *ptr, char until);
struct LinkedParent *parse_grammar(char *grammar);

void debug_print_token(struct LinkedParent tokenizer);

#endif
