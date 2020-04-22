/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#ifndef _symbolTable_
#define _symbolTable_

#include "symbolTableDef.h"
#include "ast.h"
#include "semanticAnalyzerDef.h"

Table *initialize_table(Node *parent, Table *container);
int getHash(char *val, int size);
void insert_in_Table(Node *node, Table *t, int hash);
Table *insertST(char *val, Table *t, int tag);
Node *lookUpST(char *val, Table *t);
Table *processNode(Table *tb, astNode *node, error_list *list);
Table *populateSymbolTable(astNode *root, error_list *list);
void populateTableRecur(Table *tb, astNode *root, error_list *list);
void processModule(Table *tb, astNode *node, error_list *list);
int checkRecursion(astNode *node, Table *tb);
void printSymbolTable(Table *tb);
void printSymbolTableUtil(Table *tb);
List *createListNode(char *val, char *addr, int var_tag, Leaf *lower_bound, Leaf *upper_bound);
trie *populateTrie(trie *trie, astNode *node);
void printTableEntry(Node *n, Table *tb, int nesting_level);

void calcMemoryofFunctions(Table *tb);
void calMemoryFunction(Table *tb, int *mem);

void printArrays(Table *tb);
void printArrayInfo(Node *n, Table *tb);

#endif