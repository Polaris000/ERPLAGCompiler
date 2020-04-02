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

void parameter_check(astNode *input_list, astNode *output_list, char *val);
traverse_AST(astNode *node);

#endif;