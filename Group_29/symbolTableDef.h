
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

// #include "parserDef.h"

extern char *nodeNameEnumToString[];
typedef struct Leaf Leaf;
typedef struct Table Table;
typedef struct trie trie;

typedef struct List
{
    char *val;
    int assigned; // to take care of whether the output parameter has been assigned inside the module
    char *addr;
    int var_tag;
    Leaf *lower_index;
    Leaf *upper_index;
    struct List *next;
} List;

typedef struct block
{
    char *addr;
    Table *scope; // to take care of nested scope
    int decl_required;
    int defined;
    int num_for;
    int num_while;
    int num_switch;
    List *input_list;
    List *output_list;
    trie *trie; //used for 'while' loop condition variables
    int module_overloaded;
    int flag_module;
} Block;

typedef struct variable
{
    int var_tag; // Denotes whether a variable id is normal variable(0) or an array variable(1)
    char *addr;  //Denotes the Datatype of the variable
    Leaf *lower_index;
    Leaf *upper_index;
    int offset;
} Variable;

typedef union SymbolTableNode {
    Block block;       //Initialize the nested scope
    Variable variable; //Variable Identifier
} SymbolTable;

typedef struct Node
{
    SymbolTable *SymbolTableNode;
    int tag;   // to identify the struct used of union : 1 - Block, 0 - Variable
    char *val; //lexeme of the identifier or moduleName
    struct Node *next;
} Node;

struct Table
{
    Node **nodes;
    int num;
    Node *parent;     //Denotes the source row of parent table
    Table *container; // Denotes the parent symbol table
    int size;         //How many entries are present in the table
};

// typedef struct BstWhile bstWhile;
// struct BstWhile
// {
//     astNode *key;
//     bstWhile *left;
//     bstWhile *right;
// };

#endif