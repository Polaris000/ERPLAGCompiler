/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "symbolTable.h"
#include "semanticAnalyzer.h"
// #include "parser.h"
// #include "ast.h"
extern int flag1;
extern int flag2;
int driver_traversed = 0;
int global_offset = 0;

char *itoa(int num)
{
    int temp = num;
    int len = 0;
    while (temp > 0)
    {
        temp /= 10;
        len++;
    }
    char *result = malloc(sizeof(char) * (len + 1));
    result[len] = '\0';
    while (num > 0)
    {
        result[len - 1] = (char)((num % 10) + '0');
        len--;
        num /= 10;
    }
    // printf("%s\n", result);
    return result;
}

char *stringconcat(char *str1, char *str2)
{
    char *ptr = str1 + strlen(str1);
    // Appends characters of source to the destination string
    while (*str2 != '\0')
    {
        *ptr++ = *str2++;
    }
    // null terminate destination string

    *ptr = '\0';

    // printf("%s\n", str1);
    return str1;
}

Table *initialize_table(Node *parent, Table *container)
{
    Table *t = malloc(sizeof(Table));
    if (parent == NULL)
    {
        t->nodes = malloc(sizeof(Node *) * 7);
        t->num = 7;
        for (int i = 0; i < t->num; i++)
            t->nodes[i] = NULL;
    }
    //for, while, switch constructs
    else if (parent->val[0] - '0' >= 0 && parent->val[0] - '0' <= 9)
    {
        // printf("There------\n");
        t->nodes = malloc(sizeof(Node *) * 5);
        t->num = 5;
        for (int i = 0; i < t->num; i++)
            t->nodes[i] = NULL;
    }
    //Module Tables
    else
    {
        // printf("Module Initialize Table----\n");
        t->nodes = malloc(sizeof(Node *) * 41);
        t->num = 41;
        for (int i = 0; i < t->num; i++)
            t->nodes[i] = NULL;
    }
    t->parent = parent;
    t->container = container;
    t->size = 0;

    return t;
}

int getHash(char *val, int size)
{
    int i = 0;
    int hash_val = 0;

    while (val[i] != '\0')
    {

        hash_val = (((hash_val * 47) % size) + (val[i] % size)) % size;
        i++;
    }
    return hash_val;
}

void insert_in_Table(Node *node, Table *t, int hash)
{

    Node *temp = t->nodes[hash];
    if (temp != NULL)
    {
        //chaining
        temp = t->nodes[hash];
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = node;
    }
    else
    {
        //assigning directly
        t->nodes[hash] = node;
    }
    t->size++;
}

Table *insertST(char *val, Table *t, int tag)
{
    Node *node = malloc(sizeof(Node));
    node->next = NULL;
    node->SymbolTableNode = malloc(sizeof(SymbolTable));
    // printf("Alright\n");
    if (lookUpST(val, t) != NULL) // val already exists
    {
        return NULL;
    }

    int hash = getHash(val, t->num);
    if (tag == 0) // variable
    {
        // printf("Alright1------var\n");
        node->tag = 0;
        node->val = val;
        node->SymbolTableNode->variable.lower_index = NULL;
        node->SymbolTableNode->variable.upper_index = NULL;
    }
    else // block
    {
        node->tag = 1;
        node->val = val;
        node->SymbolTableNode->block.scope = initialize_table(node, t); //data
        node->SymbolTableNode->block.num_for = 1;
        node->SymbolTableNode->block.num_while = 1;
        node->SymbolTableNode->block.num_switch = 1;
        node->SymbolTableNode->block.input_list = NULL;
        node->SymbolTableNode->block.output_list = NULL;
        node->SymbolTableNode->block.trie = NULL;
        node->SymbolTableNode->block.module_overloaded = 0;
        node->SymbolTableNode->block.flag_module = 0;
        node->SymbolTableNode->block.addr = NULL;
    }

    // printf("Here1------\n");
    insert_in_Table(node, t, hash);

    if (node->tag == 1)
        return node->SymbolTableNode->block.scope;
    return t;
}

Node *lookUpST(char *val, Table *t)
{
    int hash = getHash(val, t->num);

    Node *temp = t->nodes[hash];
    if (temp != NULL)

        if (temp != NULL)
        {
            if (strcmp(temp->val, val) == 0)
            {
                return temp; // val already exists
            }
            else
            {
                while (temp->next != NULL)
                {
                    temp = temp->next;
                    if (temp->val == NULL)
                    {
                        // printf("Hi Compiler\n");
                        // printf("%s\n", temp->SymbolTableNode->variable.addr);
                        // printf("%d\n", temp->SymbolTableNode->variable.var_tag);
                    }

                    if (strcmp(temp->val, val) == 0)
                    {
                        return temp; // val already exists
                    }
                }
            }
        }
    /*
    if()
    */
    // printf("Here---- I a3m\n");
    return NULL; // new val found
}

Table *baseTb; // base table

Table *populateSymbolTable(astNode *root, error_list *list)
{

    baseTb = initialize_table(NULL, NULL); //creates the base table
    populateTableRecur(baseTb, root, list);

    return baseTb;
}

