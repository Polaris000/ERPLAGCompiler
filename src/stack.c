#include "stack.h"

Stack* initialize()
{
	Stack* st = malloc(sizeof(Stack));
	st -> length = 0;
	st -> head = malloc(sizeof(StackNode));

	return st; 
}

void push(Type t, Symbol s, Stack *st)
{
	StackNode * snode = malloc(sizeof(StackNode));
	snode -> s =  s;
	snode -> t = t;
	snode -> next = st -> head;

	st -> head = snode;
	st -> length += 1;
}

StackNode* pop(Stack *st)
{
	if (st -> length < 1)
		return NULL;

	StackNode *tmp = st -> head;
	st -> head =  tmp -> next;

	st -> length -= 1;
	return tmp;
}

StackNode *top(Stack *st)
{
	return st -> head;
}

int isempty(Stack *st)
{
	return st -> length ? 1: 0;
}