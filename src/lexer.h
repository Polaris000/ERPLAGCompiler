/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/


#ifndef _lexer_
#define _lexer_
#include "lexerDef.h"
#include "hashtableDef.h"

Token* getNextToken(FILE* fp);
void removeComments(char *testCaseFile);
int isValidID(char c);
void generateToken(Token* tk,Terminal name, char* str, int linenum);
void printTokens(char * testCaseFile);
FILE* getStream(FILE* fp);
FILE* initializeLexer(char* testCaseFile);

HT* generate_hashtable(int s);
HT* make_array(HT* hash,char** arr, int s);
// void printTable(HT* hash);
keyword* lookup(HT* hash, char* key);
HT* insert(HT* hash,char* key);
char** populate_keywords(char* filename);

#endif