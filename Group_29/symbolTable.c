/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "symbolTable.h"
#include "parser.h"

int driver_traversed = 0;
int num_for = 0, num_while = 0, num_switch = 0;

char *itoa(int num)
{
    int temp = num;
    int len = 0;
    while (temp > 0)
    {
        temp /= 10;
        len++;
    }
    //result array of length len
    char *result = malloc(sizeof(char) * len);
    while (num > 0)
    {
        result[len - 1] = (char)((num % 10) + '0');
        len--;
        num /= 10;
    }
    return result;
}

Table *initialize_table(Node *parent, Table *container)
{
    //Global Table
    Table *t = malloc(sizeof(Table));
    if (parent == NULL)
    {
        t->nodes = malloc(sizeof(Node *) * 23);
        t->num = 23;
        for (int i = 0; i < t->num; i++)
            t->nodes[i] = NULL;
    }
    //for, while, switch constructs
    else if (strcmp(substring(parent->val, 0, 3), "for") || strcmp(substring(parent->val, 0, 5), "while") ||
             strcmp(substring(parent->val, 0, 6), "switch"))
    {
        t->nodes = malloc(sizeof(Node *) * 5);
        t->num = 5;
        for (int i = 0; i < t->num; i++)
            t->nodes[i] = NULL;
    }
    //Module Tables
    else
    {
        t->nodes = malloc(sizeof(Node *) * 41);
        t->num = 41;
        for (int i = 0; i < t->num; i++)
            t->nodes[i] = NULL;
    }
    t->parent = parent;
    t->container = container;
    t->size = 0;
    // t->nodes = malloc(t->num * sizeof(Node *));
    return t;
}

int getHash(char *val, int size)
{
    // printf("There------\n");
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
    // printf("Alright1------\n");
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
    node->SymbolTableNode = malloc(sizeof(SymbolTable));

    if (lookUpST(val, t) != NULL) // val already exists
        return NULL;
    // printf("Here------\n");
    int hash = getHash(val, 41);
    if (tag == 0) // variable
    {
        node->tag = 0;
        node->val = val;
    }
    else // block
    {
        node->tag = 1;
        node->val = val;
        node->SymbolTableNode->block.scope = initialize_table(node, t); //data
        // printf("Here1------\n");
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
    // printf("Here-- %s----%d\n", val, t->size);
    Node *temp = t->nodes[hash];
    // if (temp != NULL)
    //     printf("Here------\n");
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
                if (strcmp(temp->val, val) == 0)
                {
                    return temp; // val already exists
                }
            }
        }
    }

    return NULL; // new val found
}

Table *baseTb; // base table

Table *populateSymbolTable(astNode *root)
{

    baseTb = initialize_table(NULL, NULL); //creates the base table

    populateTableRecur(baseTb, root);

    return baseTb;
}

void populateTableRecur(Table *tb, astNode *root)
{
    //preorder traversal
    if (root == NULL)
        return;
    // printf("Hello\n");
    // Node *check;
    // if (root->n_Name == module_ast)
    //     check = lookUpST(root->child_list->head->tokenInfo->lexeme, baseTb);
    // if (!(root->n_Name == module_ast && check != NULL))
    {
        tb = processNode(tb, root);
    }
    // else
    // {
    //     printf("Exceptional  %s\n", root->child_list->head->tokenInfo->lexeme);
    //     // tb = (lookUpST(root->child_list->head->tokenInfo->lexeme, baseTb))->SymbolTableNode->block.scope;
    // }

    Children *chi = root->child_list;

    astNode *temp;
    if (chi)
    {
        temp = chi->head;

        // if ((root->n_Name == module_ast && lookUpST(root->child_list->head->tokenInfo->lexeme, baseTb) != NULL))
        // {
        //     // if (tb == baseTb)
        //     //     printf("Hello1\n");
        //     temp = chi->head->sibling;
        //     // tb = (lookUpST(root->child_list->head->tokenInfo->lexeme, baseTb))->SymbolTableNode->block.scope;
        //     printf("I'm Here\n");
        // }
        if (root->n_Name == module_ast)
        // if (root->n_Name == module_ast && tb == baseTb)
        {
            printf("Hello1****%s\n", root->child_list->head->tokenInfo->lexeme);
            temp = root->sibling;
        }
        while (temp)
        {
            populateTableRecur(tb, temp);
            temp = temp->sibling;
        }
    }
}

