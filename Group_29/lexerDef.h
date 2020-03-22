/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#ifndef _lexerdef_
#define _lexerdef_

#include <stdio.h>
#include <stdlib.h>

//Enum for storing the token names

typedef enum
{
	INTEGER,
	REAL,
	BOOLEAN,
	OF,
	ARRAY,
	START,
	END,
	DECLARE,
	MODULE,
	DRIVER,
	PROGRAM,
	GET_VALUE,
	PRINT,
	USE,
	WITH,
	PARAMETERS,
	TRUE,
	FALSE,
	TAKES,
	INPUT,
	RETURNS,
	AND,
	OR,
	FOR,
	IN,
	SWITCH,
	CASE,
	BREAK,
	DEFAULT,
	WHILE,
	PLUS,
	MINUS,
	MUL,
	DIV,
	LT,
	LE,
	GE,
	GT,
	EQ,
	NE,
	DEF,
	ENDDEF,
	DRIVERDEF,
	DRIVERENDDEF,
	COLON,
	RANGEOP,
	SEMICOL,
	COMMA,
	ASSIGNOP,
	SQBO,
	SQBC,
	BO,
	BC,
	COMMENTMARK,
	ID,
	NUM,
	RNUM,
	ERROR,
	DOLLAR,
	EPSILON
} Terminal;

typedef union {
	int intValue;
	float fValue;
} value;

typedef struct
{
	Terminal name;   // for representing the token-name as given in the specifications
	char lexeme[50]; //character sequence of` the tokens in the program
	int lineNo;		 //line number of the token which is used for error reporting
	value *val;		 // to store the value of integers and floats
} Token;

#endif
