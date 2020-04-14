/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "intermediateCode.h"

int num_temp_reg = 0;
extern int global_offset;

char *generate_reg_name()
{
    num_temp_reg++;
    char *str = malloc(sizeof(char) * 15);
    sprintf(str, "t%d", num_temp_reg);

    return str;
}

void startCode(FILE *fp)
{
    fprintf(fp, "section .text\n%s%s%s",
            "\tglobal \t driver\n",
            "driver:\n",
            "\t sub \trsp,8\n");
}

void endCode(FILE *fp)
{
    fprintf(fp, "\tadd\t rsp,8\n%s%s%s",
            "\tmov \t ebx, 0\n",
            "\tmov \t eax, 1\n",
            "int \t 80h");
}

void generate_assembly_code(astNode *root, char *asm_file)
{
    FILE *fp = fopen("code.asm", "w");
    printf("Here\n");
    fprintf(fp, "%cinclude 'functions.asm'\n", '%');

    //initialize data segment of assembly code
    fprintf(fp, "section .data\n %s%s%s%s%s%s%s%s%s",
            "\tinputMsg \t db\t 'Input: Enter an ', 0h\n",
            "\tintMsg \t db \t 'integer ', 0h\n",
            "\trealMsg \t db \t 'real ', 0h\n",
            "\tboolMsg \t db \t 'boolean ', 0h\n",
            "\tvalMsg \t db \t 'value ', 0h\n",
            "\tarrMsg \t db \t 'array elements of ', 0h\n",
            "\toutMsg \t db \t 'Output: ', 0h\n",
            "\ttrueMsg \t db \t 'true', 0h\n",
            "\tfalseMsg \t db \t 'false', 0h\n");

    //uninitialized data segment
    fprintf(fp, "section .bss\n%s",
            "\tsinput: \t resb \t 255\n");

    //text segment
    printf("Here\n");
    generate_asm_ast(root, fp);
    printf("Here\n");
    fclose(fp);
}

void generate_asm_ast(astNode *root, FILE *fp)
{

    astNode *temp = root->child_list->head;
    while (temp->n_Name != driverModule_ast)
    {
        temp = temp->sibling;
    }
    fprintf(fp, "section .text\n");
    traverseDriverModule(temp->child_list->head, fp);
}

void traverseDriverModule(astNode *node, FILE *fp)
{
    if (node == NULL)
        return;

    if (node->n_Name == ioStmt_get_value_ist)
    {
        processIO_getValue_Stmt(node, fp);
    }

    else if (node->n_Name == ioStmt_print_ast)
    {
        processIO_print_Stmt(node, fp);
    }

    else if (node->n_Name == assignmentStmt_ast)
    {
        processAssignmentStmt_asm(node, fp);
    }

    // else if (node->n_Name == moduleReuseStmt_ast)
    // {
    //     processModuleReUseStmt(node, fp);
    // }

    // else if (node->n_Name == itr_for_ast)
    // {
    //     processForLoop(node, fp);
    // }

    // else if (node->n_Name == itr_while_ast)
    // {
    //     processWhileLoop(node, fp);
    // }

    // else if (node->n_Name == conditionalStmt_ast)
    // {
    //     processSwitchStmt(node, fp);
    // }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            traverseDriverModule(temp, fp);
            temp = temp->sibling;
        }
    }
}

void processIO_getValue_Stmt(astNode *node, FILE *fp)
{

    astNode *temp = node->child_list->head;
    Node *n = lookUpST(temp->tokenInfo->lexeme, node->tb);
    if (n->SymbolTableNode->variable.var_tag == 1) //array variable
    {
    }
    else //normal identifier of int, real or boolean
    {
        fprintf(fp, "\tmov \t eax, inputMsg\n%s",
                "\t call \t sprint\n");
        if (strcmp(n->SymbolTableNode->variable.addr, "integer") == 0)
        {
            fprintf(fp, "\tmov \t eax, intMsg\n%s",
                    "\t call \t sprint\n");
        }
        else if (strcmp(n->SymbolTableNode->variable.addr, "real") == 0)
        {
            fprintf(fp, "\tmov \t eax, realMsg\n%s",
                    "\t call \t sprint\n");
        }
        else if (strcmp(n->SymbolTableNode->variable.addr, "boolean") == 0)
        {
            fprintf(fp, "\tmov \t eax, boolMsg\n%s",
                    "\t call \t sprint\n");
        }
        fprintf(fp, "\tmov \t eax, valMsg\n%s",
                "\t call \t sprintLF\n");

        fprintf(fp, "\t mov \t edx, 255 \t ;number of bytes to read\n");
        fprintf(fp, "\t mov \t ecx, sinput \t ;buffer\n");
        fprintf(fp, "\t mov \t ebx, 0 \t ;write to the STDIN file\n");
        fprintf(fp, "\t mov \t eax, 3 \t ;invoke SYS_READ\n");
        fprintf(fp, "\t int \t 80h\n");

        fprintf(fp, "\tmov \t [ebp+%d], sinput\n", n->SymbolTableNode->variable.offset);
    }
}

