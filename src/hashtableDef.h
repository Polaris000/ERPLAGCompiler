
#ifndef _abc_
#define _abc_

#include "lexerDef.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct keyword{
   char* token;
   TokenName name;
   struct keyword* k;
}keyword;

typedef struct entry{
    int chain_size;
    keyword* ptr;
}entry;

typedef struct hashtable{
   entry** arr;
   int size;
} HT;

#endif

// char* tokenNameEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", 
// 	"USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
// 	"DEFAULT", "WHILE","PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
// 	"COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK","ID","NUM","RNUM"};