Table *processNode(Table *tb, astNode *node)
{
    //check for module declarations
    if (node->n_Name == moduleDec_ast)
    {
        printf("Hello----------1\n");
        Children *chi = node->child_list;
        astNode *temp = chi->head;

        Table *t1 = insertST(temp->tokenInfo->lexeme, tb, 1);

        if (t1 == NULL)
        {
            printf("ERROR: Module Declaration already exists\n");
        }
        Node *n = lookUpST(temp->tokenInfo->lexeme, tb);
        n->SymbolTableNode->block.decl_required = 0;
        n->SymbolTableNode->block.defined = 0;
    }
    else if (node->n_Name == module_ast)
    {
        printf("Hello----------2\n");
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
                processModule(t1, node);
            }
            else // If same name module or ID already exists
            {
                printf("Hello---%%%%%%%%%%%%%%%%%%%%\n");
                Table *t1 = check->SymbolTableNode->block.scope;
                temp = chi->head;
                // Node *n = lookUpST(chi->head->tokenInfo->lexeme, tb);
                check->SymbolTableNode->block.decl_required = 0;
                check->SymbolTableNode->block.defined = 1;
                processModule(t1, node);
            }
        }
        else
        {
            printf("Hello---$$$$$$$$$$$$$$$$\n");
            Children *chi = node->child_list;
            astNode *temp;
            Node *check = lookUpST(chi->head->tokenInfo->lexeme, tb);
            if (check == NULL)
            {
                printf("ERROR: Module %s not declared before the Driver Module\n", chi->head->tokenInfo->lexeme);
                Table *t1 = insertST(chi->head->tokenInfo->lexeme, tb, 1);
                // temp = chi->head->sibling;
                // printf("%s\n", temp->child_list->head->tokenInfo->lexeme);
                Node *n = lookUpST(chi->head->tokenInfo->lexeme, tb);
                n->SymbolTableNode->block.decl_required = 1;
                n->SymbolTableNode->block.defined = -1;
                processModule(t1, node);
            }
            else
            {
                printf("Hello---****************\n");
                Table *t1 = check->SymbolTableNode->block.scope;
                temp = chi->head->sibling;

                check->SymbolTableNode->block.decl_required = 0;
                check->SymbolTableNode->block.defined = 1;
                processModule(t1, node);
            }
        }
        return tb;
    }
    //check for module use
    else if (node->n_Name == moduleReuseStmt_ast)
    {
        printf("Hello----------3\n");
        Table *parent_table = tb;

        astNode *ast = node->child_list->head->sibling;
        // printf("Hello---\n");
        while (parent_table->container != NULL)
        {
            //lookUpST
            parent_table = parent_table->container;
        }
        Node *check = lookUpST(ast->tokenInfo->lexeme, parent_table);
        if (check == NULL)
        {
            printf("ERROR: Module %s in line no. %d has not been declared\n", ast->tokenInfo->lexeme, ast->tokenInfo->lineno);
        }
        else if (check->SymbolTableNode->block.decl_required == 0)
        {
            if (check->SymbolTableNode->block.defined == 0)
                check->SymbolTableNode->block.decl_required = 1;
            else if (check->SymbolTableNode->block.defined == 1)
                printf("ERROR: Redundant declaration of module '%s' in line No. %d\n", ast->tokenInfo->lexeme, ast->tokenInfo->lineno);
        }

        int recursion = checkRecursion(node, tb);
        if (recursion)
        {
            printf("ERROR: Recursive Function call '%s()' encountered in line no. %d\n", ast->tokenInfo->lexeme, ast->tokenInfo->lineno);
        }
    }

    //check for variable(ID) declarations
    else if (node->n_Name == declareStmt_ast)
    {
        printf("Hello----------4\n");
        Children *chi = node->child_list;
        char *dType = chi->head->sibling->child_list->head->tokenInfo->lexeme;
        printf("***%s***\n", dType);
        astNode *temp = chi->head->child_list->head;
        // printf("lelo----------%s\n", temp->tokenInfo->lexeme);
        while (temp)
        {
            tb = insertST(temp->tokenInfo->lexeme, tb, 0);

            if (tb == NULL)
            {
                printf("ERROR: Multiple declaration for variable '%s' in the same scope\n", temp->tokenInfo->lexeme);
                break;
            }
            Node *node = lookUpST(temp->tokenInfo->lexeme, tb);

            node->SymbolTableNode->variable.addr = dType;
            printf("Hello------\n");
            temp = temp->sibling;
        }
        return tb;
    }
    else if (node->n_Name == input_plist_ast) // module input parameter list
    {
        printf("Hello----------5inp\n");
        astNode *temp = node->child_list->head;

        while (temp)
        {
            printf("%s\n", temp->tokenInfo->lexeme);
            tb = insertST(temp->tokenInfo->lexeme, tb, 0);

            if (tb == NULL)
            {
                printf("ERROR: Multiple declaration for variable '%s' in the same scope\n", temp->tokenInfo->lexeme);
            }
            Node *node = lookUpST(temp->tokenInfo->lexeme, tb);
            temp = temp->sibling; //Now temp denotes "dataType_ast"
            astNode *temp_child = temp->child_list->head;
            if (temp_child->tokenInfo == NULL) //temp_child denotes "range_array_ast"
            {
                node->SymbolTableNode->variable.addr = temp_child->sibling->tokenInfo->lexeme;
                node->SymbolTableNode->variable.var_tag = 1;
                astNode *grand_chi = temp_child->child_list->head;
                node->SymbolTableNode->variable.lower_index = grand_chi->tokenInfo->val->intValue;
                node->SymbolTableNode->variable.upper_index = grand_chi->sibling->tokenInfo->val->intValue;
            }
            else
            {
                node->SymbolTableNode->variable.addr = temp_child->tokenInfo->lexeme;
            }
            temp = temp->sibling;
        }
    }
    else if (node->n_Name == output_plist_ast) //Inside module
    {
        printf("Hello----------5op\n");
        astNode *temp = node->child_list->head;

        while (temp)
        {
            printf("%s\n", temp->tokenInfo->lexeme);
            tb = insertST(temp->tokenInfo->lexeme, tb, 0);

            if (tb == NULL)
            {
                printf("ERROR: Multiple declaration for variable '%s' in the same scope\n", temp->tokenInfo->lexeme);
            }
            Node *node = lookUpST(temp->tokenInfo->lexeme, tb);
            temp = temp->sibling;
            node->SymbolTableNode->variable.addr = temp->tokenInfo->lexeme;
            temp = temp->sibling;
        }
    }
    //check for variable use
    else if (node->tokenInfo != NULL && node->tokenInfo->t == ID && node->parent->parent->n_Name != declareStmt_ast && node->parent->n_Name != moduleDec_ast && node->parent->n_Name != moduleReuseStmt_ast && node->parent->n_Name != input_plist_ast && node->parent->n_Name != output_plist_ast)
    {
        printf("Hello----------6\n");
        Table *parent_table = tb;
        Node *check;
        while (parent_table != NULL)
        {
            // printf("Here\n");
            check = lookUpST(node->tokenInfo->lexeme, parent_table);
            if (check != NULL)
            {
                break;
            }
            parent_table = parent_table->container;
        }
        if (check == NULL)
            printf("ERROR: Variable '%s' not declared in line no. %d\n", node->tokenInfo->lexeme, node->tokenInfo->lineno);
    }
    else if (node->n_Name == driverModule_ast)
    {
        driver_traversed = 1;
    }
    else if (node->n_Name == itr_for_ast)
    {
        printf("Hello----------7\n");

        // char arr[20];
        // itoa(num_for, arr, 10);
        char *arr = itoa(num_for);
        Table *t1 = insertST(strcat("for", arr), tb, 1);
        num_for++; //increment the number of for loop encountered so far

        Children *chi = node->child_list;
        astNode *temp = chi->head;

        // printf("Hello---%%%%%%%%%%%%%%%%%%%%\n");
        Node *check = lookUpST(temp->tokenInfo->lexeme, tb);
        if (check == NULL)
        {
            printf("ERROR: Iterating variable '%s' not declared in line no. %d\n", temp->tokenInfo->lexeme, temp->tokenInfo->lineno);
        }
        t1 = insertST(temp->tokenInfo->lexeme, t1, 0);
        temp = temp->sibling; //temp points to range_ast
        Node *node = lookUpST(temp->tokenInfo->lexeme, t1);
        node->SymbolTableNode->variable.lower_index = temp->child_list->head->tokenInfo->val->intValue;
        node->SymbolTableNode->variable.upper_index = temp->child_list->head->sibling->tokenInfo->val->intValue;

        processModule(t1, temp->sibling);
    }
    else if (node->n_Name == itr_while_ast)
    {
        printf("Hello----------8\n");

        // char arr[20];
        // itoa(num_while, arr, 10);
        char *arr = itoa(num_while);
        Table *t1 = insertST(strcat("while", arr), tb, 1);
        num_while++; //increment the number of while loop encountered so far

        // Children *chi = node->child_list;
        // astNode *temp = chi->head;

        // printf("Hello---%%%%%%%%%%%%%%%%%%%%\n");
        // Node *check = lookUpST(temp->tokenInfo->lexeme, tb);
        // if (check == NULL)
        // {
        //     printf("ERROR: Iterating variable '%s' not declared in line no. %d\n", temp->tokenInfo->lexeme, temp->tokenInfo->lineno);
        // }
        // t1 = insertST(temp->tokenInfo->lexeme, t1, 0);

        // temp = temp->sibling; //temp points to range_ast
        // Node *node = lookUpST(temp->tokenInfo->lexeme, t1);
        // node->SymbolTableNode->variable.lower_index = temp->child_list->head->tokenInfo->val->intValue;
        // node->SymbolTableNode->variable.upper_index = temp->child_list->head->sibling->tokenInfo->val->intValue;

        processModule(t1, node->child_list->head);
    }
    else if (node->n_Name == conditionalStmt_ast)
    {
        printf("Hello----------9\n");

        // char arr[20];
        // itoa(num_switch, arr, 10);
        char *arr = itoa(num_switch);
        Table *t1 = insertST(strcat("switch", arr), tb, 1);
        num_switch++; //increment the number of for loop encountered so far

        Children *chi = node->child_list;
        astNode *temp = chi->head;

        Table *parent_table = tb;
        Node *check;
        while (parent_table != NULL)
        {
            // printf("Here\n");
            check = lookUpST(node->tokenInfo->lexeme, parent_table);
            if (check != NULL)
            {
                break;
            }
            parent_table = parent_table->container;
        }
        if () //check for INteger or boolean
            if (check == NULL)
                printf("ERROR: Condition Variable '%s' not declared in line no. %d\n", temp->tokenInfo->lexeme, temp->tokenInfo->lineno);
        // printf("Hello---%%%%%%%%%%%%%%%%%%%%\n");
        // Node *check = lookUpST(temp->tokenInfo->lexeme, tb);
        // if (check == NULL)
        // {
        //     printf("ERROR: Condition variable '%s' not declared in line no. %d\n", temp->tokenInfo->lexeme, temp->tokenInfo->lineno);
        // }
        // t1 = insertST(temp->tokenInfo->lexeme, t1, 0);
        // temp = temp->sibling; //temp points to range_ast
        // Node *node = lookUpST(temp->tokenInfo->lexeme, t1);
        // node->SymbolTableNode->variable.lower_index = temp->child_list->head->tokenInfo->val->intValue;
        // node->SymbolTableNode->variable.upper_index = temp->child_list->head->sibling->tokenInfo->val->intValue;

        // processModule(t1, node);
    }
    else
    {
        printf("Hello from else\n");
        // printf("%d\n", node->n_Name);
        return tb;
    }
    return tb;
}

