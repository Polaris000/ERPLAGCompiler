
#ifndef _hash_
#define _hash_

#include "lexerDef.h"
// #include "parserDef.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct keyword{
   char* token;
   Terminal name;
   struct keyword* k;
}keyword;

typedef struct hashentry{
    int chain_size;
    keyword* ptr;
}hashentry;

typedef struct hashtable{
   hashentry** arr;
   int size;
} HT;

#endif

// char* tokenNameEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", 
// 	"USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
// 	"DEFAULT", "WHILE","PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
// 	"COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK","ID","NUM","RNUM"};