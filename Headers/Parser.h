#include "Lexer.h"
#include "Tokenizer.h"
#include "Memory.h"

#define PROD_SIGNAL "->"
#define PROD_END ";"

extern struct AlphList dev_parsertok;
extern struct AlphList dev_alphparser;

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
	struct AlphList *alphabet, struct MemBlock *parsemem, 
	struct MemBlock *lexmem);
struct Production *newParser(struct KiwiInput *input,
	struct AlphList *alphabet, struct MemBlock *parsemem,
	struct MemBlock *lexmem);
struct Match parseNext(struct Production *parser, 
	struct TokenArray *tokens);