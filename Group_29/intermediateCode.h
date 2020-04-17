/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "ast.h"
#include "symbolTable.h"
#include "intermediateCodeDef.h"

void startCode(FILE *fp);
void generate_assembly_code(astNode *root, char *asm_file);
void generate_asm_ast(astNode *root, FILE *fp);
void traverseDriverModule(astNode *node, FILE *fp);
void processIO_getValue_Stmt(astNode *node, FILE *fp);
void processIO_print_Stmt(astNode *node, FILE *fp);
tempReg *initialize_tempreg(char *reg);
void evaluateAssignmentStmt(astNode *node, FILE *fp);
void processAssignmentStmt_asm(astNode *node, FILE *fp);
void processOtherStmts(astNode *node, FILE *fp);