void populateTableRecur(Table *tb, astNode *root, error_list *list)
{
    //preorder traversal
    if (root == NULL)
        return;
    // printf("Hello\n");
    // if (root->n_Name == module_ast)
    //     check = lookUpST(root->child_list->head->tokenInfo->lexeme, baseTb);
    if (!(root->n_Name == driverModule_ast && tb->parent && strcmp(tb->parent->val, "drivermodule") == 0))
    {
        tb = processNode(tb, root, list);
    }
    else
    {
        // printf("Exceptional\n");
        // tb = (lookUpST(root->child_list->head->tokenInfo->lexeme, baseTb))->SymbolTableNode->block.scope;
    }

    Children *chi = root->child_list;
    // if (root->n_Name == eps && chi == NULL)
    //     printf("True Corona\n");
    astNode *temp;
    if (chi)
    {
        temp = chi->head;

        if (root->n_Name == driverModule_ast && lookUpST("drivermodule", tb) != NULL)
        {
            temp = NULL;
            // printf("I'm There driver\n");
        }
        if (root->n_Name == module_ast)
        {
            // printf("Hello1****%s\n", root->child_list->head->tokenInfo->lexeme);
            temp = NULL;
        }
        while (temp)
        {
            populateTableRecur(tb, temp, list);

            temp = temp->sibling;
        }
    }
}

