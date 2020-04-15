/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#include "semanticAnalyzer.h"

extern Table *baseTb;
Node *temp;

int flag1 = 0;
int flag2 = 0;

void traverse_AST(astNode *node)
{
    if (node == NULL)
        return;

    if (node->n_Name == module_ast)
    {
        Node *temp = lookUpST(node->child_list->head->tokenInfo->lexeme, baseTb);
        if (temp->SymbolTableNode->block.module_overloaded == 1 && temp->SymbolTableNode->block.flag_module == 1)
        {
            return;
        }
        traverseModule(node, temp->SymbolTableNode->block.output_list);
        if (temp->SymbolTableNode->block.module_overloaded == 1)
            temp->SymbolTableNode->block.flag_module = 1;
    }

    if (node->n_Name == moduleReuseStmt_ast)
    {
        if (node->child_list->head->n_Name == optional_ast) // if module returns some parameters
        {
            astNode *output_list = node->child_list->head->child_list->head->child_list->head;

            astNode *moduleName = node->child_list->head->sibling;

            astNode *input_list = node->child_list->head->sibling->sibling->child_list->head; // invoking parameters

            parameter_check(input_list, output_list, moduleName, node->tb);
        }
        else // return type is void
        {

            astNode *moduleName = node->child_list->head->sibling;

            astNode *input_list = node->child_list->head->sibling->sibling->child_list->head; // invoking parameters

            parameter_check(input_list, NULL, moduleName, node->tb);
        }
    }

    if (node->n_Name == assignmentStmt_ast)
    {

        if (node->parent->n_Name == itr_for_ast)
        {
            astNode *parent = node->parent;
            if (strcmp(parent->child_list->head->tokenInfo->lexeme, node->child_list->head->tokenInfo->lexeme) == 0)
            {
                printf("Line %d: 'For' loop variable %s cannot be assigned a value\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme);
                return;
            }
        }

        flag1 = 0;
        flag2 = 0;
        char *type;
        printf("Hi  -- %s\n", node->child_list->head->tokenInfo->lexeme);
        if (node->child_list->head->sibling->n_Name == lvalueARRStmt_ast)
        {
            type = processAssignmentStmt(node->child_list->head->sibling->child_list->head->sibling->sibling);
        }
        else //lvalueIDStmt_ast
        {
            astNode *rhs = node->child_list->head->sibling->child_list->head->sibling;
            if (rhs->n_Name == var_id_num_ast)
            {
                Node *n = lookupID(node->tb, rhs->child_list->head->tokenInfo->lexeme);
                if (n && n->SymbolTableNode->variable.var_tag == 1) //Array assignment. Do bound and type check
                {
                    check_array_assignment(node);
                }
                return;
            }
            type = processAssignmentStmt(node->child_list->head->sibling->child_list->head->sibling);
        }
        Node *typeLhs = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);

        if ((type == NULL && flag1 == 0) || (type && strcmp(type, typeLhs->SymbolTableNode->variable.addr) != 0 && flag1 == 0))
            printf("Line %d: Type Mismatch\n", node->child_list->head->tokenInfo->lineno);
    }

    if (node->n_Name == itr_while_ast)
    {
        // trie *trie = node->tb->parent->SymbolTableNode->block.trie;
        traverseWhile(node, node->tb);
        if (node->tb->parent->SymbolTableNode->block.addr == NULL)
            printf("Line %d: Condition variable inside While loop has not been updated\n", node->child_list->head->tokenInfo->lineno);
    }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        if (node->n_Name == module_ast)
        {
            Node *check = lookUpST(node->child_list->head->tokenInfo->lexeme, baseTb);
            List *list = check->SymbolTableNode->block.output_list; //output parameters of module from symbol table
            while (list)
            {
                if (list->assigned == 0)
                {
                    printf("Line %d: Output parameter '%s' has not been assigned any value inside module '%s'\n", node->child_list->head->tokenInfo->lineno, list->val, node->child_list->head->tokenInfo->lexeme);
                }
                list = list->next;
            }
            temp = NULL;
            // printf("Bye -- Module -- %s\n", nodeNameEnumToString[node->n_Name]);
        }

        while (temp)
        {
            traverse_AST(temp);
            temp = temp->sibling;
        }
    }
}

