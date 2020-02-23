
#ifndef _parser_
#define _parser_
#include "parserDef.h"


char* substring(const char *src, int m, int n);
Grammar* initialize();
Grammar* make_table(char* filename, Grammar* table);
void printTable(Grammar* gm);

//First and Follow set calculations
FirstAndFollow* initializeFirstFollow();
void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow*  F);
int* computeFirst(Grammar* G, FirstAndFollow* F, NonTerminal nt);
int* computeFollow(Grammar* G, FirstAndFollow* F, NonTerminal nt);
void setBit(int* firstSet, Terminal t);
int TestBit(int* firstSet, Terminal t);
int* setUnion(int* firstSet1, int* firstSet2);
void printFirstSet(FirstAndFollow* F);
void printFollowSet(FirstAndFollow* F);


ParseTable* initializeParseTable();
void populateParseTable(FirstAndFollow*F, Grammar *G, ParseTable * pt);
ParseTree* parseInputSourceCode(char * testCaseFile, ParseTable* table, Grammar* gm,FirstAndFollow* F);



//Following fuctions are for trie

node* initialize_node();
trie* initialize_trie();
int lookUp(trie* t,char* w);
void insert_trie(trie* t,char* w,int key);
trie* populate_trie_nonterminals(trie* t,char** nonTerminalEnumToString);
trie* populate_trie_terminals(trie* t,char** TerminalEnumToString);


//Nary Tree

Queue* initialize_queue();
void enQueue(Queue* q, TreeNode* t);
TreeNode* deQueue(Queue* q);
TreeNode* first(Queue* q);
TreeNode* last(Queue* q);
ParseTree* initialize_tree();
void printNode(TreeNode *node,FILE* fp);
void printParseTree_inorder(TreeNode* pt , char* filename);
void printParseTree_inorder_util(TreeNode* pt,FILE* fpt);
void printParseTree_levelOrder(TreeNode* pt, char* filename);



TreeNode* initialize_internalnode(int tag, char* lexeme, NonTerminal parent ,NonTerminal nt);
TreeNode* initialize_leafnode(int tag, Terminal ter, NonTerminal parent);


//Stack

Stack* initialize_stack();

void push(Stack *st, TreeNode *node);

TreeNode* pop(Stack *st);

TreeNode *top(Stack *st);

int isempty(Stack *st);

#endif
