#include "Lexer.h"
#include "Tokenizer.h"
#include "Data.h"
#include "Memory.h"

extern char *PARSE_ERR;
extern char *PROD_SIGNAL;
extern char *PROD_END;

struct NestLinkList *newProduction(char *prodname, int namesize,
	struct MemBlock *mem);
struct LinkList *newGrammar(char *type, int typesz,
	struct LinkList *tokenizer, struct MemBlock *mem);
struct NestLinkList *newParser(struct KiwiInput *input,
	struct LinkList *tok, struct MemBlock *mem);
struct LinkList *parseNextProduction(struct KiwiInput *input,
	struct LinkList *tokenizer);