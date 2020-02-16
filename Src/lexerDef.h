#include <stdio.h>
#include <stdlib.h>


#ifndef _LEXERDEF_
#define _LEXERDEF_

//Enum for storing the token names

typedef enum {
	INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, GET_VALUE, PRINT, 
	USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK,
	DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, DRIVERDEF, DRIVERENDDEF,
	COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, COMMENTMARK,ID,NUM,RNUM
}TokenName;

typedef union{
	int intValue;
	float fValue;
}value;

typedef struct{
	TokenName name;  // for representing the token-name as given in the specifications
	char* lexeme; //character sequence of the tokens in the program
	int lineNo;   //line number of the token which is used for error reporting
	value* val;   // to store the value of integers and floats
}Token;




#endif

