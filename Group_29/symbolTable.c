/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "symbolTable.h"

int driver_traversed = 0;

Table *initialize_table(Node *parent, Table *container)
{
    Table *t = malloc(sizeof(Table));
    t->nodes = malloc(sizeof(Node *) * 41);
    t->num = 41;
    for (int i = 0; i < t->num; i++)
        t->nodes[i] = NULL;
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
    //add data

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
        node->SymbolTableNode = malloc(sizeof(SymbolTable));
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
    printf("Here-- %s----%d\n", val, t->size);
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

    return NULL;
    // new val found
}

Table *populateSymbolTable(astNode *root)
{

    Table *tb = initialize_table(NULL, NULL); //creates the base table

    populateTableRecur(tb, root);

    return tb;
}

void populateTableRecur(Table *tb, astNode *root)
{
    //preorder traversal
    if (root == NULL)
        return;
    // printf("Hello\n");
    tb = processNode(tb, root);
    // printf("Hello1\n");

    Children *chi = root->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
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
    // printf("Hello----------\n");
    if (node->n_Name == moduleDec_ast)
    {
        printf("Hello----------1\n");
        Children *chi = node->child_list;
        astNode *temp = chi->head;
        while (temp)
        {
            tb = insertST(temp->tokenInfo->lexeme, tb, 1);
            if (tb == NULL)
            {
                printf("ERROR: Module Declaration already exists\n");
            }
            temp = temp->sibling;
        }
    }
    else if (node->n_Name == module_ast)
    {
        printf("Hello----------2\n");
        if (driver_traversed == 0)
        {
            Children *chi = node->child_list;
            astNode *temp;
            Node *check = lookUpST(chi->head->tokenInfo->lexeme, tb);
            if (check == NULL)
            {
                Table *t1 = insertST(chi->head->tokenInfo->lexeme, tb, 1);
                temp = chi->head->sibling;
                // if (chi)
                // {
                //     processNode(t1, temp);
                //     temp = chi->head;
                //     while (temp)
                //     {
                //         processNode(t1, temp);
                //         temp = temp->sibling;
                //     }

                // }

                printf("%s\n", temp->child_list->head->tokenInfo->lexeme);
                populateTableRecur(t1, temp);
            }
            else
            {
                tb = check->SymbolTableNode->block.scope;
                temp = chi->head;
                while (temp)
                {
                    tb = processNode(tb, temp);
                    temp = temp->sibling;
                }
            }
        }
        else
        {
            Children *chi = node->child_list;
            astNode *temp;
            Node *check = lookUpST(chi->head->tokenInfo->lexeme, tb);
            if (check == NULL)
            {
                printf("ERROR: Module not declared initially\n");
            }
            else
            {
                tb = check->SymbolTableNode->block.scope;
                temp = chi->head;
                while (temp)
                {
                    tb = processNode(tb, temp);
                    temp = temp->sibling;
                }
            }
        }
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
            printf("ERROR: Module not declared\n");
        }
    }

    //check for variable(ID) declarations
    else if (node->n_Name == declareStmt_ast)
    {
        printf("Hello----------4\n");
        Children *chi = node->child_list;
        char *dType = chi->head->sibling->tokenInfo->lexeme;
        astNode *temp = chi->head->child_list->head;

        while (temp)
        {
            tb = insertST(temp->tokenInfo->lexeme, tb, 0);
            // printf("Hello------\n");
            if (tb == NULL)
            {
                printf("ERROR: Variable having same name already exists in this scope\n");
            }
            Node *node = lookUpST(temp->tokenInfo->lexeme, tb);
            node->SymbolTableNode->variable.addr = dType;
            temp = temp->sibling;
        }
        return tb;
    }
    else if (node->n_Name == output_plist_ast || node->n_Name == input_plist_ast)
    {
        printf("Hello----------5\n");
        astNode *temp = node->child_list->head;

        while (temp)
        {
            // printf("%s\n", temp->tokenInfo->lexeme);
            tb = insertST(temp->tokenInfo->lexeme, tb, 0);

            if (tb == NULL)
            {
                printf("ERROR: Variable having same name already exists in this scope\n");
            }
            Node *node = lookUpST(temp->tokenInfo->lexeme, tb);
            temp = temp->sibling;
            node->SymbolTableNode->variable.addr = temp->tokenInfo->lexeme;
            temp = temp->sibling;
        }
    }
    //check for variable use
    else if (node->tokenInfo != NULL && node->tokenInfo->t == ID && node->parent->parent->n_Name != declareStmt_ast && node->parent->n_Name != moduleReuseStmt_ast && node->parent->n_Name != input_plist_ast && node->parent->n_Name != output_plist_ast)
    {
        printf("Hello----------6\n");
        Table *parent_table = tb;
        // astNode *ast = node->child_list->head;

        while (parent_table != NULL)
        {
            //lookUpST
            // printf("Here\n");
            Node *check = lookUpST(node->tokenInfo->lexeme, parent_table);
            if (check == NULL)
            {
                printf("ERROR: Variable not declared\n");
            }
            else
            {
                break;
            }
            parent_table = parent_table->container;
        }
    }
    else
    {
        printf("Hello from else\n");
        return tb;
    }
    return tb;
}