Table *processNode(Table *tb, astNode *node, error_list *list)
{
    //check for module declarations
    if (node->n_Name == moduleDec_ast)
    {
        // printf("Hello----------1\n");
        Children *chi = node->child_list;
        astNode *temp = chi->head;

        Table *t1 = insertST(temp->tokenInfo->lexeme, tb, 1);
        if (t1 == NULL)
        {
            // printf("Line %d: Redeclaration of module '%s' \n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Redeclaration of module '%s' \n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        Node *n = lookUpST(temp->tokenInfo->lexeme, tb);
        n->SymbolTableNode->block.decl_required = 0;
        n->SymbolTableNode->block.defined = 0;
    }
    else if (node->n_Name == moduleDef_ast)
    {
        Node *n = tb->parent;
        n->SymbolTableNode->block.lower_scope = node->child_list->head->tokenInfo->lineno;
    }
    else if (node->n_Name == module_ast)
    {
        // printf("Hello----------2\n");
        // global_offset = 0;
        if (driver_traversed == 0)
        {
            Children *chi = node->child_list;
            astNode *temp;
            Node *check = lookUpST(chi->head->tokenInfo->lexeme, tb);
            if (check == NULL) //If module name doesn't exist
            {
                Table *t1 = insertST(chi->head->tokenInfo->lexeme, tb, 1);
                // temp = chi->head->sibling;
                // printf("%s\n", temp->child_list->head->tokenInfo->lexeme);
                Node *n = lookUpST(chi->head->tokenInfo->lexeme, tb);
                n->SymbolTableNode->block.decl_required = 1;
                n->SymbolTableNode->block.defined = -1;
                n->SymbolTableNode->block.addr = "module";
                processModule(t1, node, list);
            }
            else if (check && (check->SymbolTableNode->block.scope->size > 0 || check->SymbolTableNode->block.input_list))
            {
                // printf("Line %d: '%s' cannot be overloaded\n", chi->head->tokenInfo->lineno, chi->head->tokenInfo->lexeme);
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: '%s' cannot be overloaded\n", chi->head->tokenInfo->lineno, chi->head->tokenInfo->lexeme);
                appendError_at_head(list, err, chi->head->tokenInfo->lineno);
                check->SymbolTableNode->block.module_overloaded = 1;
                return tb;
            }
            else // If same name module already exists
            {
                Table *t1 = check->SymbolTableNode->block.scope;
                temp = chi->head;
                // Node *n = lookUpST(chi->head->tokenInfo->lexeme, tb);
                check->SymbolTableNode->block.decl_required = 0;
                check->SymbolTableNode->block.defined = 1;
                check->SymbolTableNode->block.addr = "module";
                processModule(t1, node, list);
            }
        }
        else
        {
            Children *chi = node->child_list;
            astNode *temp;
            Node *check = lookUpST(chi->head->tokenInfo->lexeme, tb);
            if (check == NULL)
            {
                // printf("Line %d: Module '%s' not declared before the driver module\n", chi->head->tokenInfo->lineno, chi->head->tokenInfo->lexeme);
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: Module '%s' not declared before the driver module\n", chi->head->tokenInfo->lineno, chi->head->tokenInfo->lexeme);
                appendError_at_head(list, err, chi->head->tokenInfo->lineno);

                Table *t1 = insertST(chi->head->tokenInfo->lexeme, tb, 1);
                // temp = chi->head->sibling;
                // printf("%s\n", temp->child_list->head->tokenInfo->lexeme);
                Node *n = lookUpST(chi->head->tokenInfo->lexeme, tb);
                n->SymbolTableNode->block.decl_required = 1;
                n->SymbolTableNode->block.defined = -1;
                n->SymbolTableNode->block.addr = "module";
                processModule(t1, node, list);
            }
            else if (check && (check->SymbolTableNode->block.scope->size > 0 || check->SymbolTableNode->block.input_list))
            {
                // printf("Line %d: '%s' cannot be overloaded\n", chi->head->tokenInfo->lineno, chi->head->tokenInfo->lexeme);
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: '%s' cannot be overloaded\n", chi->head->tokenInfo->lineno, chi->head->tokenInfo->lexeme);
                appendError_at_head(list, err, chi->head->tokenInfo->lineno);

                check->SymbolTableNode->block.module_overloaded = 1;
                return tb;
            }
            else
            {
                // printf("Hello---****************\n");
                Table *t1 = check->SymbolTableNode->block.scope;
                temp = chi->head->sibling;

                check->SymbolTableNode->block.decl_required = 0;
                check->SymbolTableNode->block.defined = 1;
                check->SymbolTableNode->block.addr = "module";
                processModule(t1, node, list);
            }
        }
        return tb;
    }
    //check for module use
    else if (node->n_Name == moduleReuseStmt_ast)
    {
        // printf("Hello----------3\n");
        Table *parent_table = tb;

        astNode *ast = node->child_list->head->sibling;
        // printf("Hello---\n");
        while (parent_table->container != NULL)
        {
            parent_table = parent_table->container;
        }
        Node *check = lookUpST(ast->tokenInfo->lexeme, parent_table);
        if (check == NULL)
        {
            // printf("Line %d: module '%s' is neither defined nor declared preceding this call\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: module '%s' is neither defined nor declared preceding this call\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
            appendError_at_head(list, err, ast->tokenInfo->lineno);
        }
        else if (check->SymbolTableNode->block.decl_required == 0 && strcmp(ast->tokenInfo->lexeme, check->val) != 0)
        {
            // printf("Here comes\n");
            if (check->SymbolTableNode->block.defined == 0)
                check->SymbolTableNode->block.decl_required = 1;
            else if (check->SymbolTableNode->block.defined == 1)
            {
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: Redundant declaration of module '%s'\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
                appendError_at_head(list, err, ast->tokenInfo->lineno);
            }
            // printf("Line %d: Redundant declaration of module '%s'\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
        }

        int recursion = checkRecursion(node, tb);
        if (recursion)
        {
            // printf("Line %d: Recursive Function call '%s()' encountered\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Recursive Function call '%s()' encountered\n", ast->tokenInfo->lineno, ast->tokenInfo->lexeme);
            appendError_at_head(list, err, ast->tokenInfo->lineno);
        }
        node->tb = tb;
    }

    //check for variable(ID) declarations
    else if (node->n_Name == declareStmt_ast)
    {
        // printf("Hello----------4\n");
        Children *chi = node->child_list;
        // printf("***%s***\n", dType);
        astNode *temp = chi->head->child_list->head;
        astNode *temp_type = chi->head->sibling->child_list->head; //Now temp denotes "range_array_ast"

        while (temp)
        {
            // printf("%s\n", temp->tokenInfo->lexeme);
            Table *t1 = insertST(temp->tokenInfo->lexeme, tb, 0);
            if (t1 == NULL)
            {
                // printf("Line %d: Multiple declaration of variable '%s' in the same scope\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: Multiple declaration of variable '%s' in the same scope\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
                appendError_at_head(list, err, temp->tokenInfo->lineno);
                temp = temp->sibling;
                continue;
            }

            Node *node = lookUpST(temp->tokenInfo->lexeme, tb);

            if (temp_type->tokenInfo == NULL) //temp_type denotes "range_array_ast"
            {
                node->SymbolTableNode->variable.addr = temp_type->sibling->tokenInfo->lexeme;
                node->SymbolTableNode->variable.var_tag = 1;
                astNode *grand_chi = temp_type->child_list->head; //grand_chi denotes index_ast

                node->SymbolTableNode->variable.lower_index = grand_chi->child_list->head->tokenInfo;
                node->SymbolTableNode->variable.upper_index = grand_chi->sibling->child_list->head->tokenInfo;
                node->SymbolTableNode->variable.offset = global_offset;
                if (node->SymbolTableNode->variable.lower_index->t == NUM && node->SymbolTableNode->variable.upper_index->t == NUM) //array is static
                {
                    int width = 0;
                    if (strcmp(temp_type->sibling->tokenInfo->lexeme, "integer") == 0)
                        width = 1 + (node->SymbolTableNode->variable.upper_index->val->intValue - node->SymbolTableNode->variable.lower_index->val->intValue + 1) * 2;
                    else if (strcmp(temp_type->sibling->tokenInfo->lexeme, "real") == 0)
                        width = 1 + (node->SymbolTableNode->variable.upper_index->val->intValue - node->SymbolTableNode->variable.lower_index->val->intValue + 1) * 4;
                    else if (strcmp(temp_type->sibling->tokenInfo->lexeme, "boolean") == 0)
                        width = 1 + (node->SymbolTableNode->variable.upper_index->val->intValue - node->SymbolTableNode->variable.lower_index->val->intValue + 1) * 1;

                    global_offset += width;
                    node->SymbolTableNode->variable.width = width;
                }
                else //dynamic array
                {
                    global_offset++;
                    node->SymbolTableNode->variable.width = 1;
                }
            }
            else
            {
                // printf("***%s***\n", temp_type->tokenInfo->lexeme);
                node->SymbolTableNode->variable.addr = temp_type->tokenInfo->lexeme;
                node->SymbolTableNode->variable.offset = global_offset;
                if (strcmp(temp_type->tokenInfo->lexeme, "integer") == 0)
                {
                    global_offset += 2;
                    node->SymbolTableNode->variable.width = 2;
                }

                else if (strcmp(temp_type->tokenInfo->lexeme, "real") == 0)
                {
                    global_offset += 4;
                    node->SymbolTableNode->variable.width = 4;
                }
                else if (strcmp(temp_type->tokenInfo->lexeme, "boolean") == 0)
                {
                    global_offset += 1;
                    node->SymbolTableNode->variable.width = 1;
                }
            }
            temp = temp->sibling;
        }
        return tb;
    }
    else if (node->n_Name == input_plist_ast) // module input parameter list
    {
        // printf("Hello----------5inp\n");
        global_offset = 0;
        astNode *temp = node->child_list->head;
        List *in_list = tb->parent->SymbolTableNode->block.input_list;
        while (temp)
        {

            astNode *temp_child = temp->sibling->child_list->head;
            if (temp_child->tokenInfo == NULL) //temp_child denotes "range_array_ast"
            {
                astNode *grand_chi = temp_child->child_list->head; //grand_chi denotes index_ast

                List *templist = createListNode(temp->tokenInfo->lexeme, temp_child->sibling->tokenInfo->lexeme, 1, grand_chi->child_list->head->tokenInfo, grand_chi->sibling->child_list->head->tokenInfo);
                // printf("%s\t %s\n", templist->lower_index->lexeme, templist->upper_index->lexeme);
                if (in_list != NULL)
                {
                    while (in_list->next)
                        in_list = in_list->next;
                    in_list->next = templist;
                }
                else
                {
                    tb->parent->SymbolTableNode->block.input_list = templist;
                    in_list = tb->parent->SymbolTableNode->block.input_list;
                }
            }
            else //normal variable
            {

                List *templist = createListNode(temp->tokenInfo->lexeme, temp_child->tokenInfo->lexeme, 0, NULL, NULL);

                if (in_list != NULL)
                {
                    while (in_list->next)
                        in_list = in_list->next;
                    in_list->next = templist;
                }
                else
                {
                    tb->parent->SymbolTableNode->block.input_list = templist;
                    in_list = tb->parent->SymbolTableNode->block.input_list;
                }
            }
            temp = temp->sibling->sibling;
        }
        if (node->sibling->n_Name != output_plist_ast)
            global_offset = 0;
    }
    else if (node->n_Name == output_plist_ast) //Inside module
    {
        // printf("Hello----------5op\n");
        astNode *temp = node->child_list->head;
        List *out_list = tb->parent->SymbolTableNode->block.output_list;
        while (temp)
        {
            List *templist = createListNode(temp->tokenInfo->lexeme, temp->sibling->tokenInfo->lexeme, 0, NULL, NULL);

            if (out_list != NULL)
            {
                while (out_list->next)
                    out_list = out_list->next;
                out_list->next = templist;
            }
            else
            {
                tb->parent->SymbolTableNode->block.output_list = templist;
                out_list = tb->parent->SymbolTableNode->block.output_list;
            }

            temp = temp->sibling;
            temp = temp->sibling;
        }

        global_offset = 0;
    }

    //check for variable use
    else if (node->tokenInfo != NULL && node->tokenInfo->t == ID && node->parent->parent->n_Name != declareStmt_ast && node->parent->n_Name != moduleDec_ast && node->parent->n_Name != moduleReuseStmt_ast && node->parent->n_Name != input_plist_ast && node->parent->n_Name != output_plist_ast && node->parent->n_Name != conditionalStmt_ast && node->parent->n_Name != index_ast)
    {
        // printf("Hello----------6\n");
        Table *parent_table = tb;
        Node *check;
        // printf("%s\t%d\n", node->tokenInfo->lexeme, node->tokenInfo->lineno);
        while (parent_table != NULL)
        {
            check = lookUpST(node->tokenInfo->lexeme, parent_table);
            if (check != NULL)
            {
                break;
            }
            parent_table = parent_table->container;
        }
        parent_table = tb;
        while (parent_table->container != NULL && parent_table->container->container != NULL)
            parent_table = parent_table->container;
        if (check == NULL && parent_table->parent && strcmp(parent_table->parent->val, "drivermodule") != 0 && strcmp(parent_table->parent->SymbolTableNode->block.addr, "module") == 0)
        {
            List *in_list = parent_table->parent->SymbolTableNode->block.input_list;
            while (in_list)
            {
                if (strcmp(node->tokenInfo->lexeme, in_list->val) == 0)
                {
                    check = parent_table->parent;
                    break;
                }
                in_list = in_list->next;
            }
            if (check == NULL)
            {
                List *out_list = parent_table->parent->SymbolTableNode->block.output_list;
                while (out_list)
                {
                    if (strcmp(node->tokenInfo->lexeme, out_list->val) == 0)
                    {
                        check = parent_table->parent;
                        break;
                    }
                    out_list = out_list->next;
                }
            }
        }
        if (check == NULL)
        {
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Variable '%s' is not declared \n", node->tokenInfo->lineno, node->tokenInfo->lexeme);
            appendError_at_head(list, err, node->tokenInfo->lineno);
        }
        // printf("Line %d: Variable '%s' is not declared \n", node->tokenInfo->lineno, node->tokenInfo->lexeme);
    }
    else if (node->n_Name == driverModule_ast)
    {
        driver_traversed = 1;
        Table *t1 = insertST("drivermodule", tb, 1);
        populateTableRecur(t1, node, list);
    }
    else if (node->n_Name == itr_for_ast)
    {
        // printf("Hello----------7\n");

        Table *t1 = insertST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_for), "for"), tb, 1);
        tb->parent->SymbolTableNode->block.num_for++; //increment the number of for loop encountered so far

        Children *chi = node->child_list;
        astNode *temp = chi->head;

        Node *check;
        Table *parentTable = tb;
        while (parentTable->container)
        {
            check = lookUpST(temp->tokenInfo->lexeme, parentTable);
            if (check)
                break;
            parentTable = parentTable->container;
        }

        if (check == NULL)
        {
            // printf("Line %d: Iterating variable '%s' is not declared\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Iterating variable '%s' is not declared\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }

        t1 = insertST(temp->tokenInfo->lexeme, t1, 0);
        Node *n = lookUpST(temp->tokenInfo->lexeme, t1);

        temp = temp->sibling; //temp points to range_ast

        if (check)
        {
            n->SymbolTableNode->variable.addr = check->SymbolTableNode->variable.addr;
            n->SymbolTableNode->variable.offset = check->SymbolTableNode->variable.offset;
        }

        n->SymbolTableNode->variable.lower_index = temp->child_list->head->tokenInfo;
        n->SymbolTableNode->variable.upper_index = temp->child_list->head->sibling->tokenInfo;
        // printf("%s -- %s\n", n->SymbolTableNode->variable.lower_index->lexeme, n->SymbolTableNode->variable.upper_index->lexeme);
        Node *where_for = lookUpST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_for - 1), "for"), tb);
        where_for->SymbolTableNode->block.lower_scope = temp->sibling->tokenInfo->lineno; //store the lower scope

        processModule(t1, node, list);
    }
    else if (node->n_Name == itr_while_ast)
    {
        // printf("Hello----------8\n");

        Table *t1 = insertST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_while), "while"), tb, 1);
        tb->parent->SymbolTableNode->block.num_while++; //increment the number of while loop encountered so far

        //check whether the condition of the while loop is boolean or not
        astNode *temp = node->child_list->head;
        if (!(temp->n_Name == le_ast || temp->n_Name == ge_ast || temp->n_Name == lt_ast || temp->n_Name == gt_ast || temp->n_Name == eq_ast || temp->n_Name == ne_ast || temp->n_Name == and_ast || temp->n_Name == or_ast))
        {
            // printf("Line %d: Condition for while loop must be a boolean expression\n", temp->tokenInfo->lineno);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Condition for while loop must be a boolean expression\n", temp->tokenInfo->lineno);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        Node *n = lookUpST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_while - 1), "while"), tb);

        n->SymbolTableNode->block.lower_scope = temp->sibling->tokenInfo->lineno; //to store the lower scope

        trie *trie = initialize_trie();
        trie = populateTrie(trie, node->child_list->head);
        n->SymbolTableNode->block.trie = trie;
        node->tb = t1;

        processModule(t1, node, list);
    }
    else if (node->n_Name == conditionalStmt_ast)
    {
        // printf("Hello----------9\n");

        Table *t1 = insertST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_switch), "switch"), tb, 1);

        Node *n = lookUpST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_switch), "switch"), tb);
        tb->parent->SymbolTableNode->block.num_switch++; //increment the number of switch loop encountered so far
        Children *chi = node->child_list;
        astNode *temp = chi->head;
        n->SymbolTableNode->block.lower_scope = temp->sibling->tokenInfo->lineno; //store the lower scope
        //check whether the conditional variable has been declared or not
        Table *parent_table = tb;
        Node *check;
        while (parent_table != NULL)
        {
            check = lookUpST(temp->tokenInfo->lexeme, parent_table);
            if (check != NULL)
            {
                break;
            }
            parent_table = parent_table->container;
        }
        if (check == NULL)
        {
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Condition Variable '%s' not declared\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        // printf("Line %d: Condition Variable '%s' not declared\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
        else if (strcmp(check->SymbolTableNode->variable.addr, "integer") != 0 && strcmp(check->SymbolTableNode->variable.addr, "boolean") != 0) //check for INteger or boolean
            // printf("Line %d: Condition Variable '%s' has to be of type integer or boolean\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
        {
            // printf("Line %d: switch variable '%s' cannot be of type real\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: switch variable '%s' cannot be of type real\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        else if (check->SymbolTableNode->variable.var_tag == 1)
        {
            // printf("Line %d: switch variable '%s' cannot be of type array\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: switch variable '%s' cannot be of type array\n", temp->tokenInfo->lineno, temp->tokenInfo->lexeme);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        else
        {
            n->SymbolTableNode->block.addr = check->SymbolTableNode->variable.addr;
        }

        // //If condition variable is of type Integer then default is compulsory
        if (check && strcmp(check->SymbolTableNode->variable.addr, "integer") == 0 && chi->tail->n_Name != default_ast)
        {
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: default statement is missing- the type of switch variable is integer\n", temp->tokenInfo->lineno);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        // printf("Line %d: default statement is missing- the type of switch variable is integer\n", temp->tokenInfo->lineno);
        else if (check && strcmp(check->SymbolTableNode->variable.addr, "boolean") == 0 && chi->tail->n_Name == default_ast)
        {
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Presence of default statement is incorrect as condiiton variable type is boolean\n", temp->tokenInfo->lineno);
            appendError_at_head(list, err, temp->tokenInfo->lineno);
        }
        // printf("Line %d: Presence of default statement is incorrect as condiiton variable type is boolean\n", temp->tokenInfo->lineno);

        node->tb = t1;
        processModule(t1, node, list);
    }

    else if (node->n_Name == caseStmts_ast)
    {
        // printf("Hello----------10\n");

        Children *chi = node->child_list;
        astNode *temp = chi->head;
        Terminal t;
        // printf("----%s----%d---%s\n", temp->tokenInfo->lexeme, temp->tokenInfo->t, tb->parent->SymbolTableNode->block.addr);
        if (tb->parent->SymbolTableNode->block.addr && strcmp(tb->parent->SymbolTableNode->block.addr, "integer") == 0)
        // t = INTEGER;
        {
            if (temp->tokenInfo->t != NUM)
            {
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: Case value is incorrect as condition variable type is %s\n", temp->tokenInfo->lineno, "integer");
                appendError_at_head(list, err, temp->tokenInfo->lineno);
            }
            // printf("Line %d: Case value is incorrect as condition variable type is %s\n", temp->tokenInfo->lineno, "integer");
        }
        else if (tb->parent->SymbolTableNode->block.addr && strcmp(tb->parent->SymbolTableNode->block.addr, "boolean") == 0)
        {
            if (strcmp(temp->tokenInfo->lexeme, "true") != 0 && strcmp(temp->tokenInfo->lexeme, "false") != 0)
            {
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: Case value is incorrect as condition variable type is %s\n", temp->tokenInfo->lineno, "boolean");
                appendError_at_head(list, err, temp->tokenInfo->lineno);
            }
            // printf("Line %d: Case value is incorrect as condition variable type is %s\n", temp->tokenInfo->lineno, "boolean");
        }
    }
    else if (node->n_Name == var_id_num_ast)
        node->tb = tb;

    else if (node->n_Name == assignmentStmt_ast)
    {
        // printf("Hello----------11\n");
        node->tb = tb;

        if (node->parent->n_Name == itr_for_ast)
        {
            astNode *parent = node->parent;
            if (strcmp(parent->child_list->head->tokenInfo->lexeme, node->child_list->head->tokenInfo->lexeme) == 0)
            {
                // printf("Line %d: 'For' loop variable %s cannot be assigned a value\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme);
                char *err = malloc(sizeof(char) * 200);
                sprintf(err, "Line %d: 'For' loop variable %s cannot be assigned a value\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme);
                appendError_at_head(list, err, node->child_list->head->tokenInfo->lineno);
                return tb;
            }
        }

        flag1 = 0;
        flag2 = 0;
        char *type;
        Node *typeLhs = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);
        if (typeLhs == NULL)
            return tb;
        // printf("Hi  -- %s\n", node->child_list->head->tokenInfo->lexeme);
        if (node->child_list->head->sibling->n_Name == lvalueARRStmt_ast)
        {
            Node *n = lookupID(node->tb, node->child_list->head->tokenInfo->lexeme);
            char *check = checkArray(n, node->child_list->head->sibling->child_list->head->child_list->head->tokenInfo, node->child_list->head->tokenInfo->lineno, list);

            type = processAssignmentStmt(node->child_list->head->sibling->child_list->head->sibling->sibling, tb, list);
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
                    check_array_assignment(node, list);
                    return tb;
                }
                else if (n && typeLhs && typeLhs->SymbolTableNode->variable.var_tag == 1) //expression of type  array_variable = integer
                {
                    // printf("Line %d: Variables '%s' and '%s' are of different types\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme, rhs->child_list->head->tokenInfo->lexeme);
                    char *err = malloc(sizeof(char) * 200);
                    sprintf(err, "Line %d: Variables '%s' and '%s' are of different types\n", node->child_list->head->tokenInfo->lineno, node->child_list->head->tokenInfo->lexeme, rhs->child_list->head->tokenInfo->lexeme);
                    appendError_at_head(list, err, node->child_list->head->tokenInfo->lineno);
                    return tb;
                }
            }
            // printf("Here\n");
            type = processAssignmentStmt(node->child_list->head->sibling->child_list->head->sibling, tb, list);
        }

        if ((type == NULL && flag1 == 0) || (type && strcmp(type, typeLhs->SymbolTableNode->variable.addr) != 0 && flag1 == 0))
        {
            char *err = malloc(sizeof(char) * 200);
            sprintf(err, "Line %d: Type Mismatch\n", node->child_list->head->tokenInfo->lineno);
            appendError_at_head(list, err, node->child_list->head->tokenInfo->lineno);
        }
        // printf("Line %d: Type Mismatch\n", node->child_list->head->tokenInfo->lineno);
    }

    else if (node->n_Name == ioStmt_print_ast)
        node->tb = tb;
    else if (node->n_Name == ioStmt_get_value_ist)
        node->tb = tb;
    else
    {
        // printf("Hello from else\n");
        // if ((int)node->n_Name > -1 && node->parent && (int)node->parent->n_Name > -1)
        //     printf("%s \t %s\n", nodeNameEnumToString[node->n_Name], nodeNameEnumToString[node->parent->n_Name]);

        return tb;
    }
    return tb;
}

