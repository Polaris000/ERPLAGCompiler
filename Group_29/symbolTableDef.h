
/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#ifndef _symbolTabledef_
#define _symbolTabledef_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Table Table;

typedef struct block
{
    char *addr;
    Table *scope; // to take care of nested scope
    int defined;
    decl_required;
} Block;

typedef struct variable
{
    int var_tag; // Denotes whether a variable id normal variable(0) or an array variable(1)
    char *addr;
    int lower_index;
    int upper_index;
} Variable;

typedef union SymbolTableNode {
    Block block;
    Variable variable;
} SymbolTable;

typedef struct Node
{
    SymbolTable *SymbolTableNode;
    int tag; // to identify the struct used of union
    char *val;
    struct Node *next;
} Node;

struct Table
{
    Node **nodes;
    int num;
    Node *parent;     //Denotes the source row of parent table
    Table *container; // Denotes the parent symbol table
    int size;
};

#endif