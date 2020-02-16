#include "hashtableDef.h"

HT* generate_hashtable(int s);
HT* make_array(HT* hash,char** arr, int s);
void printTable(HT* hash);
keyword* lookup(HT* hash, char* key);
HT* insert(HT* hash,char* key);
char** populate_keywords(char* filename);