void processModule(Table *tb, astNode *node)
{
    if (node == NULL)
        return;

    if (!(node->n_Name == module_ast && lookUpST(node->child_list->head->tokenInfo->lexeme, baseTb) != NULL) || !(node->n_Name == itr_for_ast) || !(node->n_Name == itr_while_ast))
    {
        // printf("Hello from modules populate\n");
        tb = processNode(tb, node);
    }
    // else
    // {
    //     tb = processNode(tb, node);
    // }

    Children *chi = node->child_list;

    astNode *temp;
    if (chi)
    {
        temp = chi->head;

        if ((node->n_Name == module_ast && lookUpST(node->child_list->head->tokenInfo->lexeme, baseTb) != NULL) || (node->n_Name == itr_for_ast) || (node->n_Name == itr_while_ast))
        {
            temp = chi->head->sibling;
            printf("I'm Here\n");
        }
        while (temp)
        {
            processModule(tb, temp);
            temp = temp->sibling;
        }
    }
}

void printSymbolTable(Table *tb)
{
}

int checkRecursion(astNode *node, Table *tb)
{
    if (tb->parent == NULL)
        return 0;
    else if (strcmp(tb->parent->val, node->child_list->head->sibling->tokenInfo->lexeme) == 0)
        return 1;

    return 0;
}