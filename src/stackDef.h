
#include "parserDef.h"

struct stackelem_
{
	struct stackelem_ *next;
	Symbol s;
	Type t;
}

typedef struct stackelem_ StackNode;

struct meta_
{
	StackNode *head;
	int length;
}

typedef struct meta_ Stack;