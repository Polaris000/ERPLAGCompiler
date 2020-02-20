#include "stack.h"

Stack* initialize();

void push(Type t, Symbol s, Stack *st);

StackNode* pop(Stack *st);

StackNode *top(Stack *st);

int isempty(Stack *st);