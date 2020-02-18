#include "parserDef.h"


void compute_first_and_follow_sets(grammar G, FirstAndFollow F);
void createParseTable(FirstAndFollow F, Table T);
ParseTree* parseInputSourceCode(char *testcaseFile, Table T);
void printParseTree(ParseTree PT, char *outfile);
