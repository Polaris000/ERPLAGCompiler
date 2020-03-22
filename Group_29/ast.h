
/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#ifndef _ast_
#define _ast_
#include "astDef.h"

astNode *postOrder_ParseTree(ParseTree *pt);
void processNode(TreeNode *node);
void postOrderRecur(ParseTree *pt);
astNode *createNode(nodeName name, astNode *parent, astNode *sibling, Token *tokenInfo);
astNode *populateParent();
astNode *addSibling();
Children *populateChild(TreeNode *node);
