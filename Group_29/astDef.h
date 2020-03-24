
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
    program_ast,
    moduleDec_ast,
    driverModule_ast,
    module_ast,
    input_plist_ast,
    output_plist_ast,
    dataType_ast,
    range_array_ast,
    ioStmt_get_value_ist,
    ioStmt_print_ast,
    var_id_num_ast,
    assignmentStmt_ast,
    lvalueIDStmt_ast,
    lvalueARRStmt_ast,
    moduleReuseStmt_ast,
    optional_ast,
    idList_ast,
    expression_ast,
    arithmeticExpr_dash_ast,
    bool_ast,
    arithmeticExpr_recur_ast,
    term_dash_ast,
    declareStmt_ast,
    conditionalStmt_ast,
    caseStmts_ast,
    itr_for_ast,
    itr_while_ast,
    range_ast,
} nodeName;

typedef struct children Children;
typedef struct astNode astNode;
struct children
{
    astNode *head;
    astNode *tail;
    int children_size;
};

struct astNode
{
    nodeName n_Name;
    astNode *parent;
    astNode *sibling;
    Children *child_list;
    // Token *tokenInfo;
    Leaf *tokenInfo;
};

#endif