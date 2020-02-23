#ifndef _parserdef_
#define _parserdef_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


#include "lexer.h"

#define NON_TERMINALS 55
#define TERMINALS 60

extern char* nonTerminalEnumToString[];
extern char* TerminalEnumToString[];

typedef enum 
{
    program,moduleDeclarations,moduleDeclaration,otherModules, driverModule, module,
    ret, input_plist, input_plist_dash, output_plist, output_plist_dash, dataType, range_array,
    type, moduleDef, statements, statement, ioStmt, var, var_id_num, boolConstt, whichId,
    simpleStmt, assignmentStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, Index, moduleReuseStmt,
    optional, idList, idList_dash, expression, unaryOrExpr, arithmeticOrBooleanExpr, anyTerm,
    arithmeticExpr_dash, bool, arithmeticExpr, arithmeticExpr_recur, term, term_dash, factor, 
    op_plus_minus, op_mul_div, logicalOp, relationalOp, declareStmt, conditionalStmt, caseStmts,
    caseStmt, Value,Default,iterativeStmt, range, ROOT
} NonTerminal;



// Following structures are used for representing the grammar
typedef union symbol
{
    Terminal t;
    NonTerminal nt;
}Symbol;

typedef enum 
{
    
    terminal,
    nonterminal
}Type;

struct RHSNode
{
    Symbol s;
    Type type;
    struct RHSNode * next;
};

typedef struct RHSNode RHSNode;

typedef struct
{
    NonTerminal nt;
    int num_rules;
    RHSNode **rules;
}LHSNode;

typedef struct
{
    int num_non_terminals;
    LHSNode * non_terminals;
}Grammar;




// first and follow -----------------------------------

typedef struct
{
	int *firstSet;
}First;
// typedef struct First[NON_TERMINALS] First;

typedef struct
{
	int *followSet;
}Follow;
// typedef struct Follow_[NON_TERMINALS] Follow;

typedef struct
{
	First* first;
	Follow* follow;
}FirstAndFollow;


//Parse Table structs

typedef struct parse
{
	int rule_no;
}parse;

typedef struct ParseTable
{
	parse Parse[NON_TERMINALS][TERMINALS];
}ParseTable;
// g -> non_terminals [i] -> rules [ruleno] RHSNode*


//Trie--------------------------------------------------------------
typedef union
{
    char c;
    int pos;
}entry;

typedef struct node{
    entry data;
    struct node* next[28];
}node;

typedef struct trie{
    int num;
    node* children;
}trie;



//Parse Tree Construction

typedef struct TreeNode TreeNode;


typedef struct internal
{
    // display info
    char lexeme[100]; // ----
    int lineno;
    NonTerminal parent;      // would be ROOT for root node
    NonTerminal nodesym;    // non_term symbol

    // structure
    int rhs_size;            // num_of_elems in rhs of rule
    TreeNode** children;    // array of pointers to children
}Internal;

typedef struct leaf
{
    // display info
    char lexeme[100];
    int lineno;
    Terminal t;     // token
    value* val;    // if num/rnum
    NonTerminal parent; // non_term sym of parent
}Leaf;


typedef union NodeData
{
    Internal internalnode;
    Leaf leafnode;
}NodeData;

typedef struct TreeNode{
    NodeData t;
    int tag;
}TreeNode;







typedef struct ParseTree{
    TreeNode* root;
    int levels;
}ParseTree;
// typedef union node TreeNode;

// points to root of tree
typedef struct Queue{
    int top;
    int end;
    TreeNode** t;
    int size;
    int capacity;
}Queue;



typedef struct Stack
{
    int top;
    TreeNode ** t;
    int size;
    int capacity;
}Stack;

#endif