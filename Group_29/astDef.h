
/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#ifndef _astdef_
#define _astdef_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "parserDef.h"

typedef enum
{
    program,
    module,
    Driver_Module, // is two words in our doc
    input_plist,
    output_plist,
    integer,
    boolean,
    real,
    dataType,
    range_array,
    ioStmt_get_value,
    ioStmt_print,
    var_id_num,
    num,
    rnum,
    true,
    false,
    assignmentStmt,
    lvalueIDStmt,
    lvalueARRStmt,
    moduleReuseStmt,
    optional,
    idList,
    expression,
    arithmeticExpr_dash,
    bool,
    arithmeticExpr_recur,
    term_dash,
    plus,
    minus,
    mul,
    div,
    and,
    or
    ,
    gt,
    lt,
    ge,
    le,
    eq,
    ne,
    declareStmt,
    conditionalStmt,
    caseStmts,
    itr_for,
    itr_while,
    range
} nodeName;

typedef struct children Children;
struct children
{
    Children *head;
    Children *tail;
    Children *next;
    int children_size;
};

typedef struct astNode astNode;
struct astNode
{
    nodeName n_Name;
    astNode *parent;
    astNode *sibling;
    Children *child_list;
    Token *tokenInfo;
};

#endif