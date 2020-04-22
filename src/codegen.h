/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "ast.h"
#include "symbolTable.h"
#include "codegenDef.h"

void startCode(FILE *fp);
void generate_assembly_code(astNode *root, char *asm_file);
void generate_asm_ast(astNode *root, FILE *fp);
void traverseDriverModule(astNode *node, FILE *fp);
void processIO_getValue_Stmt(astNode *node, FILE *fp);
void processIO_print_Stmt(astNode *node, FILE *fp);
// tempReg *initialize_tempreg(char *reg);
void processAssignmentStmt_asm(astNode *node, FILE *fp);
void evaluateAssignmentStmt(astNode *node, FILE *fp, astNode *lhs);

void processWhileLoop(astNode *node, FILE *fp);
void processForLoop(astNode *node, FILE *fp);

void populateUninitializedData(FILE *fp);
void populateSTVariables(Table *tb, FILE *fp);
void printVariable(Node *node, Table *tb, int nesting_level, FILE *fp);
void printMsg(char *str, FILE *fp);
void getValue(char *str, FILE *fp);
void printDOut(char *str, FILE *fp);
void processBooleanExpression(astNode *node, FILE *fp);
void processOtherStmts(astNode *node, FILE *fp, int nesting_level);
void processSwitchStmt(astNode *node, FILE *fp);
void processIntSwitch(astNode *node, FILE *fp, int label);
void processCaseStmt(astNode *node, FILE *fp, char *str);
void processModuleReUseStmt(astNode *node, astNode *inputList, astNode *outputList, astNode *moduleName, FILE *fp);

void defineAllModules(astNode *root, FILE *fp);
void defineModule(astNode *node, FILE *fp);
templookup *lookupIDCodegen(Table *tb, char *val);
void dynamicArrayHandle(templookup *node_t, Table *tb, FILE *fp);
int DynamicArrayIndexHandle(templookup *node_t, Table *tb, FILE *fp, astNode *index);
void dynamicarrstruct(FILE *fp);