void processModule(Table *tb, astNode *node, error_list *list)
{
    // printf("Hello from ModuleProcess\n");
    if (node == NULL)
        return;

    if (!(node->n_Name == module_ast || (node->n_Name == itr_for_ast && tb->container->parent && lookUpST(stringconcat(itoa(tb->container->parent->SymbolTableNode->block.num_for - 1), "for"), tb->container) != NULL) || (node->n_Name == conditionalStmt_ast && tb->container->parent && lookUpST(stringconcat(itoa(tb->container->parent->SymbolTableNode->block.num_switch - 1), "switch"), tb->container) != NULL) || (node->n_Name == itr_while_ast && tb->container->parent && lookUpST(stringconcat(itoa(tb->container->parent->SymbolTableNode->block.num_while - 1), "while"), tb->container) != NULL)))
    {
        // printf("Hello from modules populate\n");
        // if (node->n_Name == conditionalStmt_ast)
        //     printf("%s\n", nodeNameEnumToString[node->n_Name]);
        tb = processNode(tb, node, list);
    }
    else
    {
        // printf("Hello from modules populate\n");
    }

    Children *chi = node->child_list;

    astNode *temp;
    if (chi)
    {
        temp = chi->head;

        if ((node->n_Name == module_ast) || (node->n_Name == conditionalStmt_ast && tb->container->parent && lookUpST(stringconcat(itoa(tb->container->parent->SymbolTableNode->block.num_switch - 1), "switch"), tb->container) != NULL) || (node->n_Name == itr_while_ast && tb->container->parent && lookUpST(stringconcat(itoa(tb->container->parent->SymbolTableNode->block.num_while - 1), "while"), tb->container) != NULL))
        {
            temp = chi->head->sibling;
            // printf("I'm Here\n");
        }

        if (node->n_Name == itr_for_ast && tb->container->parent && lookUpST(stringconcat(itoa(tb->container->parent->SymbolTableNode->block.num_for - 1), "for"), tb->container) != NULL)
        {
            temp = chi->head->sibling->sibling;
            // printf("I'm Here\n");
        }
        if ((node->n_Name == conditionalStmt_ast && lookUpST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_switch - 1), "switch"), tb) != NULL))
        {
            temp = NULL;
            // printf("I'm There switch\n");
        }
        if ((node->n_Name == itr_for_ast && lookUpST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_for - 1), "for"), tb) != NULL))
        {
            temp = NULL;
            // printf("I'm There for\n");
        }
        if ((node->n_Name == itr_while_ast && lookUpST(stringconcat(itoa(tb->parent->SymbolTableNode->block.num_while - 1), "while"), tb) != NULL))
        {
            temp = NULL;
            // printf("I'm There while\n");
        }
        while (temp)
        {
            processModule(tb, temp, list);
            temp = temp->sibling;
        }
    }
}

