
/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#ifndef _ast_
#define _ast_
#include "parserDef.h"

astNode *postOrder_ParseTree(ParseTree *pt);
void applyASTRule_Node(TreeNode *node);
void postOrderRecur(TreeNode *pt);
astNode *createNode(nodeName name, astNode *parent, astNode *sibling, TreeNode *tokenInfo);
void populateParent(astNode *ast);
void populateChild(TreeNode *node, astNode *ast);
int isValidterminal(Terminal t);
void printAst(astNode *node);
void printAstNode(astNode *node);
void printAstRecur(astNode *node);

#endif