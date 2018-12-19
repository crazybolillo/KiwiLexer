#include "Lexer.h"
#include "Tokenizer.h"
#include "Data.h"
#include "Memory.h"

extern char *PROD_SIGNAL;
extern char *PROD_END;

struct Production {
	char *name;
	char **rules;
	struct Production *next;
	uint8_t rulesize;
};

struct Match {
	char *id;
	uint8_t size;
};

struct Production *newProHead(char *prodname, int namesize,
	struct MemBlock *mem);
int addRule(char *val, struct MemBlock *mem, 
	struct Production *prod);
void addProduction(struct Production **head, 
	struct Production *node);
struct Production *newProduction(struct KiwiInput *input,
	struct LinkList *tok, struct MemBlock *parsemem, 
	struct MemBlock *lexmem);
struct Production *newParser(struct KiwiInput *input,
	struct LinkList *alphabet, struct MemBlock *parsemem,
	struct MemBlock *lexmem);
struct Match parseNext(struct Production *parser, 
	struct TokenArray *tokens);