/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#ifndef _semanticAnalyzer_
#define _semanticAnalyzer_

#include "ast.h"
#include "symbolTable.h"

void parameter_check(astNode *input_list, astNode *output_list, astNode *moduleName, Table *tb);
void traverse_AST(astNode *node);
void traverseModule(astNode *node, List *output_list);
List *retrieveOutList(List *list, char *val);
Node *lookupID(Table *tb, char *val);
char *processOperator(nodeName name, char *type1, char *type2);
char *processAssignmentStmt(astNode *node);
char *checkArray(Node *node, Leaf *index, int lineNo);

#endif