void processIO_print_Stmt(astNode *node, FILE *fp) // check whether whole array can also be printed
{
    fprintf(fp, "\tmov \t eax, inputMsg\n%s",
            "\t call \t sprint\n");
    if (node->child_list->head->n_Name == var_id_num_ast)
    {
        Node *n = lookUpST(node->child_list->head->tokenInfo->lexeme, node->tb);
        if (n->SymbolTableNode->variable.var_tag == 1) //array variable
        {
        }
        else
        {
            int offset = n->SymbolTableNode->variable.offset;
            fprintf(fp, "\tmov \t eax, [ebp+%d]\n", offset);
            fprintf(fp, "\tcall iprintLF\n");
        }
    }
    else if (node->child_list->head->tokenInfo->t == TRUE)
    {
    }
    else if (node->child_list->head->tokenInfo->t == FALSE)
    {
    }
    else if (node->child_list->head->tokenInfo->t == NUM)
    {
    }
    else if (node->child_list->head->tokenInfo->t == RNUM)
    {
    }
}

void processAssignmentStmt_asm(astNode *node, FILE *fp)
{
    astNode *lhs = node->child_list->head;
    astNode *rhs = node->child_list->head->sibling->child_list->head->sibling;

    evaluateAssignmentStmt(rhs, fp);
    Node *n = lookUpST(lhs->tokenInfo->lexeme, node->tb);
    fprintf(fp, "\tmov \t [ebp+%d], eax\n", n->SymbolTableNode->variable.offset);
}

void evaluateAssignmentStmt(astNode *node, FILE *fp)
{
    if (node->n_Name == plus_ast || node->n_Name == minus_ast || node->n_Name == mul_ast || node->n_Name == div_ast)
    {
        astNode *left = node->child_list->head;
        astNode *right = left->sibling;

        evaluateAssignmentStmt(left, fp);

        // t1 <- ax
        char *reg = generate_reg_name();
        tempReg *tempreg = initialize_tempreg(reg);
        fprintf(fp, "\tmov \t [ebp+%d], eax\n", tempreg->offset);

        evaluateAssignmentStmt(right, fp);

        if (node->n_Name == plus_ast)
        {
            fprintf(fp, "\tadd \t eax, [ebp+%d]\n", tempreg->offset);
        }

        else if (node->n_Name == minus_ast)
        {
            fprintf(fp, "\tsub \t eax, [ebp+%d]\n", tempreg->offset);
        }

        else if (node->n_Name == mul_ast)
        {
            fprintf(fp, "\tmul \t[ebp+%d]\n", tempreg->offset);
        }

        else if (node->n_Name == div_ast)
        {
            fprintf(fp, "div \t [ebp+%d]\n", tempreg->offset);
        }
    }

    else if (node->n_Name == var_id_num_ast)
    {
        Node *n = lookUpST(node->child_list->head->tokenInfo->lexeme, node->tb);
        if (n)
        {
            fprintf(fp, "\tmov \t eax, [ebp+%d]\n", n->SymbolTableNode->variable.offset);
        }
    }

    else if (node->tokenInfo && node->tokenInfo->t == NUM)
    {
        fprintf(fp, "\t mov \t eax, %d\n", node->tokenInfo->val->intValue);
    }

    else if (node->tokenInfo && node->tokenInfo->t == RNUM)
    {
        fprintf(fp, "\tmov \t eax, %f\n", node->tokenInfo->val->fValue);
    }
}

tempReg *initialize_tempreg(char *reg)
{
    tempReg *temp = malloc(sizeof(tempReg));
    temp->reg = reg;
    temp->offset = global_offset;

    global_offset++;

    return temp;
}