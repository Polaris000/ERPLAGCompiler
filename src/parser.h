#include "parserDef.h"


void compute_first_and_follow_sets(grammar G, firstandfollow F);
void createParseTable(FirstAndFollow F, Table T);
void parseInputSourceCode(char *testcaseFile, Table T);
void printParseTree(parseTree PT, char *outfile);
