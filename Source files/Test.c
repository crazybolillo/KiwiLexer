#include "TreeData.h"
#include <stdio.h>

void main(void) {
	char *query = "|COMIDA,mango,manzana,platano,uva,zanahoria,cebolla|`";
	struct LinkedParent *root = parse_grammar(query);
	debug_print_token(*root);
}
