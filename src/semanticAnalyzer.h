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
#include "semanticAnalyzerDef.h"

void parameter_check(astNode *input_list, astNode *output_list, astNode *moduleName, Table *tb, error_list *list);
void traverse_AST(astNode *node, error_list *list);
void traverseModule(astNode *node, List *output_list, error_list *list);
List *retrieveOutList(List *list, char *val);
Node *lookupID(Table *tb, char *val);
char *processOperator(nodeName name, char *type1, char *type2);
char *processAssignmentStmt(astNode *node, Table *tb, error_list *list);
char *checkArray(Node *node, Leaf *index, int lineNo, error_list *list);
void traverseWhile(astNode *node, Table *tb);
void check_array_assignment(astNode *node, error_list *list);

//For handling errors
error_list *init_errors();
void appendError_at_head(error_list *list, char *err, int line_no);
void printSemanticErrors(error_list *list);
void sortErrorList(error_list *list);
void sortErrorsListRecur(error *temp, int total);

#endif