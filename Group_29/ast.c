
/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#include "ast.h"

astNode *postOrder_ParseTree(ParseTree *pt)
{
    if (pt->root == NULL)
        return NULL;

    postOrderRecur(pt->root);

    astNode *ast = pt->root->addr;
    return ast;
}

void postOrderRecur(TreeNode *node)
{
    if (node == NULL)
        return;

    NodeData data = node->t;

    if (node->tag == 0) // if internal node
    {
        TreeNode **children = data.internalnode.children;
        // printf("Hello -- %s\n", nonTerminalEnumToString[data.internalnode.nodesym]);
        if (children != NULL)
        {
            int size = data.internalnode.rhs_size;
            int i = 0;
            while (size--)
            {
                postOrderRecur(children[i]);
                i++;
            }
        }
        // if (node->t.internalnode.nodesym == program)
        //     printf("Here - %s\n", nonTerminalEnumToString[data.internalnode.nodesym]);
        printf("Hello -- %s -- ", nonTerminalEnumToString[node->t.internalnode.nodesym]);
        processNode(node);
    }
}

astNode *createNode(nodeName name, astNode *parent, astNode *sibling, TreeNode *tokenInfo)
{
    astNode *temp = malloc(sizeof(astNode));
    temp->n_Name = name;
    temp->parent = parent;
    temp->sibling = sibling;
    temp->child_list = NULL;
    // temp->tokenInfo = tokenInfo;
    temp->tokenInfo = &(tokenInfo->t.leafnode);

    return temp;
}
int isValidterminal(Terminal t)
{
    if (t == ID || t == NUM || t == RNUM || t == ASSIGNOP)
        return 1;
    return 0;
}

void populateChild(TreeNode *node, astNode *newNode)
{
    int i = 0, j = 0, size = node->t.internalnode.rhs_size;
    astNode *temp;
    Children *chi = malloc(sizeof(Children));
    chi->children_size = 0;
    // if (node->t.internalnode.nodesym == module)
    //    printf("%d\n", size);
    while (size--)
    {
        // if (node->t.internalnode.nodesym == module)
        //     printf("%d -- %d\n", i, size);
        temp = node->t.internalnode.children[i]->addr;
        if (node->t.internalnode.children[i]->tag == 0)
        {
            // if (node->t.internalnode.nodesym == module)
            //     printf("Alright\n");
            if (j == 0)
            {
                chi->head = temp;
                chi->tail = temp;
                chi->children_size++;
            }
            else
            {
                chi->tail->sibling = temp;
                chi->children_size++;
                chi->tail = temp;
            }
            j++;
        }
        else if (isValidterminal(node->t.internalnode.children[i]->t.leafnode.t))
        {
            // temp = createNode(-1, NULL, NULL, &(node->t.internalnode.children[i]->t.leafnode));
            temp = createNode(-1, NULL, NULL, node->t.internalnode.children[i]);

            if (j == 0)
            {
                chi->head = temp;
                chi->tail = temp;
                chi->children_size++;
            }
            else
            {
                chi->tail->sibling = temp;
                chi->children_size++;
                chi->tail = temp;
            }
            j++;
        }
        i++;
    }
    newNode->child_list = chi;
    node->addr = newNode;
}

void populateParent(astNode *ast)
{
    astNode *temp = ast->child_list->head;
    while (temp != NULL)
    {
        temp->parent = ast;
        temp = temp->sibling;
    }
}