void traverseModule(astNode *node, List *output_list)
{
    if (node == NULL)
        return;

    if (node->n_Name == moduleReuseStmt_ast)
    {
        if (node->child_list->head->n_Name == optional_ast)
        {
            astNode *out_list = node->child_list->head->child_list->head->child_list->head;

            astNode *moduleName = node->child_list->head->sibling;

            astNode *input_list = node->child_list->head->sibling->sibling->child_list->head;

            parameter_check(input_list, out_list, moduleName, node->tb);

            //check whether the output variable has been assigned any value
            out_list = node->child_list->head->child_list->head->child_list->head;
            while (out_list)
            {
                List *out_node = retrieveOutList(output_list, out_list->tokenInfo->lexeme);
                if (out_node)
                {
                    out_node->assigned = 1;
                }
                out_list = out_list->sibling;
            }
        }
        else
        {
            astNode *moduleName = node->child_list->head->sibling;

            astNode *input_list = node->child_list->head->sibling->sibling->child_list->head;

            parameter_check(input_list, NULL, moduleName, node->tb);
        }
    }
    //assignment statement
    if (node->n_Name == assignmentStmt_ast)
    {
        if (node->parent->n_Name == itr_for_ast)
        {
            astNode *parent = node->parent;
            if (strcmp(parent->child_list->head->tokenInfo->lexeme, node->child_list->head->tokenInfo->lexeme) == 0)
            {
                printf("Line %d: 'For' loop variable %s cannot be assigned a value\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme);
                return;
            }
        }

        flag1 = 0;
        flag2 = 0;
        char *type;
        Node *typeLhs = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);
        if (typeLhs == NULL)
            return;
        // printf("Hi  -- %s\n", node->child_list->head->tokenInfo->lexeme);
        if (node->child_list->head->sibling->n_Name == lvalueARRStmt_ast)
        {
            Node *n = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);
            char *check = checkArray(n, node->child_list->head->sibling->child_list->head->child_list->head->tokenInfo, node->child_list->head->tokenInfo->lineno);

            type = processAssignmentStmt(node->child_list->head->sibling->child_list->head->sibling->sibling);
        }
        else //lvalueIDStmt_ast
        {
            astNode *rhs = node->child_list->head->sibling->child_list->head->sibling;
            if (rhs->n_Name == var_id_num_ast)
            {
                Node *n = lookupID(node->tb, rhs->child_list->head->tokenInfo->lexeme);
                if (n == NULL)
                    printf("%s----\n", rhs->child_list->head->tokenInfo->lexeme);
                if (n && n->SymbolTableNode->variable.var_tag == 1 && typeLhs && typeLhs->SymbolTableNode->variable.var_tag == 1) //Array assignment. Do bound and type check
                {
                    // printf("Here\n");
                    check_array_assignment(node);
                    return;
                }
                else if (n && typeLhs && typeLhs->SymbolTableNode->variable.var_tag == 1) //expression of type  array_variable = integer
                {
                    printf("Line %d: Variables '%s' and '%s' are of different types\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme, rhs->child_list->head->tokenInfo->lexeme);
                    return;
                }
            }
            type = processAssignmentStmt(node->child_list->head->sibling->child_list->head->sibling);
        }

        if ((type == NULL && flag1 == 0) || (type && strcmp(type, typeLhs->SymbolTableNode->variable.addr) != 0 && flag1 == 0))
            printf("Line %d: Type Mismatch\n", node->child_list->head->tokenInfo->lineno);

        //check whether the output variable has been assigned any value
        List *out_node = retrieveOutList(output_list, node->child_list->head->tokenInfo->lexeme);
        if (out_node)
        {
            out_node->assigned = 1;
        }
    }
    if (node->n_Name == ioStmt_get_value_ist)
    {
        //check whether the output variable has been assigned any value
        List *out_node = retrieveOutList(output_list, node->child_list->head->tokenInfo->lexeme);
        if (out_node)
        {
            out_node->assigned = 1;
        }
    }
    if (node->n_Name == itr_while_ast)
    {
        // trie *trie = node->tb->parent->SymbolTableNode->block.trie;
        traverseWhile(node, node->tb);
        // printf("Here\n");
        if (node->tb->parent->SymbolTableNode->block.addr == NULL)
            printf("Line %d: Condition variable inside While loop has not been updated\n", node->child_list->head->tokenInfo->lineno);
    }

    // if(node->n_Name == itr_for_ast)
    // {
    //     astNode* temp = node->child_list->head;

    // }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            traverseModule(temp, output_list);
            temp = temp->sibling;
        }
    }
}

