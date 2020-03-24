
/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#ifndef _astdef_
#define _astdef_

typedef struct Node Node;

typedef struct block
{
    char *addr;
    Node *scope; // to take care of nested scope
    //metadata
} block;

typedef struct variable
{
    //metadata
    char *name;
    char *addr;
    //type
} variable;

typedef union SymbolTableNode {
    struct block;
    struct variable;
} SymbolTable;
;

struct Node
{
    union SymbolTableNode;
    int tag; // to identify the struct used of union
};

#endif