void printSymbolTable(Table *tb)
{
    printf("%10s \t %20s \t %5s \t %5s \t %8s \t %8s \t %8s \t %8s \t %5s \t %5s\n",
           "variable_name", "scope(module_name)", "scope(line_num)", "width", "isArray", "static_or_dynamic", "range_lexemes",
           "type_of_element", "offset", "nesting_level");

    printSymbolTableUtil(tb);
}

void printTableEntry(Node *n, Table *tb)
{
    Table *parentTable = tb;
    while (parentTable->container && parentTable->container->parent)
    {
        parentTable = parentTable->container;
    }
    // if (strcmp(n->SymbolTableNode->variable.addr, "integer") == 0)
    //     printf("%10s \t %20s \t %5s \t %5s \t %8s \t %8s \t %8s \t %8s \t %5d \t %5s\n",
    //            n->val, parentTable->parent->val, "---", "2", "---", "---", "---", n->SymbolTableNode->variable.addr, n->SymbolTableNode->variable.offset, "---");
    // else if (strcmp(n->SymbolTableNode->variable.addr, "real") == 0)
    //     printf("%10s \t %20s \t %5s \t %5s \t %8s \t %8s \t %8s \t %8s \t %5d \t %5s\n",
    //            n->val, parentTable->parent->val, "---", "4", "---", "---", "---", n->SymbolTableNode->variable.addr, n->SymbolTableNode->variable.offset, "---");

    // else if (strcmp(n->SymbolTableNode->variable.addr, "boolean") == 0)
    //     printf("%10s \t %20s \t %5s \t %5s \t %8s \t %8s \t %8s \t %8s \t %5d \t %5s\n",
    //            n->val, parentTable->parent->val, "---", "1", "---", "---", "---", n->SymbolTableNode->variable.addr, n->SymbolTableNode->variable.offset, "---");
    printf("%10s \t %20s \t %5s \t %5d \t %8s \t %8s \t %8s \t %8s \t %5d \t %5s\n",
           n->val, tb->parent->val, "---", n->SymbolTableNode->variable.width, "---", "---", "---", n->SymbolTableNode->variable.addr, n->SymbolTableNode->variable.offset, "---");
}

