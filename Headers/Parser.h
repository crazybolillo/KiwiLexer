#include "Lexer.h"
#include "Tokenizer.h"

extern char *PARSE_ERR;

struct Production {
	struct Production *next;
	struct Grammar *gram;
	char *prodname;
};

struct Grammar {
	char *type;
	struct Grammar *next;
};

void addGrammar(struct Production *prod, struct Grammar *grammar);
void addProduction(struct Production *prod);

struct Production *newProduction(char *prodname, int namesize);
struct Grammar newGrammar(char *type, int typesz, 
	struct LinkList *tokenizer);
struct Production *parseNextProduction(struct KiwiInput *input, 
	struct LinkList *tokenizer);
char *parseMatch(struct KiwiInput input, struct Production prod);
void destroyParser(struct Production *prod);