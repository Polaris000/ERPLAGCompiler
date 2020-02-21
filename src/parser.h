
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
//Following fuctions are for trie

node* initialize_node();
trie* initialize_trie();
int lookup(trie* t,char* w);
void insert_trie(trie* t,char* w,int key);
trie* populate_trie_nonterminals(trie* t,char** nonTerminalEnumToString);
trie* populate_trie_terminals(trie* t,char** TerminalEnumToString);