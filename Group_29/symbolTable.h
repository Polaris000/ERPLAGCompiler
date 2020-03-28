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

Table *initialize_table(Node *parent, Table *container);
int getHash(char *val, int size);
void insert_in_Table(Node *node, Table *t, int hash);
Table *insertST(char *val, Table *t, int tag);
Node *lookUpST(char *val, Table *t);
Table *processNode(Table *tb, astNode *node);
Table *populateSymbolTable(astNode *root);
void populateTableRecur(Table *tb, astNode *root);

#endif