void check_array_assignment(astNode *node)
{
    // printf("Array Bound Check\n");
    astNode *lhs = node->child_list->head;
    astNode *rhs = node->child_list->head->sibling->child_list->head->sibling->child_list->head;

    //bound check
    Node *n1 = lookupID(node->tb, lhs->tokenInfo->lexeme);
    Node *n2 = lookupID(node->tb, rhs->tokenInfo->lexeme);
    if (n1 && n2)
    {
        Leaf *n1_lower = n1->SymbolTableNode->variable.lower_index;
        Leaf *n1_upper = n1->SymbolTableNode->variable.upper_index;
        Leaf *n2_lower = n2->SymbolTableNode->variable.lower_index;
        Leaf *n2_upper = n2->SymbolTableNode->variable.upper_index;

        if (n1_lower->t == NUM && n1_upper->t == NUM && n2_lower->t == NUM && n2_upper->t == NUM)
        {
            if (n1_lower->val->intValue != n2_lower->val->intValue)
            {
                printf("Line %d: Mismatch in lower bound of '%s' and '%s' \n", lhs->tokenInfo->lineno, lhs->tokenInfo->lexeme, rhs->tokenInfo->lexeme);
            }
            if (n1_upper->val->intValue != n2_upper->val->intValue)
            {
                printf("Line %d: Mismatch in upper bound of '%s' and '%s' \n", lhs->tokenInfo->lineno, lhs->tokenInfo->lexeme, rhs->tokenInfo->lexeme);
            }
        }
    }
}

// returning the pointer where the variable is present in the list
List *retrieveOutList(List *list, char *val)
{
    List *temp = list;
    while (temp)
    {
        if (strcmp(temp->val, val) == 0)
            return temp;
        temp = temp->next;
    }
    return temp;
}