void printSymbolTableUtil(Table *tb)
{
    for (int i = 0; i < tb->num; i++)
    {
        Node *node = tb->nodes[i];
        while (node != NULL)
        {
            if (node->tag == 0) // for variable
            {
                // printf("%s %15s\t %6s\t %d\n", node->val, node->SymbolTableNode->variable.addr, node->SymbolTableNode->variable.offset);
                printTableEntry(node, tb);
            }
            else // for block
            {
                // printf("%s %15s\n", str, node->val);
                // printf("\t %s", node-);
                if (node->SymbolTableNode->block.input_list)
                {
                    List *in_list = node->SymbolTableNode->block.input_list;
                    // printf("%s %s Printing input Parameter List\n", str, str);
                    while (in_list)
                    {
                        // printf("%s%s%s %s \t %s\n", str, str, str, in_list->val, in_list->addr);

                        Node *tempnode = malloc(sizeof(Node));
                        tempnode->val = in_list->val;
                        tempnode->tag = 0;
                        tempnode->SymbolTableNode = malloc(sizeof(SymbolTable));
                        tempnode->SymbolTableNode->variable.addr = in_list->addr;
                        tempnode->SymbolTableNode->variable.upper_index = in_list->upper_index;
                        tempnode->SymbolTableNode->variable.lower_index = in_list->lower_index;
                        tempnode->SymbolTableNode->variable.var_tag = in_list->var_tag;
                        tempnode->SymbolTableNode->variable.offset = in_list->offset;
                        tempnode->SymbolTableNode->variable.width = in_list->width;

                        printTableEntry(tempnode, node->SymbolTableNode->block.scope);
                        in_list = in_list->next;
                        free(tempnode);
                    }
                }
                if (node->SymbolTableNode->block.output_list)
                {
                    // printf("%s %s Printing output Parameter List\n", str, str);
                    List *out_list = node->SymbolTableNode->block.output_list;
                    while (out_list)
                    {
                        // printf("%s%s%s %s\t%s\n", str, str, str, out_list->val, out_list->addr);
                        Node *tempnode = malloc(sizeof(Node));
                        tempnode->val = out_list->val;
                        tempnode->tag = 1;
                        tempnode->SymbolTableNode = malloc(sizeof(SymbolTable));
                        tempnode->SymbolTableNode->variable.addr = out_list->addr;
                        tempnode->SymbolTableNode->variable.upper_index = out_list->upper_index;
                        tempnode->SymbolTableNode->variable.lower_index = out_list->lower_index;
                        tempnode->SymbolTableNode->variable.var_tag = out_list->var_tag;
                        tempnode->SymbolTableNode->variable.offset = out_list->offset;
                        tempnode->SymbolTableNode->variable.width = out_list->width;

                        printTableEntry(tempnode, node->SymbolTableNode->block.scope);
                        out_list = out_list->next;
                        free(tempnode);
                    }
                }

                printSymbolTableUtil(node->SymbolTableNode->block.scope);
            }
            node = node->next;
            printf("\n");
        }
    }
}