void processNode(TreeNode *node)
{
    // int rule_no = node->rule_no1;
    int rule_no = node->t.internalnode.nodesym;
    printf("%d\n", node->rule_no);
    switch (rule_no)
    {
    case 0: //<program> -> <moduleDeclarations> <otherModules> <driverModule> <otherModules>
    {
        printf("Hello from program\n");
        astNode *ast = createNode(program_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        break;
    }
    case 1: //<moduleDeclarations> -> <moduleDeclaration> <moduleDeclarations> | e
    case 3: //<otherModules> ->  <module> <otherModules> | e
    {
        if (node->rule_no == 0)
        {
            node->t.internalnode.children[0]->addr->sibling = node->t.internalnode.children[1]->addr;
            node->addr = node->t.internalnode.children[0]->addr;
            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
        }
        else
        {
            astNode *ast = createNode(-1, NULL, NULL, NULL);
            node->addr = ast;
        }
        break;
    }
    case 2: //<moduleDeclaration> -> DECLARE MODULE ID SEMICOL
    {
        astNode *ast = createNode(moduleDec_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[3]);
        break;
    }
    case 4: //<driverModule> -> DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
    {
        astNode *ast = createNode(driverModule_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        free(node->t.internalnode.children[4]);
        break;
    }
    case 5: //<module> ->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
    {
        astNode *ast = createNode(module_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[3]);
        free(node->t.internalnode.children[4]);
        free(node->t.internalnode.children[5]);
        free(node->t.internalnode.children[6]);
        free(node->t.internalnode.children[7]);
        free(node->t.internalnode.children[8]);
        free(node->t.internalnode.children[9]);
        free(node->t.internalnode.children[10]);
        free(node->t.internalnode.children[11]);
        break;
    }
    case 6: //<ret> ->  RETURNS SQBO <output_plist> SQBC SEMICOL | e
    {
        if (node->rule_no == 0)
        {
            node->addr = node->t.internalnode.children[2]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
        }
        else
        {
            // node->addr = NULL;
            astNode *ast = createNode(-1, NULL, NULL, NULL);
            node->addr = ast;
        }

        break;
    }
    case 7: //<input_plist> -> ID COLON <dataType> <input_plist_dash>
    {
        astNode *ast = createNode(input_plist_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        break;
    }
    case 8:  //<input_plist_dash> -> COMMA ID COLON <dataType> <input_plist_dash> | e
    case 10: //<output_plist_dash> -> COMMA ID COLON <type> <output_plist_dash> | e
    {
        if (node->rule_no == 0)
        {
            //Following two lines first make an astNode for the ID terminal
            astNode *temp = createNode(-1, NULL, NULL, node->t.internalnode.children[1]);
            node->t.internalnode.children[1]->addr = temp;

            node->t.internalnode.children[1]->addr->sibling = node->t.internalnode.children[3]->addr;
            node->t.internalnode.children[3]->addr->sibling = node->t.internalnode.children[4]->addr;
            node->addr = node->t.internalnode.children[1]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 9: //<output_plist> -> ID COLON <type> <output_plist_dash>
    {
        astNode *ast = createNode(output_plist_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        break;
    }
    case 11: //<dataType> -> INTEGER | BOOLEAN | REAL | ARRAY SQBO <range_array> SQBC OF <type>
    {
        switch (node->rule_no)
        {
        case 0:
        case 1:
        case 2:
        {
            node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
            break;
        }
        case 3:
        {
            astNode *ast = createNode(dataType_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
            free(node->t.internalnode.children[5]);
            break;
        }
        }
        break;
    }
    case 12: //<range_array> -> <index> RANGEOP <index>
    {
        astNode *ast = createNode(range_array_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        break;
    }
    case 13: //<type> ->  INTEGER | REAL | BOOLEAN
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 14: //<moduleDef> ->  START <statements> END
    {
        node->addr = node->t.internalnode.children[1]->addr;

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        break;
    }
    case 15: //<statements> -> <statement> <statements> | e
    {
        if (node->rule_no == 0)
        {
            node->t.internalnode.children[0]->addr->sibling = node->t.internalnode.children[1]->addr;
            node->addr = node->t.internalnode.children[0]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 16: //<statement> -> <ioStmt> | <simpleStmt> | <declareStmt> | <conditionalStmt> | <iterativeStmt>
    {
        node->addr = node->t.internalnode.children[0]->addr;

        free(node->t.internalnode.children[0]);
        break;
    }
    case 17: //<ioStmt> -> GET_VALUE BO ID BC SEMICOL | PRINT BO <var> BC SEMICOL
    {
        if (node->rule_no == 0)
        {
            astNode *ast = createNode(ioStmt_get_value_ist, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
        }
        else
        {
            astNode *ast = createNode(ioStmt_print_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
        }
        break;
    }
    case 18: //<var> -> <var_id_num> | <boolConstt>
    {
        node->addr = node->t.internalnode.children[0]->addr;

        free(node->t.internalnode.children[0]);
        break;
    }
    case 19: //<var_id_num> ->  ID <whichId> | NUM | RNUM
    {
        if (node->rule_no == 0)
        {
            astNode *ast = createNode(var_id_num_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[1]);
        }
        else
        {
            node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        }
        break;
    }
    case 20: //<boolConstt> -> TRUE | FALSE
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 21: //<whichId> ->  SQBO <index> SQBC | e
    {
        if (node->rule_no == 0)
        {
            node->addr = node->t.internalnode.children[1]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
        }
        else
        {
            node->addr = NULL;
            // astNode *ast = createNode(whichId_ast, NULL, NULL, NULL);
            // node->addr = ast;
        }
        break;
    }
    case 22: //<simpleStmt> -> <assignmentStmt> | <moduleReuseStmt>
    {
        node->addr = node->t.internalnode.children[0]->addr;
        free(node->t.internalnode.children[0]);
        break;
    }
    case 23: //<assignmentStmt> ->  ID <whichStmt>
    {
        astNode *ast = createNode(assignmentStmt_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[1]);
        break;
    }
    case 24: //<whichStmt> ->  <lvalueIDStmt> | <lvalueARRStmt>
    {
        node->addr = node->t.internalnode.children[0]->addr;
        free(node->t.internalnode.children[0]);
        break;
    }
    case 25: //<lvalueIDStmt> ->  ASSIGNOP <expression> SEMICOL
    {
        astNode *ast = createNode(lvalueIDStmt_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        break;
    }
    case 26: //<lvalueARRStmt> ->   SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
    {
        astNode *ast = createNode(lvalueARRStmt_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[4]);
        free(node->t.internalnode.children[5]);
        break;
    }
    case 27: //<index> ->  NUM | ID
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 28: //<moduleReuseStmt> ->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
    {
        astNode *ast = createNode(moduleReuseStmt_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[4]);
        free(node->t.internalnode.children[5]);
        free(node->t.internalnode.children[6]);
        free(node->t.internalnode.children[7]);
        break;
    }
    case 29: //<optional> ->  SQBO <idList> SQBC ASSIGNOP | e
    {
        if (!node->rule_no)
        {
            astNode *ast = createNode(optional_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
        }
        else
        {
            // node->addr = NULL;
            astNode *ast = createNode(-1, NULL, NULL, NULL);
            node->addr = ast;
        }
        break;
    }
    case 30: //<idList> -> ID <idList_dash>
    {
        astNode *ast = createNode(idList_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[1]);
        break;
    }
    case 31: //<idList_dash> -> COMMA ID <idList_dash> | e
    {
        if (!node->rule_no)
        {
            astNode *newNode = createNode(-1, NULL, NULL, node->t.internalnode.children[1]);
            node->t.internalnode.children[1]->addr = newNode;
            node->t.internalnode.children[1]->addr->sibling = node->t.internalnode.children[2]->addr;
            node->addr = node->t.internalnode.children[1]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[2]);
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 32: //<expression> -> <arithmeticOrBooleanExpr> | <op_plus_minus> <unaryOrExpr>
    {
        node->addr = node->t.internalnode.children[0]->addr;

        free(node->t.internalnode.children[0]);
        break;
    }
    case 33: //<unaryOrExpr> -> BO <arithmeticExpr> BC | <var_id_num>
    {
        if (!node->rule_no)
        {
            node->addr = node->t.internalnode.children[1]->addr;
        }
        else
        {
            node->addr = node->t.internalnode.children[0]->addr;
        }
        break;
    }
    case 34: //<arithmeticOrBooleanExpr> ->  <anyTerm> <bool>
    {
        if (node->t.internalnode.children[1]->addr)
        {
            astNode *temp = node->t.internalnode.children[1]->addr;
            Children *temp_child = temp->child_list;

            temp_child->head = node->t.internalnode.children[0]->addr;
            temp_child->head->sibling = temp_child->tail;
            temp_child->children_size++;

            node->addr = temp;
        }
        else
        {
            node->addr = node->t.internalnode.children[0]->addr;
        }
        break;
    }
    case 35: //<anyTerm> -> <arithmeticExpr> <arithmeticExpr_dash> | <boolConstt>
    {
        if (!node->rule_no)
        {
            if (node->t.internalnode.children[1]->addr)
            {
                astNode *temp = node->t.internalnode.children[1]->addr;
                Children *temp_child = temp->child_list;

                temp_child->head = node->t.internalnode.children[0]->addr;
                temp_child->head->sibling = temp_child->tail;
                temp_child->children_size++;

                node->addr = temp;
            }
            else
            {
                node->addr = node->t.internalnode.children[0]->addr;
            }
        }
        else
        {
            node->addr = node->t.internalnode.children[0]->addr;
            free(node->t.internalnode.children[0]);
        }
        break;
    }
    case 36: //<arithmeticExpr_dash> -> <relationalOp> <arithmeticExpr> | e
    {
        if (!node->rule_no)
        {
            astNode *newNode = createNode(arithmeticExpr_dash_ast, NULL, NULL, node->t.internalnode.children[0]);

            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;

            chi->head = node->t.internalnode.children[1]->addr;
            chi->tail = node->t.internalnode.children[1]->addr;
            chi->children_size++;

            newNode->child_list = chi;
            node->addr = newNode;
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 37: //<bool> -> <logicalOp> <anyTerm> <bool> | e
    {
        if (!node->rule_no)
        {
            astNode *newNode = createNode(bool_ast, NULL, NULL, node->t.internalnode.children[0]);

            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            if (node->t.internalnode.children[2]->addr == NULL)
            {
                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->children_size++;

                newNode->child_list = chi;
                node->addr = newNode;
            }
            else
            {
                astNode *temp = node->t.internalnode.children[2]->addr;
                Children *temp_child = temp->child_list;
                temp_child->head = node->t.internalnode.children[1]->addr;
                temp_child->head->sibling = temp_child->tail;
                temp_child->children_size++;

                chi->head = temp;
                chi->tail = temp;
                chi->children_size++;

                newNode->child_list = chi;
                node->addr = newNode;
            }
        }
        else
        {
            // node->addr = node->inh;
            node->addr = NULL;
        }
        break;
    }
    case 38: //<arithmeticExpr> -> <term>  <arithmeticExpr_recur>
    {
        if (node->t.internalnode.children[1]->addr)
        {
            astNode *temp = node->t.internalnode.children[1]->addr;
            Children *temp_child = temp->child_list;
            temp_child->head = node->t.internalnode.children[0]->addr;
            temp_child->head->sibling = temp_child->tail;
            temp_child->children_size++;

            node->addr = temp;
        }
        else
        {
            node->addr = node->t.internalnode.children[0]->addr;
        }
        break;
    }
    case 39: //<arithmeticExpr_recur> -> <op_plus_minus> <term> <arithmeticExpr_recur> | e
    {
        if (!node->rule_no)
        {
            astNode *newNode = createNode(arithmeticExpr_recur_ast, NULL, NULL, node->t.internalnode.children[0]);

            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            if (node->t.internalnode.children[2]->addr == NULL)
            {
                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->children_size++;

                newNode->child_list = chi;
                node->addr = newNode;
            }
            else
            {
                astNode *temp = node->t.internalnode.children[2]->addr;
                Children *temp_child = temp->child_list;
                temp_child->head = node->t.internalnode.children[1]->addr;
                temp_child->head->sibling = temp_child->tail;
                temp_child->children_size++;

                chi->head = temp;
                chi->tail = temp;
                chi->children_size++;

                newNode->child_list = chi;
                node->addr = newNode;
            }
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 40: //<term> ->  <factor> <term_dash>
    {
        if (node->t.internalnode.children[1]->addr)
        {
            astNode *temp = node->t.internalnode.children[1]->addr;
            Children *temp_child = temp->child_list;
            temp_child->head = node->t.internalnode.children[0]->addr;
            temp_child->head->sibling = temp_child->tail;
            temp_child->children_size++;

            node->addr = temp;
        }
        else
        {
            node->addr = node->t.internalnode.children[0]->addr;
        }
        break;
    }
    case 41: //<term_dash> -> <op_mul_div> <factor> <term_dash> | e
    {
        if (!node->rule_no)
        {
            astNode *newNode = createNode(term_dash_ast, NULL, NULL, node->t.internalnode.children[0]);

            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            if (node->t.internalnode.children[2]->addr == NULL)
            {
                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->children_size++;

                newNode->child_list = chi;
                node->addr = newNode;
            }
            else
            {
                astNode *temp = node->t.internalnode.children[2]->addr;
                Children *temp_child = temp->child_list;
                temp_child->head = node->t.internalnode.children[1]->addr;
                temp_child->head->sibling = temp_child->tail;
                temp_child->children_size++;

                chi->head = temp;
                chi->tail = temp;
                chi->children_size++;

                newNode->child_list = chi;
                node->addr = newNode;
            }
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 42: //<factor> ->  BO <arithmeticOrBooleanExpr> BC | <var_id_num>
    {
        if (!node->rule_no)
        {
            node->addr = node->t.internalnode.children[1]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
        }
        else
        {
            node->addr = node->t.internalnode.children[0]->addr;
            free(node->t.internalnode.children[0]);
        }
        break;
    }
    case 43: //<op_plus_minus> ->  PLUS | MINUS
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 44: //<op_mul_div> -> MUL | DIV
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 45: //<logicalOp> ->  AND | OR
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 46: //<relationalOp> ->  GT | LT | GE | LE | EQ | NE
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 47: //<declareStmt> -> DECLARE <idList> COLON <dataType> SEMICOL
    {
        astNode *ast = createNode(declareStmt_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        free(node->t.internalnode.children[4]);
        break;
    }
    case 48: //<conditionalStmt> ->  SWITCH BO ID BC START <caseStmts> <default> END
    {
        astNode *ast = createNode(conditionalStmt_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[3]);
        free(node->t.internalnode.children[4]);
        free(node->t.internalnode.children[5]);
        free(node->t.internalnode.children[6]);
        free(node->t.internalnode.children[7]);
        break;
    }
    case 49: //<caseStmts> ->  CASE <value> COLON <statements> BREAK SEMICOL <caseStmt>
    {
        astNode *ast = createNode(caseStmts_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        free(node->t.internalnode.children[4]);
        free(node->t.internalnode.children[5]);
        free(node->t.internalnode.children[6]);
        break;
    }
    case 50: //<caseStmt> ->  CASE <value> COLON <statements> BREAK SEMICOL <caseStmt> | e
    {
        if (!node->rule_no)
        {
            node->t.internalnode.children[1]->addr->sibling = node->t.internalnode.children[3]->addr;
            node->t.internalnode.children[3]->addr->sibling = node->t.internalnode.children[6]->addr;
            node->addr = node->t.internalnode.children[1]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
            free(node->t.internalnode.children[5]);
            free(node->t.internalnode.children[6]);
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 51: //<value> ->  NUM | TRUE | FALSE
    {
        node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        break;
    }
    case 52: //<default> ->  DEFAULT COLON <statements> BREAK SEMICOL | e
    {
        if (!node->rule_no)
        {
            node->addr = node->t.internalnode.children[2]->addr;

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 53: //<iterativeStmt> ->  FOR BO ID IN <range> BC START <statements> END | WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END
    {
        if (!node->rule_no)
        {
            astNode *ast = createNode(itr_for_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[7]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
            free(node->t.internalnode.children[5]);
            free(node->t.internalnode.children[6]);
            free(node->t.internalnode.children[8]);
        }
        else
        {
            astNode *ast = createNode(itr_while_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);

            free(node->t.internalnode.children[0]);
            free(node->t.internalnode.children[1]);
            free(node->t.internalnode.children[2]);
            free(node->t.internalnode.children[3]);
            free(node->t.internalnode.children[4]);
            free(node->t.internalnode.children[5]);
            free(node->t.internalnode.children[6]);
        }
        break;
    }
    case 54: //<range> ->  NUM RANGEOP NUM
    {
        astNode *ast = createNode(range_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);
    }
    }
}

// int main()
// {
// }