void parameter_check(astNode *input_list, astNode *output_list, astNode *moduleName, Table *tb)
{

    Node *temp = lookUpST(moduleName->tokenInfo->lexeme, baseTb);
    // printf("---%s\n", moduleName->tokenInfo->lexeme);
    // extracting the input and output parameter list for a module
    List *in_list = temp->SymbolTableNode->block.input_list;
    List *out_list = temp->SymbolTableNode->block.output_list;
    astNode *ast = input_list;
    while (in_list && ast) // checking invoking parameters and input list of module
    {
        Table *parentTable = tb;

        Node *n;
        while (parentTable->container)
        {
            n = lookUpST(ast->tokenInfo->lexeme, parentTable);
            if (n != NULL)
                break;
            parentTable = parentTable->container;
        }
        if (n)
        {
            // checking the type of the variables
            if (strcmp(n->SymbolTableNode->variable.addr, in_list->addr) != 0)
            {
                printf("Line %d: Input parameter type of '%s' does not match with formal parameter type which is '%s' \n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme, in_list->addr);
            }
            else if (n->SymbolTableNode->variable.var_tag == 1) //input variable is an array
            {
                if (in_list->var_tag == 1)
                {
                    // printf("Hello -- %s---%d\n", n->val, ast->tokenInfo->lineno);
                    Leaf *n1_lower = n->SymbolTableNode->variable.lower_index;
                    Leaf *n1_upper = n->SymbolTableNode->variable.upper_index;
                    Leaf *n2_lower = in_list->lower_index;
                    Leaf *n2_upper = in_list->upper_index;
                    if (n1_lower->t == NUM && n1_upper->t == NUM && n2_lower->t == NUM && n2_upper->t == NUM)
                    {
                        if ((n1_lower->val->intValue != n2_lower->val->intValue) || (n1_upper->val->intValue != n2_upper->val->intValue))
                            printf("Line %d: Input parameter type of '%s' does not match with formal parameter type \n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
                    }
                }
            }
        }
        in_list = in_list->next;
        ast = ast->sibling;
    }

    // different no of parameters are there
    if (ast || in_list)
    {
        printf("Line %d: Number of input parameters does not match with that of formal parameters for module '%s'\n", moduleName->tokenInfo->lineno, moduleName->tokenInfo->lexeme);
    }

    int flag = 0;
    ast = output_list;

    if (output_list != NULL && out_list == NULL)
    {
        printf("Line %d: Module %s shouldn't return anything\n", moduleName->tokenInfo->lineno, moduleName->tokenInfo->lexeme);
        flag = 1;
    }

    while (out_list && ast && flag == 0)
    {
        Table *parentTable = tb;
        Node *n;
        while (parentTable->container)
        {
            n = lookUpST(ast->tokenInfo->lexeme, parentTable);
            if (n != NULL)
                break;
            parentTable = parentTable->container;
        }
        if (n)
        {
            if (strcmp(n->SymbolTableNode->variable.addr, out_list->addr) != 0)
            {
                // printf("Line %d: Variable '%s' should be of '%s' type\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme, out_list->addr);
                printf("Line %d: Output parameter type of '%s' does not match with formal parameter type which is '%s'\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme, out_list->addr);
            }
        }
        out_list = out_list->next;
        ast = ast->sibling;
    }
    if ((ast || out_list) && flag == 0)
    {
        printf("Line %d: Number of output parameters does not match with that of formal parameters for module '%s'\n", moduleName->tokenInfo->lineno, moduleName->tokenInfo->lexeme);
    }
}

char *processAssignmentStmt(astNode *node)
{
    //if children is null, we have reached a terminal (operand). Return the type of the operand.
    if (node->child_list == NULL)
    {
        // check AstNode for the type
        if (node->tokenInfo->t == NUM)
        {
            // printf("%s\n", node->tokenInfo->lexeme);
            return "integer";
        }
        else if (node->tokenInfo->t == RNUM)
        {
            // printf("%s\n", node->tokenInfo->lexeme);
            return "real";
        }
        else if (node->tokenInfo->t == TRUE || node->tokenInfo->t == FALSE)
        {
            // printf("boolean\n");
            // printf("%s\n", node->tokenInfo->lexeme);
            return "boolean";
        }
    }
    // if the operand is a variable, lookup in the Symbol table for the type
    if (node->n_Name == var_id_num_ast)
    {
        // printf("var_id_num\n");
        if (node->child_list->head->sibling) //array variable
        {
            Node *n = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);
            // printf("%s-- %d -- %s\n", n->val, n->tag, n->SymbolTableNode->variable.addr);
            // printf("Hello1\n");
            if (n == NULL)
            {
                flag1 = 1;
                return NULL;
            }

            // n->SymbolTableNode->variable.var_tag = 1;
            // printf("Hello2\n");
            return checkArray(n, node->child_list->head->sibling->child_list->head->tokenInfo, node->child_list->head->tokenInfo->lineno);
        }
        else
        {

            Node *n = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);
            if (n)
            {
                if (n->SymbolTableNode->variable.var_tag == 1)
                {
                    printf("Line %d: Array variable %s should not be a part of any expression\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme);
                    flag1 = 1;
                    return NULL;
                }
                // printf("%s---%s\n", n->SymbolTableNode->variable.addr, node->child_list->head->tokenInfo->lexeme);
                return n->SymbolTableNode->variable.addr;
            }
            else
            {
                flag1 = 1;
                // printf("Hello1\n");
                return NULL;
            }
        }
    }

    //Else recursively call the children and then process the root (Post order traversal)

    char *type1 = NULL;
    char *type2 = NULL;

    Children *chi = node->child_list;
    astNode *temp = chi->head;

    type1 = processAssignmentStmt(temp);
    // if (type1 == NULL && flag1 == 0)
    // {
    //     flag1 = 1;
    // }
    temp = temp->sibling;
    if (temp)
    {
        type2 = processAssignmentStmt(temp);
        // if (type2 == NULL && flag1 == 0)
        // {
        //     flag1 = 1;
        // }
        temp = temp->sibling;
    }
    else //Unary Operation
        flag2 = 1;

    return processOperator(node->n_Name, type1, type2);
}

char *processOperator(nodeName name, char *type1, char *type2)
{
    //check if the types are eligible for the given operator
    if ((name == plus_ast) || (name == minus_ast) || (name == mul_ast) || (name == div_ast))
    {
        if (type2 == NULL && flag2 == 1)
            return type1;
        else if (type2 == NULL)
        {
            return NULL;
        }

        if (type1 == NULL)
            return NULL;
        if ((strcmp(type1, "integer") != 0 && strcmp(type1, "real") != 0) || (strcmp(type2, "integer") != 0 && strcmp(type2, "real") != 0))
        {
            return NULL;
        }
        else
        {
            if (strcmp(type1, type2) == 0)
            {
                // printf("%s----\n", type1);
                return type1;
            }

            else
            {
                return NULL;
            }
        }
    }
    else if ((name == le_ast) || (name == ge_ast) || (name == gt_ast) || (name == lt_ast))
    {

        if (type2 == NULL)
            return NULL;
        if (type1 == NULL)
            return NULL;
        if ((strcmp(type1, "integer") != 0 && strcmp(type1, "real") != 0) || (strcmp(type2, "integer") != 0 && strcmp(type2, "real") != 0))
        {
            return NULL;
        }
        else
        {
            if (strcmp(type1, type2) == 0)
            {
                // printf("%s----\n", "boolean");
                return "boolean";
            }
            else
            {
                return NULL;
            }
        }
    }
    else if ((name == ne_ast) || (name == eq_ast))
    {
        if ((strcmp(type1, type2) != 0))
        {
            return NULL;
        }
        return "boolean";
    }
    else if ((name == and_ast) || (name == or_ast))
    {
        if (type2 == NULL)
            return NULL;
        if (type1 == NULL)
            return NULL;
        if (strcmp(type1, "boolean") != 0 || strcmp(type2, "boolean") != 0)
            return NULL;
        else
        {
            // printf("%s----line 573\n", "boolean");
            return "boolean";
        }
    }
}

Node *lookupID(Table *tb, char *val)
{
    Table *parentTable = tb;
    Node *node = NULL;
    while (parentTable->container)
    {
        node = lookUpST(val, parentTable);
        if (node)
            return node;
        // node;
        else
            parentTable = parentTable->container;
    }
    parentTable = tb;
    while (parentTable->container != NULL && parentTable->container->container != NULL)
        parentTable = parentTable->container;
    if (node == NULL && parentTable->parent && strcmp(parentTable->parent->val, "drivermodule") != 0 && strcmp(parentTable->parent->SymbolTableNode->block.addr, "module") == 0)
    {
        List *in_list = parentTable->parent->SymbolTableNode->block.input_list;

        while (in_list)
        {
            if (strcmp(val, in_list->val) == 0)
            {
                Node *tempnode = malloc(sizeof(Node));
                tempnode->val = in_list->val;
                tempnode->tag = 0;
                tempnode->SymbolTableNode = malloc(sizeof(SymbolTable));
                tempnode->SymbolTableNode->variable.addr = in_list->addr;
                tempnode->SymbolTableNode->variable.upper_index = in_list->upper_index;
                tempnode->SymbolTableNode->variable.lower_index = in_list->lower_index;
                tempnode->SymbolTableNode->variable.var_tag = in_list->var_tag;
                // printf("%s----%s\n", in_list->addr, in_list->val);
                return tempnode;
            }
            in_list = in_list->next;
        }

        List *out_list = parentTable->parent->SymbolTableNode->block.output_list;
        while (out_list)
        {

            if (strcmp(val, out_list->val) == 0)
            {
                Node *tempnode = malloc(sizeof(Node));
                tempnode->val = out_list->val;
                tempnode->tag = 1;
                tempnode->SymbolTableNode = malloc(sizeof(SymbolTable));
                tempnode->SymbolTableNode->variable.addr = out_list->addr;
                tempnode->SymbolTableNode->variable.upper_index = out_list->upper_index;
                tempnode->SymbolTableNode->variable.lower_index = out_list->lower_index;
                tempnode->SymbolTableNode->variable.var_tag = out_list->var_tag;
                return tempnode;
            }
            out_list = out_list->next;
        }
    }
    return NULL;
}

char *checkArray(Node *node, Leaf *index, int lineNo)
{
    // printf("Hello--0\n");
    Leaf *lower_index = node->SymbolTableNode->variable.lower_index;
    // printf("Hello--0\n");
    Leaf *upper_index = node->SymbolTableNode->variable.upper_index;

    if (lower_index->t == NUM && upper_index->t == NUM)
    {
        // printf("Hello--2\n");
        if (index->t == NUM)
        {
            if (index->val->intValue < lower_index->val->intValue || index->val->intValue > upper_index->val->intValue) // Bound checking
            {
                printf("Line %d: Element of array '%s' is out of bound\n", lineNo, node->val);
                flag1 = 1;
                return NULL;
            }
        }
    }
    // printf("Hello--4  %s\n", node->SymbolTableNode->variable.addr);
    return node->SymbolTableNode->variable.addr; //type return
}

void traverseWhile(astNode *node, Table *tb)
{
    if (node == NULL)
        return;

    if (node->n_Name == assignmentStmt_ast)
    {
        char *val = node->child_list->head->tokenInfo->lexeme;

        int check = lookUp(tb->parent->SymbolTableNode->block.trie, val);
        // printf("%s---%d\n", val, check);
        if (check == 0)
            tb->parent->SymbolTableNode->block.addr = "true";
    }

    if (node->n_Name == ioStmt_get_value_ist)
    {
        //check whether the condition variable has been assigned any value
        int check = lookUp(tb->parent->SymbolTableNode->block.trie, node->child_list->head->tokenInfo->lexeme);
        if (check == 0)
        {
            tb->parent->SymbolTableNode->block.addr = "true";
        }
    }

    if (node->n_Name == moduleReuseStmt_ast)
    {
        if (node->child_list->head->n_Name == optional_ast)
        {
            //check whether the condition variable has been assigned any value
            astNode *out_list = node->child_list->head->child_list->head->child_list->head;
            while (out_list)
            {
                // List *out_node = retrieveOutList(output_list, out_list->tokenInfo->lexeme);
                int check = lookUp(tb->parent->SymbolTableNode->block.trie, out_list->tokenInfo->lexeme);
                if (check == 0)
                {
                    tb->parent->SymbolTableNode->block.addr = "true";
                    break;
                }
                out_list = out_list->sibling;
            }
        }
    }
    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            traverseWhile(temp, tb);
            temp = temp->sibling;
        }
    }
}