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
traverse_AST(astNode *node)
{
    if (node == NULL)
        return;

    if (node->n_Name == module_ast)
    {
        temp = lookUpST(node->child_list->head->tokenInfo->lexeme, baseTb);
    }
    if (node->n_Name == driverModule_ast)
    {
        temp = lookUpST("drivermodule", baseTb);
    }
    if (node->n_Name == itr_for_ast || node->n_Name == itr_while_ast || node->n_Name == conditionalStmt_ast)
    {
        Table *childTable = temp->SymbolTableNode->block.scope;
    }

    if (node->n_Name == moduleReuseStmt_ast)
    {
        if (node->child_list->head->n_Name == optional_ast)
        {
            astNode *output_list = node->child_list->head->child_list->head->child_list->head;
            char *moduleName = node->child_list->head->sibling->tokenInfo->lexeme;
            astNode *input_list = node->child_list->head->sibling->sibling->sibling->child_list->head;

            parameter_check(input_list, output_list, moduleName);
        }
    }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            traverse_AST(temp);
            temp = temp->sibling;
        }
    }
}

void parameter_check(astNode *input_list, astNode *output_list, char *val)
{
    //verify input parameter list with the arguments in the module defi
    int hash = getHash(val, baseTb->size);
    Node *temp = baseTb->nodes[hash];
    while (temp != NULL)
    {
        if (strcmp(temp->val, val) == 0)
            break;
        temp = temp->next;
    }

    //check if there are no variables to be returned then module reuse statement is invoked appropriately

    //verify output return type and number with the module definition in the symbol table

    //check if all the variables returned in the module definition are assigned some value or not
}