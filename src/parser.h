
#include "parserDef.h"


char* substring(const char *src, int m, int n);
Grammar* initialize();
Grammar* make_table(char* filename, Grammar* table);
void printTable(Grammar* gm);
First* computeFirst(Grammar* G, First* first);
Follow* computeFollow(Grammar* G, Follow* follow );
void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow*  F);