List *createListNode(char *val, char *addr, int var_tag, Leaf *lower_bound, Leaf *upper_bound)
{
    List *temp = malloc(sizeof(List));
    temp->val = val;
    temp->addr = addr;
    temp->var_tag = var_tag;
    temp->lower_index = lower_bound;
    temp->upper_index = upper_bound;
    temp->assigned = 0;
    temp->next = NULL;
    if (var_tag == 0 && strcmp(addr, "integer") == 0)
    {
        temp->offset = global_offset;
        global_offset += 2;
        temp->width = 2;
    }
    else if (var_tag == 0 && strcmp(addr, "real") == 0)
    {
        temp->offset = global_offset;
        global_offset += 4;
        temp->width = 4;
    }
    else if (var_tag == 0 && strcmp(addr, "boolean") == 0)
    {
        temp->offset = global_offset;
        global_offset += 1;
        temp->width = 1;
    }
    else if (var_tag == 1)
    {
        temp->offset = global_offset;
        global_offset += 5;
        temp->width = 5;
    }
    return temp;
}

int checkRecursion(astNode *node, Table *tb)
{
    Table *temp = tb;
    while (temp->parent && temp->container->parent)
        temp = temp->container;

    if (strcmp(temp->parent->val, node->child_list->head->sibling->tokenInfo->lexeme) == 0)
        return 1;

    return 0;
}

trie *populateTrie(trie *trie, astNode *node)
{
    if (node == NULL)
        return trie;

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            trie = populateTrie(trie, temp);
            temp = temp->sibling;
        }
    }
    if (node->tokenInfo && node->tokenInfo->t == ID)
    {
        // printf("%s\n", node->tokenInfo->lexeme);
        insert_trie(trie, node->tokenInfo->lexeme, 0);
    }
    return trie;
}
