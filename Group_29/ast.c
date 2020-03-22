
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

    astNode *ast = pt->root;
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
    }
    processNode(node);
}

astNode *populateChild(TreeNode *node, astNode *newNode)
{
    int i = 0, size = node->t.internalnode.rhs_size;
    while (size--)
    {
        astNode *temp = node->t.internalnode.children[i]->addr;
    }
}

astNode *populateParent()
{
}

astNode *addSibling()
{
}

astNode *createNode(nodeName name, astNode *parent, astNode *sibling, Token *tokenInfo)
{
    astNode *temp = malloc(sizeof(astNode));
    temp->n_Name = name;
    temp->parent = parent;
    temp->sibling = sibling;
    temp->child_list = NULL;
    temp->tokenInfo = tokenInfo;

    return temp;
}

void processNode(TreeNode *node)
{
    int rule_no = node->rule_no;

    switch (rule_no)
    {
    case 0:
    {
        astNode *temp = createNode(program, NULL, NULL, NULL);
        populateChild(node, temp);
    }
    case 8:
    {
        node->addr = node->t.internalnode.children[2]->addr;
        break;
    }
    }
}