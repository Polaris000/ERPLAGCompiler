
/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#include "ast.h"

char *nodeNameEnumToString[] = {"program_ast", "moduleDec_ast", "driverModule_ast", "module_ast", "input_plist_ast", "output_plist_ast", "dataType_ast", "whichId_ast", "range_array_ast", "ioStmt_get_value_ast",
                                "ioStmt_print_ast", "var_id_num_ast", "moduleDef_ast", "assignmentStmt_ast", "lvalueIDStmt_ast", "lvalueARRStmt_ast",
                                "moduleReuseStmt_ast", "optional_ast", "idList_ast", "expression_ast", "arithmeticExpr_dash_ast", "bool_ast", "arithmeticExpr_recur_ast", "term_dash_ast", "declareStmt_ast",
                                "conditionalStmt_ast", "caseStmts_ast", "itr_for_ast", "itr_while_ast", "range_ast", "eps", "plus_ast", "minus_ast", "mul_ast", "div_ast", "and_ast", "or_ast", "num_ast", "rnum_ast", "gt_ast", "lt_ast", "ge_ast", "le_ast", "eq_ast", "ne_ast", "index_ast", "default_ast"};

astNode *postOrder_ParseTree(ParseTree *pt)
{
    if (pt->root == NULL)
        return NULL;

    postOrderRecur(pt->root);
    // printf("Here---hello\n");
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
            // if (node->t.internalnode.nodesym == program)
            // printf("child of program %d\n", size);
            int i = 0;
            while (size--)
            {
                postOrderRecur(children[i]);
                i++;
            }
        }

        // if (node->t.internalnode.nodesym == program)
        //     printf("program-\n");
        // printf("Here - %s\n", nonTerminalEnumToString[data.internalnode.nodesym]);
        // printf("Hello -- %s -- \n", nonTerminalEnumToString[node->t.internalnode.nodesym]);
        applyASTRule_Node(node);
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
    temp->tb = NULL;

    return temp;
}
int isValidterminal(Terminal t)
{
    if (t == ID || t == NUM || t == RNUM || t == ASSIGNOP || t == START || t == END)
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
        // if (node->t.internalnode.nodesym == driverModule)
        //     printf("%d -- %d\n", i, size);
        temp = node->t.internalnode.children[i]->addr;
        if (node->t.internalnode.children[i]->tag == 0)
        {
            // if (node->t.internalnode.nodesym == driverModule)
            //     printf("Alright\n");
            if (j == 0)
            {
                chi->head = temp;
                chi->tail = temp;
                // chi->head->sibling = chi->tail;
                while (chi->tail && chi->tail->sibling)
                    chi->tail = chi->tail->sibling;
                chi->children_size++;
                // if (node->t.internalnode.nodesym == driverModule)
                //     printf("Alright1\n");
            }
            else
            {
                chi->tail->sibling = temp;
                chi->children_size++;
                while (chi->tail && chi->tail->sibling)
                    chi->tail = chi->tail->sibling;
                // chi->tail = temp;
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
                // while (chi->tail->sibling)
                //     chi->tail = chi->tail->sibling;
                // chi->head->sibling = chi->tail;
                chi->children_size++;
            }
            else
            {
                chi->tail->sibling = temp;
                chi->children_size++;
                // while (chi->tail->sibling)
                //     chi->tail = chi->tail->sibling;
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
    int size = ast->child_list->children_size;
    // if (ast->n_Name == program)
    //     printf("AllIsWell --%d--%s\n", size,nodeNameEnumToString[]);
    while (temp)
    {
        // if (!temp->tokenInfo)
        //     printf("%s --child--%s\n", nodeNameEnumToString[ast->n_Name], nodeNameEnumToString[temp->n_Name]);
        temp->parent = ast;
        temp = temp->sibling;
    }
}

void applyASTRule_Node(TreeNode *node)
{
    int rule_no = node->t.internalnode.nodesym;
    // printf("%d\n", node->rule_no);
    switch (rule_no)
    {
    case 0: //<program> -> <moduleDeclarations> <otherModules> <driverModule> <otherModules>
    {
        // printf("Here-----ast\n");
        astNode *ast = createNode(program_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);

        // printf("Here----ast_end\n");
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
            astNode *ast = createNode(eps, NULL, NULL, NULL);
            node->addr = ast;
            // printf("Here--module\n");
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
        // printf("Here\n");
        astNode *ast = createNode(driverModule_ast, NULL, NULL, NULL);
        populateChild(node, ast);
        populateParent(ast);

        free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        free(node->t.internalnode.children[2]);
        free(node->t.internalnode.children[3]);
        free(node->t.internalnode.children[4]);
        // printf("Here_end\n");
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
            astNode *ast = createNode(eps, NULL, NULL, NULL);
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
            // node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
            astNode *ast = createNode(dataType_ast, NULL, NULL, NULL);
            node->addr = ast;
            astNode *temp = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
            Children *chi = malloc(sizeof(Children));
            chi->head = temp;
            chi->tail = temp;
            chi->head->parent = ast;
            ast->child_list = chi;
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
        // node->addr = node->t.internalnode.children[1]->addr;

        astNode *ast = createNode(moduleDef_ast, NULL, NULL, NULL);

        populateChild(node, ast);
        // printf("Hello\n");
        populateParent(ast);

        // free(node->t.internalnode.children[0]);
        free(node->t.internalnode.children[1]);
        // free(node->t.internalnode.children[2]);

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
        else if (node->rule_no == 1)
        {
            node->addr = createNode(num_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else
        {
            node->addr = createNode(rnum_ast, NULL, NULL, node->t.internalnode.children[0]);
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
        // node->addr = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        astNode *newNode = createNode(index_ast, NULL, NULL, NULL);
        Children *chi = malloc(sizeof(Children));
        chi->head = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        chi->tail = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
        chi->head->parent = newNode;
        newNode->child_list = chi;
        node->addr = newNode;
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
            astNode *ast = createNode(eps, NULL, NULL, NULL);
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
        if (!node->rule_no)
        {
            node->addr = node->t.internalnode.children[0]->addr;

            free(node->t.internalnode.children[0]);
        }
        else
        {
            // astNode *newNode = createNode(-1, NULL, NULL, node->t.internalnode.children[0]);
            astNode *newNode = node->t.internalnode.children[0]->addr;
            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;

            chi->head = node->t.internalnode.children[1]->addr;
            chi->tail = node->t.internalnode.children[1]->addr;
            chi->children_size++;
            chi->head->parent = newNode;
            newNode->child_list = chi;

            node->addr = newNode;
            // printf("%s\n", node->addr->child_list->head->tokenInfo->lexeme);
        }
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
            node->addr = temp;
            while (temp_child->children_size == 2)
            {
                temp = temp_child->head;
                temp_child = temp_child->head->child_list;
            }

            temp_child->head = node->t.internalnode.children[0]->addr;
            temp_child->head->sibling = temp_child->tail;
            temp_child->head->parent = temp;
            temp_child->children_size++;

            // node->addr = temp;
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
                temp_child->head->parent = temp;
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
            // astNode *newNode = createNode(arithmeticExpr_dash_ast, NULL, NULL, node->t.internalnode.children[0]);
            astNode *newNode = node->t.internalnode.children[0]->addr;
            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;

            chi->head = node->t.internalnode.children[1]->addr;
            chi->tail = node->t.internalnode.children[1]->addr;
            chi->head->parent = newNode;
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
            // astNode *newNode = createNode(bool_ast, NULL, NULL, node->t.internalnode.children[0]);
            astNode *newNode = node->t.internalnode.children[0]->addr;
            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            if (node->t.internalnode.children[2]->addr == NULL)
            {
                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->head->parent = newNode;
                chi->children_size++;

                newNode->parent = node->addr;
                newNode->child_list = chi;
                node->addr = newNode;
            }
            else
            {
                // astNode *temp = node->t.internalnode.children[2]->addr;
                // Children *temp_child = temp->child_list;
                // temp_child->head = node->t.internalnode.children[1]->addr;
                // temp_child->head->sibling = temp_child->tail;
                // temp_child->head->parent = temp;
                // temp_child->children_size++;

                // chi->head = temp;
                // chi->tail = temp;
                // chi->children_size++;
                // chi->head->parent = newNode;

                // newNode->parent = node->addr;
                // newNode->child_list = chi;
                // node->addr = newNode;

                astNode *temp = node->t.internalnode.children[2]->addr;
                astNode *newNode2 = node->t.internalnode.children[0]->addr; //node of +/-

                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->children_size++;
                chi->head->parent = newNode2;
                newNode2->child_list = chi;

                Children *temp_child = temp->child_list;
                while (temp_child->children_size == 2)
                {
                    temp = temp_child->head;
                    temp_child = temp_child->head->child_list;
                }
                temp_child->head = newNode2;
                temp_child->head->sibling = temp_child->tail;
                temp_child->head->parent = temp;
                temp_child->children_size++;

                node->addr = node->t.internalnode.children[2]->addr;
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
            node->addr = temp;
            while (temp_child->children_size == 2)
            {
                temp = temp_child->head;
                temp_child = temp_child->head->child_list;
            }

            temp_child->head = node->t.internalnode.children[0]->addr;
            temp_child->head->sibling = temp_child->tail;
            temp_child->head->parent = temp;
            temp_child->children_size++;

            // node->addr = temp;
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
            // astNode *newNode = createNode(arithmeticExpr_recur_ast, NULL, NULL, node->t.internalnode.children[0]);
            astNode *newNode = node->t.internalnode.children[0]->addr;
            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            if (node->t.internalnode.children[2]->addr == NULL)
            {
                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->head->parent = newNode;
                chi->children_size++;

                newNode->parent = node->addr;
                newNode->child_list = chi;
                node->addr = newNode;
            }
            else
            {
                // astNode *temp = node->t.internalnode.children[2]->addr;
                // Children *temp_child = temp->child_list;
                // temp_child->head = node->t.internalnode.children[1]->addr;
                // temp_child->head->sibling = temp_child->tail;
                // temp_child->head->parent = temp;
                // temp_child->children_size++;

                // chi->head = temp;
                // chi->tail = temp;
                // chi->children_size++;
                // chi->head->parent = newNode;

                // newNode->child_list = chi;
                // node->addr = newNode;

                astNode *temp = node->t.internalnode.children[2]->addr;
                astNode *newNode2 = node->t.internalnode.children[0]->addr; //node of +/-

                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->children_size++;
                chi->head->parent = newNode2;
                newNode2->child_list = chi;

                Children *temp_child = temp->child_list;
                while (temp_child->children_size == 2)
                {
                    temp = temp_child->head;
                    temp_child = temp_child->head->child_list;
                }
                temp_child->head = newNode2;
                temp_child->head->sibling = temp_child->tail;
                temp_child->head->parent = temp;
                temp_child->children_size++;

                node->addr = node->t.internalnode.children[2]->addr;
            }
        }
        else
        {
            node->addr = NULL;
        }
        break;
    }
    case 40: //<term> ->  <factor><term_dash>
    {
        if (node->t.internalnode.children[1]->addr)
        {
            astNode *temp = node->t.internalnode.children[1]->addr;
            Children *temp_child = temp->child_list;
            node->addr = temp;
            while (temp_child->children_size == 2)
            {
                temp = temp_child->head;
                temp_child = temp_child->head->child_list;
            }
            temp_child->head = node->t.internalnode.children[0]->addr;
            temp_child->head->sibling = temp_child->tail;
            temp_child->head->parent = temp;
            temp_child->children_size++;

            // node->addr = temp;
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
            // astNode *newNode = createNode(term_dash_ast, NULL, NULL, node->t.internalnode.children[0]);
            astNode *newNode = node->t.internalnode.children[0]->addr;
            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            if (node->t.internalnode.children[2]->addr == NULL)
            {
                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->head->parent = newNode;
                chi->children_size++;

                newNode->parent = node->addr;
                newNode->child_list = chi;
                node->addr = newNode;
            }
            else
            {
                // astNode *temp = node->t.internalnode.children[2]->addr;
                // Children *temp_child = temp->child_list;
                // temp_child->head = node->t.internalnode.children[1]->addr;
                // temp_child->head->parent = temp;
                // temp_child->head->sibling = temp_child->tail;
                // temp_child->children_size++;

                // chi->head = temp;
                // chi->tail = temp;
                // chi->children_size++;
                // chi->head->parent = newNode;

                // newNode->child_list = chi;
                // node->addr = newNode;

                astNode *temp = node->t.internalnode.children[2]->addr;
                astNode *newNode2 = node->t.internalnode.children[0]->addr; //node of * | /

                chi->head = node->t.internalnode.children[1]->addr;
                chi->tail = node->t.internalnode.children[1]->addr;
                chi->children_size++;
                chi->head->parent = newNode2;
                newNode2->child_list = chi;

                Children *temp_child = temp->child_list;
                while (temp_child->children_size == 2)
                {
                    temp = temp_child->head;
                    temp_child = temp_child->head->child_list;
                }
                temp_child->head = newNode2;
                temp_child->head->sibling = temp_child->tail;
                temp_child->head->parent = temp;
                temp_child->children_size++;

                node->addr = node->t.internalnode.children[2]->addr;
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
            node->t.internalnode.children[0]->addr->parent = node->addr;
            free(node->t.internalnode.children[0]);
        }
        break;
    }
    case 43: //<op_plus_minus> ->  PLUS | MINUS
    {
        if (!node->rule_no)
        {
            node->addr = createNode(plus_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else
        {
            node->addr = createNode(minus_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        break;
    }
    case 44: //<op_mul_div> -> MUL | DIV
    {
        if (!node->rule_no)
        {
            node->addr = createNode(mul_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else
        {
            node->addr = createNode(div_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        break;
    }
    case 45: //<logicalOp> ->  AND | OR
    {
        if (!node->rule_no)
        {
            node->addr = createNode(and_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else
        {
            node->addr = createNode(or_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        break;
    }
    case 46: //<relationalOp> ->  GT | LT | GE | LE | EQ | NE
    {
        if (!node->rule_no)
        {
            node->addr = createNode(gt_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else if (node->rule_no == 1)
        {
            node->addr = createNode(lt_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else if (node->rule_no == 2)
        {
            node->addr = createNode(ge_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else if (node->rule_no == 3)
        {
            node->addr = createNode(le_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else if (node->rule_no == 4)
        {
            node->addr = createNode(eq_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
        else if (node->rule_no == 5)
        {
            node->addr = createNode(ne_ast, NULL, NULL, node->t.internalnode.children[0]);
        }
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
        // free(node->t.internalnode.children[4]);
        free(node->t.internalnode.children[5]);
        free(node->t.internalnode.children[6]);
        // free(node->t.internalnode.children[7]);
        break;
    }
    case 49: //<caseStmts> ->  CASE <value> COLON <statements> BREAK SEMICOL <caseStmt>
    {
        // astNode *ast = createNode(caseStmts_ast, NULL, NULL, NULL);
        // populateChild(node, ast);
        // populateParent(ast);
        astNode *ast = createNode(caseStmts_ast, NULL, NULL, NULL);
        Children *chi = malloc(sizeof(Children));
        chi->children_size = 0;
        chi->head = node->t.internalnode.children[1]->addr;
        chi->tail = node->t.internalnode.children[1]->addr;
        chi->children_size++;

        chi->tail->sibling = node->t.internalnode.children[3]->addr;
        chi->children_size++;
        while (chi->tail)
        {
            chi->tail->parent = ast;
            chi->tail = chi->tail->sibling;
        }

        ast->child_list = chi;
        ast->sibling = node->t.internalnode.children[6]->addr;
        node->addr = ast;

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
            astNode *ast = createNode(caseStmts_ast, NULL, NULL, NULL);
            Children *chi = malloc(sizeof(Children));
            chi->children_size = 0;
            chi->head = node->t.internalnode.children[1]->addr;
            chi->tail = node->t.internalnode.children[1]->addr;
            chi->children_size++;

            chi->tail->sibling = node->t.internalnode.children[3]->addr;
            chi->children_size++;
            while (chi->tail)
            {
                chi->tail->parent = ast;
                chi->tail = chi->tail->sibling;
            }

            ast->child_list = chi;
            ast->sibling = node->t.internalnode.children[6]->addr;
            node->addr = ast;
            // ast->parent = node->addr;
            // node->t.internalnode.children[1]->addr->sibling = node->t.internalnode.children[3]->addr;
            // node->t.internalnode.children[3]->addr->sibling = node->t.internalnode.children[6]->addr;
            // node->addr = node->t.internalnode.children[1]->addr;

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
            astNode *ast = createNode(default_ast, NULL, NULL, NULL);
            populateChild(node, ast);
            populateParent(ast);
            // node->addr = node->t.internalnode.children[2]->addr; //make a node here for semantic check

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
            // free(node->t.internalnode.children[6]);
            // free(node->t.internalnode.children[8]);
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
            // free(node->t.internalnode.children[4]);
            free(node->t.internalnode.children[5]);
            // free(node->t.internalnode.children[6]);
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

void printAst(astNode *node)
{
    printf("%20s\t %20s\t %5s\n\n", "NodeName", "Parent", "LineNo");
    printAstRecur(node);
}

void printAstNode(astNode *node)
{
    char *parent;
    if (node->parent == NULL)
        parent = "NULL";
    else
    {
        parent = nodeNameEnumToString[node->parent->n_Name];
    }
    if (!node->tokenInfo)
        printf("%20s\t %20s\t %5s\n", nodeNameEnumToString[node->n_Name], parent, "---");
    else
    {
        //     if (node->tokenInfo->t == END || node->tokenInfo->t == START)
        //         printf("%s----Here End -- %d\n", TerminalEnumToString[node->tokenInfo->t], node->tokenInfo->lineno);
        if (node->tokenInfo->t != EPSILON)
            printf("%20s\t %20s\t %5d\n", node->tokenInfo->lexeme, parent, node->tokenInfo->lineno);
    }
}

void printAstRecur(astNode *node)
{
    if (node == NULL)
        return;

    printAstNode(node);

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            printAstRecur(temp);
            temp = temp->sibling;
        }
    }
}

void Compression_Ratio_calc(TreeNode *root, astNode *root_ast)
{
    int PTree_num = 0;
    int ptree_mem = 0;
    PTreeRecur(root, &PTree_num, &ptree_mem);

    int ast_num = 0;
    int ast_mem = 0;
    ASTRecur(root_ast, &ast_num, &ast_mem);

    printf("\n\nParse tree Number of nodes = %d\n", PTree_num);
    printf("Allocated Memory = %d Bytes\n", ptree_mem);

    printf("\nAST Number of nodes = %d\n", ast_num);
    printf("Allocated Memory = %d Bytes\n", ast_mem);

    double comp_ratio = ((ptree_mem - ast_mem) / (double)ptree_mem) * 100;

    printf("Compression Precentage = %lf %%\n\n", comp_ratio);
}

void PTreeRecur(TreeNode *root, int *count, int *mem)
{
    if (root == NULL)
        return;

    TreeNode *tmp = root;

    if (root->tag == 0)
    {
        for (int i = 0; i < tmp->t.internalnode.rhs_size; i++)
        {
            PTreeRecur(tmp->t.internalnode.children[i], count, mem);
        }
    }

    *count = *count + 1;
    *mem = *mem + sizeof(root);
}

void ASTRecur(astNode *node, int *count, int *mem)
{
    if (node == NULL)
        return;

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            ASTRecur(temp, count, mem);
            temp = temp->sibling;
        }
    }

    *count = *count + 1;
    *mem = *mem + sizeof(node);
}