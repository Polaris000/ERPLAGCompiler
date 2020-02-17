#include "lexerDef.h"
#include "hashtableDef.h"

Token* getNextToken(FILE* fp);
void removeComments(char *testCaseFile, char* cleanFile);
int isValidID(char c);
void generateToken(Token* tk,TokenName name, char* str, int linenum);
void printTokens(char * testCaseFile);
FILE* getStream(FILE* fp);


HT* generate_hashtable(int s);
HT* make_array(HT* hash,char** arr, int s);
void printTable(HT* hash);
keyword* lookup(HT* hash, char* key);
HT* insert(HT* hash,char* key);
char** populate_keywords(char* filename);