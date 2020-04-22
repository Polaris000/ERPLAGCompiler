/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "codegen.h"
#include "symbolTable.h"
extern Table *baseTb;
// Table *parentTableGlobal;

int num_temp_reg = 0;
int global_label = 0;
extern int global_offset;

char *generate_reg_name()
{
    num_temp_reg++;
    char *str = malloc(sizeof(char) * 15);
    sprintf(str, "t%d", num_temp_reg);

    return str;
}

void getarrlength(FILE *fp)
{
    fprintf(fp, "_getarrlength:\n%s%s%s%s%s%s%s%s",
            "\tpop ebp\n",
            "\tpop ebx\n",
            "\tpop eax\n",
            "\tsub ebx, eax\n",
            "\tinc ebx\n",
            "\tpush ebx\n",
            "\tpush ebp\n",
            "\tret\n");
}

void dynamicarrstruct(FILE *fp)
{
    fprintf(fp, "\tstruc DARRAY\n");
    fprintf(fp, "\t.ptr: resd 1\n");
    fprintf(fp, "\t.count: resd 1\n");
    fprintf(fp, "\tendstruc\n");
}

void startCode(FILE *fp)
{
    fprintf(fp, "section .text\n%s%s",
            "\tglobal  main\n",
            "\textern scanf, printf, malloc, realloc, free\n");
    getarrlength(fp);

    fprintf(fp, "main:\n%s%s",
            "\tmov ebp, esp\n",
            "\tsub esp, 8\n");
}

void endCode(FILE *fp)
{
    fprintf(fp, "\nEXIT:\n");
    fprintf(fp, " %s%s%s%s",
            "\tadd esp, 8\n",
            "\tmov ebx, 0\n",
            "\tmov eax, 1\n",
            "\tint 80h \n");
}

void printMsg(char *str, FILE *fp)
{
    fprintf(fp, "\tpush \t%s\n", str);
    fprintf(fp, "\tpush \tpercentSout\n");
    fprintf(fp, "\tcall \tprintf\n");
    fprintf(fp, "\tadd esp, 8\n");
}

void printDOut(char *str, FILE *fp)
{
    fprintf(fp, "\tpush \tdword [%s]\n", str);
    fprintf(fp, "\tpush \tpercentDout\n");
    fprintf(fp, "\tcall \tprintf\n");
    fprintf(fp, "\tadd esp, 8\n");
}

void printDoutarr(char *str, FILE *fp)
{
    fprintf(fp, "\tpush \tdword [%s]\n", str);
    fprintf(fp, "\tpush \tpercentDoutarr\n");
    fprintf(fp, "\tcall \tprintf\n");
    fprintf(fp, "\tadd esp, 8\n");
}

void getValue(char *str, FILE *fp)
{
    fprintf(fp, "\tpush \t%s\n", str);
    fprintf(fp, "\tpush \tpercentDin\n");
    fprintf(fp, "\tcall \tscanf\n");
    fprintf(fp, "\tadd esp, 8\n");
}

void populateUninitializedData(FILE *fp)
{
    fprintf(fp, "section .bss\n");

    populateSTVariables(baseTb, fp);
}

void populateSTVariables(Table *tb, FILE *fp)
{
    for (int i = 0; i < tb->num; i++)
    {
        Node *node = tb->nodes[i];
        while (node != NULL)
        {
            if (node->tag == 0) // for variable
            {
                printVariable(node, tb, tb->nesting_level, fp);
            }
            else // for block
            {
                if (node->SymbolTableNode->block.input_list)
                {
                    List *in_list = node->SymbolTableNode->block.input_list;

                    while (in_list)
                    {

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

                        printVariable(tempnode, node->SymbolTableNode->block.scope, node->SymbolTableNode->block.scope->nesting_level, fp);
                        in_list = in_list->next;
                        free(tempnode);
                    }
                }
                if (node->SymbolTableNode->block.output_list)
                {
                    List *out_list = node->SymbolTableNode->block.output_list;
                    while (out_list)
                    {
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

                        printVariable(tempnode, node->SymbolTableNode->block.scope, node->SymbolTableNode->block.scope->nesting_level, fp);
                        out_list = out_list->next;
                        free(tempnode);
                    }
                }

                populateSTVariables(node->SymbolTableNode->block.scope, fp);
            }
            node = node->next;
        }
    }
}

void printVariable(Node *node, Table *tb, int nesting_level, FILE *fp)
{

    char *str = malloc(sizeof(char) * 30);
    sprintf(str, "%s_%d_%d", node->val, tb->parent->SymbolTableNode->block.lower_scope, tb->parent->SymbolTableNode->block.upper_scope);

    //static arrays
    if (node->SymbolTableNode->variable.var_tag == 1 && node->SymbolTableNode->variable.lower_index->t == NUM && node->SymbolTableNode->variable.upper_index->t == NUM)
        fprintf(fp, "\t %s \t resd \t %d\n", str, (node->SymbolTableNode->variable.upper_index->val->intValue - node->SymbolTableNode->variable.lower_index->val->intValue) + 1);

    // dynamic arrays
    else if (node->SymbolTableNode->variable.var_tag == 1)
    {
        fprintf(fp, "\t%s \t resb \t DARRAY_size\n", str);
        // fprintf(fp, "index_%d_%d \t resd \t 1\n", tb->parent->SymbolTableNode->block.lower_scope, tb->parent->SymbolTableNode->block.upper_scope);
    }

    else
        fprintf(fp, "\t %s \t resd \t 1\n", str);
}

void generate_assembly_code(astNode *root, char *asm_file)
{
    FILE *fp = fopen(asm_file, "w");
    // printf("Here\n");
    dynamicarrstruct(fp);
    fprintf(fp, "section .data\n%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
            "\tprintmessage: db \" The number is\", 0\n ",
            "\tscanmessage: db \"input: Enter the number \", 0\n",
            "\tintegerMsg: db \"Input: Enter an integer value \", 0\n",
            "\tbooleanMsg: db \"Input: Enter a boolean value \", 0\n",
            "\tintMsg: db \"integer\", 0\n",
            "\ttrueMsg: db \"true\", 0\n",
            "\tfalseMsg: db \"false\", 0\n",
            "\tboolMsg: db \"boolean\", 0\n",
            "\toutput: db \"Output:\", 0\n",
            "\tpercentDin: db \"%d\", 0\n",
            "\tpercentDout: db \"%d\", 10, 0\n",
            "\tpercentDoutarr: db \"%d \", 0\n",
            "\tpercentS: db \"%s\", 0\n",
            "\tpercentSout: db \"%s\", 10, 0\n",
            "\tOutofBoundError: db \"RUN TIME ERROR in Line %d:  Array index out of bound\", 10, 0\n",
            "\tBoundMismatchError: db \"RUN TIME ERROR in Line %d:  Bounds do not match\", 10, 0\n",
            "\tarrayInputString: db \"Input: Enter %d array elements of %s type for range %d to %d\", 0\n");

    //uninitialized data segment

    populateUninitializedData(fp);

    //text segment
    startCode(fp);

    generate_asm_ast(root, fp);

    endCode(fp);
    // printf("Comes Here\n");
    defineAllModules(root, fp);

    fclose(fp);
    // printf("Finished\n");
}

int get_next_label()
{
    return global_label++;
}

void generate_asm_ast(astNode *root, FILE *fp)
{

    astNode *temp = root->child_list->head;
    while (temp->n_Name != driverModule_ast)
        temp = temp->sibling;

    traverseDriverModule(temp->child_list->head, fp);
}

void traverseDriverModule(astNode *node, FILE *fp)
{
    if (node == NULL)
        return;

    if (node->n_Name == ioStmt_get_value_ist && node->parent->n_Name != itr_while_ast && node->parent->n_Name != itr_for_ast)
    {
        // printf("Here -- getvalue--driver\n");
        processIO_getValue_Stmt(node, fp);
    }

    else if (node->n_Name == ioStmt_print_ast && node->parent->n_Name != itr_while_ast && node->parent->n_Name != itr_for_ast)
    {
        // printf("Here -- print--driver\n");
        processIO_print_Stmt(node, fp);
    }

    else if (node->n_Name == assignmentStmt_ast && node->parent->n_Name != itr_while_ast && node->parent->n_Name != itr_for_ast)
    {
        // printf("Here -- assign--driver\n");
        processAssignmentStmt_asm(node, fp);
    }

    else if (node->n_Name == moduleReuseStmt_ast)
    {
        // printf("Here -- modulecall--driver\n");
        astNode *temp = node->child_list->head;
        astNode *moduleName;
        if (temp->n_Name == optional_ast) //module returns variables
        {
            astNode *outputList = temp->child_list->head->child_list->head;
            moduleName = temp->sibling;
            astNode *inputList = temp->sibling->sibling->child_list->head;
            processModuleReUseStmt(node, inputList, outputList, moduleName, fp);
        }

        else //function which doesn't return anything
        {
            moduleName = temp->sibling;
            astNode *inputList = temp->sibling->sibling->child_list->head;
            processModuleReUseStmt(node, inputList, NULL, moduleName, fp);
        }
    }

    else if (node->n_Name == itr_for_ast && node->parent->n_Name != itr_while_ast && node->parent->n_Name != itr_for_ast)
    {
        // printf("Here -- for--driver\n");
        processForLoop(node, fp);
    }

    else if (node->n_Name == itr_while_ast && node->parent->n_Name != itr_while_ast && node->parent->n_Name != itr_for_ast)
    {
        // printf("Here -- while--driver\n");
        processWhileLoop(node, fp);
    }

    else if (node->n_Name == conditionalStmt_ast && node->parent->n_Name != itr_while_ast && node->parent->n_Name != itr_for_ast)
    {
        // printf("Here -- switch--driver\n");
        processSwitchStmt(node, fp);
    }

    else if (node->n_Name == declareStmt_ast)
    {

        astNode *dataType = node->child_list->head->sibling;
        if (dataType->child_list->head->tokenInfo == NULL) //arrays
        {
            // printf("Hello---\n");
            astNode *range_array = dataType->child_list->head;
            // printf("Hello---4\n");
            astNode *index = range_array->child_list->head;
            astNode *idList = node->child_list->head->child_list->head;

            if (index->child_list->head->tokenInfo->t != NUM || index->sibling->child_list->head->tokenInfo->t != NUM) //dynamic array
            {

                while (idList)
                {
                    // printf("Hello---5----%s\n", idList->tokenInfo->lexeme);
                    templookup *node_t = lookupIDCodegen(node->tb, idList->tokenInfo->lexeme);
                    Node *n = node_t->n;
                    Table *parentTable = node_t->t;

                    // printf("Hello---6\n");
                    dynamicArrayHandle(node_t, node->tb, fp);
                    // printf("Hello---7\n");
                    // get length from stack
                    fprintf(fp, "\tpop eax\n");
                    fprintf(fp, "\tmov dword[%s_%d_%d + DARRAY.count], eax\n", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
                    fprintf(fp, "\tsal eax, 2\n");
                    fprintf(fp, "\tpush eax\n");
                    fprintf(fp, "\tcall malloc\n");
                    fprintf(fp, "\tadd esp, 4\n");
                    fprintf(fp, "\tmov dword [%s_%d_%d + DARRAY.ptr], eax\n", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    idList = idList->sibling;
                }
                // Node *n = lookUpST(node->child_list->head->child_list->head->tokenInfo->lexeme, node->tb);
            }
        }
    }

    Children *chi = node->child_list;
    astNode *temp;

    if (chi)
    {
        temp = chi->head;
        if (node->n_Name == conditionalStmt_ast)
            temp = NULL;

        while (temp)
        {
            traverseDriverModule(temp, fp);
            temp = temp->sibling;
        }
    }
}

void processIO_getValue_Stmt(astNode *node, FILE *fp)
{
    astNode *temp = node->child_list->head; //variablename

    templookup *node_t = lookupIDCodegen(node->tb, temp->tokenInfo->lexeme);
    Node *n = node_t->n;
    Table *parentTable = node_t->t;

    // staticarray variable
    if (n->SymbolTableNode->variable.var_tag == 1 && n->SymbolTableNode->variable.lower_index->t == NUM && n->SymbolTableNode->variable.upper_index->t == NUM)
    {
        int lower = n->SymbolTableNode->variable.lower_index->val->intValue;
        int upper = n->SymbolTableNode->variable.upper_index->val->intValue;

        fprintf(fp, "\tmov eax, %d\n", lower);
        fprintf(fp, "\tpush eax\n");
        fprintf(fp, "\tmov edi, %d\n", upper);
        fprintf(fp, "\tpush edi\n");
        fprintf(fp, "\tcall _getarrlength\n");
        fprintf(fp, "\tpop esi\n");
        fprintf(fp, "\tpush edi\n");
        fprintf(fp, "\tpush eax\n");
        fprintf(fp, "\tpush intMsg\n");
        fprintf(fp, "\tpush esi\n");
        fprintf(fp, "\tpush arrayInputString\n");
        fprintf(fp, "\tcall printf\n");
        fprintf(fp, "\tmov edi, 0\n");

        int label = get_next_label();
        fprintf(fp, "\nscan_%d:\n", label);
        char *str = malloc(sizeof(char) * 20);

        sprintf(str, "%s_%d_%d", temp->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
        fprintf(fp, "\tmov ebx, %s\n", str);

        fprintf(fp, "\tmov eax, 4\n");
        fprintf(fp, "\tmul edi\n");
        fprintf(fp, "\tadd ebx, eax\n");
        getValue("ebx", fp);
        fprintf(fp, "\tinc edi\n");
        fprintf(fp, "\tcmp edi, esi\n");
        fprintf(fp, "\tjnz scan_%d\n", label);
    }

    //dynamic array variable
    else if (n->SymbolTableNode->variable.var_tag == 1)
    {

        dynamicArrayHandle(node_t, node->tb, fp);

        fprintf(fp, "\tpop esi\n");
        fprintf(fp, "\tpush edi\n");
        fprintf(fp, "\tpush eax\n");
        fprintf(fp, "\tpush intMsg\n");

        fprintf(fp, "\tpush esi\n");
        fprintf(fp, "\tpush arrayInputString\n");
        fprintf(fp, "\tcall printf\n");

        fprintf(fp, "\tmov edi, 0\n");
        int label = get_next_label();
        fprintf(fp, "\nscan_%d:\n", label);
        char *str = malloc(sizeof(char) * 20);

        sprintf(str, "%s_%d_%d", temp->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
        fprintf(fp, "\tmov ebx, dword[%s + DARRAY.ptr]\n", str);

        fprintf(fp, "\tmov eax, 4\n");
        fprintf(fp, "\tmul edi\n");
        fprintf(fp, "\tadd ebx, eax\n");
        getValue("ebx", fp);
        fprintf(fp, "\tinc edi\n");
        fprintf(fp, "\tcmp edi, esi\n");
        fprintf(fp, "\tjne scan_%d\n", label);
    }

    else //normal identifier of int, real or boolean
    {
        if (strcmp(n->SymbolTableNode->variable.addr, "integer") == 0)
        {
            printMsg("integerMsg", fp); // print enter an integer

            char *str = malloc(sizeof(char) * 30);
            sprintf(str, "%s_%d_%d", temp->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
            getValue(str, fp);
        }

        else if (strcmp(n->SymbolTableNode->variable.addr, "boolean") == 0)
        {
            printMsg("booleanMsg", fp); // print enter an boolean

            char *str = malloc(sizeof(char) * 30);
            sprintf(str, "%s_%d_%d", temp->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
            getValue(str, fp);
        }

        else if (strcmp(n->SymbolTableNode->variable.addr, "real") == 0)
        {
            // not working on this...
        }
    }
}

void processIO_print_Stmt(astNode *node, FILE *fp) // check whether whole array can also be printed
{
    if (node->child_list->head->n_Name == var_id_num_ast)
    {
        astNode *temp = node->child_list->head;
        // printf("Here--- %s\n", temp->child_list->head->tokenInfo->lexeme);

        templookup *node_t = lookupIDCodegen(node->tb, temp->child_list->head->tokenInfo->lexeme);
        Node *n = node_t->n;
        Table *parentTable = node_t->t;

        // array variable
        if (n->SymbolTableNode->variable.var_tag == 1)
        {
            if (temp->child_list->head->sibling == NULL) //complete array
            {
                // static array
                if (n->SymbolTableNode->variable.lower_index->t == NUM && n->SymbolTableNode->variable.upper_index->t == NUM)
                {
                    int lower = n->SymbolTableNode->variable.lower_index->val->intValue;
                    int upper = n->SymbolTableNode->variable.upper_index->val->intValue;

                    fprintf(fp, "\tmov esi, %d\n", lower);
                    fprintf(fp, "\tpush esi\n");
                    fprintf(fp, "\tmov esi, %d\n", upper);
                    fprintf(fp, "\tpush esi\n");
                    fprintf(fp, "\tcall _getarrlength\n");
                    fprintf(fp, "\tpop esi\n");
                    fprintf(fp, "\tmov edi, 0\n");

                    int label = get_next_label();
                    fprintf(fp, "\nprint_%d:\n", label);
                    char *str = malloc(sizeof(char) * 20);
                    sprintf(str, "%s_%d_%d + 4 * edi", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    printDOut(str, fp);
                    fprintf(fp, "\tinc edi\n");
                    fprintf(fp, "\tcmp edi, esi\n");
                    fprintf(fp, "\tjnz print_%d\n", label);
                }

                // dynamic array
                else
                {
                    dynamicArrayHandle(node_t, node->tb, fp);

                    fprintf(fp, "\tpop esi\n"); //get length of array in esi
                    fprintf(fp, "\tmov edi, 0\n");
                    int label = get_next_label();
                    fprintf(fp, "\nprint_%d:\n", label);

                    fprintf(fp, "\tmov ebx, dword[%s_%d_%d + DARRAY.ptr]\n", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    fprintf(fp, "\tmov eax, 4\n");
                    fprintf(fp, "\tmul edi\n");
                    fprintf(fp, "\tadd ebx, eax\n");
                    printDOut("ebx", fp);
                    fprintf(fp, "\tinc edi\n");
                    fprintf(fp, "\tcmp edi, esi\n");
                    fprintf(fp, "\tjnz print_%d\n", label);
                }
            }
            // array element
            else
            {
                astNode *index = temp->child_list->head->sibling->child_list->head;
                // static
                if (n->SymbolTableNode->variable.lower_index->t == NUM && n->SymbolTableNode->variable.upper_index->t == NUM)
                {
                    // A[2]
                    if (index->tokenInfo->t == NUM)
                    {
                        fprintf(fp, "\tmov ebx, %d\n", n->SymbolTableNode->variable.lower_index->val->intValue);
                        fprintf(fp, "\tmov eax, %d\n", index->child_list->head->tokenInfo->val->intValue);
                        fprintf(fp, "\tsub eax, ebx\n");
                        fprintf(fp, "\tmov edi, eax\n");

                        // fprintf(fp, "\tpus dword[%s + 4 * edi]\n", str);
                        printMsg("output", fp);
                        char *str = malloc(sizeof(char) * 20);
                        sprintf(str, "dword[%s_%d_%d + 4 * edi]", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
                        printDOut(str, fp);
                    }
                    else //bound check-- A[i]
                    {

                        int start = DynamicArrayIndexHandle(node_t, node->tb, fp, index);

                        printMsg("output", fp);
                        char *str = malloc(sizeof(char) * 20);
                        sprintf(str, "dword[%s_%d_%d + 4 * edi]", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
                        printDOut(str, fp);

                        fprintf(fp, "\tjmp L_END_%d\n", start);

                        //Here give out of bound error
                        fprintf(fp, "\nL_%d:\n", start);

                        fprintf(fp, "\tmov edi, %d\n", node->child_list->head->tokenInfo->lineno);
                        fprintf(fp, "\tpush edi\n");
                        fprintf(fp, "\tpush OutofBoundError\n");
                        fprintf(fp, "\tcall printf\n");
                        fprintf(fp, "\tjmp EXIT\n");

                        fprintf(fp, "\nL_END_%d:\n", start);
                    }
                } //static array ends

                // dynamic
                else
                {
                    int start = DynamicArrayIndexHandle(node_t, node->tb, fp, index);
                    printMsg("output", fp);
                    fprintf(fp, "\tmov ebx, dword[%s_%d_%d + DARRAY.ptr]\n", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    // char *str = malloc(sizeof(char) * 20);

                    fprintf(fp, "\tmov eax, 4\n");
                    fprintf(fp, "\tmul edi\n");
                    fprintf(fp, "\tadd ebx, eax\n");
                    printDOut("ebx", fp);

                    fprintf(fp, "\tjmp L_END_%d\n", start);

                    //Here give out of bound error
                    fprintf(fp, "\nL_%d:\n", start);

                    fprintf(fp, "\tmov edi, %d\n", temp->child_list->head->tokenInfo->lineno);
                    fprintf(fp, "\tpush edi\n");
                    fprintf(fp, "\tpush OutofBoundError\n");
                    fprintf(fp, "\tcall printf\n");
                    fprintf(fp, "\tjmp EXIT\n");

                    fprintf(fp, "\nL_END_%d:\n", start);
                }
            }
        }
        // non-array variable
        else
        {
            // int offset = n->SymbolTableNode->variable.offset;

            if (strcmp(n->SymbolTableNode->variable.addr, "integer") == 0)
            {
                printMsg("output", fp);
                char *str = malloc(sizeof(char) * 30);
                sprintf(str, "%s_%d_%d", temp->child_list->head->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                printDOut(str, fp);
            }

            else if (strcmp(n->SymbolTableNode->variable.addr, "boolean") == 0)
            {
                printMsg("output", fp);

                int start = get_next_label();
                char *str = malloc(sizeof(char) * 30);
                sprintf(str, "%s_%d_%d", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                fprintf(fp, "\tmov esi, [%s]\n", str);
                fprintf(fp, "\tcmp esi, 1\n");
                fprintf(fp, "\tje LABEL_%d\n", start);

                printMsg("falseMsg", fp);

                fprintf(fp, "\tjmp LABEL_END_%d\n", start);

                fprintf(fp, "\nLABEL_%d:\n", start);

                printMsg("trueMsg", fp);

                fprintf(fp, "\nLABEL_END_%d:\n", start);
            }
        }
    }

    else if (node->child_list->head->tokenInfo && node->child_list->head->tokenInfo->t == TRUE)
    {
        printMsg("output", fp);

        printMsg("trueMsg", fp);
    }

    else if (node->child_list->head->tokenInfo && node->child_list->head->tokenInfo->t == FALSE)
    {
        printMsg("output", fp);

        printMsg("falseMsg", fp);
    }

    else if (node->child_list->head->tokenInfo && node->child_list->head->tokenInfo->t == NUM)
    {
        printMsg("output", fp);

        fprintf(fp, "\tmov edi, %d\n", node->child_list->head->tokenInfo->val->intValue);

        printDOut("edi", fp);
    }
}

void processAssignmentStmt_asm(astNode *node, FILE *fp)
{
    astNode *lhs = node->child_list->head;

    templookup *node_t = lookupIDCodegen(node->tb, lhs->tokenInfo->lexeme);
    Node *n = node_t->n;
    Table *parentTable = node_t->t;

    astNode *rhs;

    // if (n->SymbolTableNode->variable.var_tag == 0) // variable
    if (node->child_list->head->sibling->n_Name == lvalueIDStmt_ast)
        rhs = node->child_list->head->sibling->child_list->head->sibling;
    else //array indices
    {
        rhs = node->child_list->head->sibling->child_list->head->sibling->sibling;
    }

    //------------------------------------------------------------------------------

    evaluateAssignmentStmt(rhs, fp, lhs);

    //------------------------------------------------------------------------------
    // printf("RHS Evaluated\n");
    fprintf(fp, "\tpop eax\n");

    char *str = malloc(sizeof(char) * 30);
    sprintf(str, "%s_%d_%d", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

    // LHS is ID
    if (lhs->sibling->n_Name == lvalueIDStmt_ast)
    {
        if (n->SymbolTableNode->variable.var_tag != 1) //normal identifier
            fprintf(fp, "\tmov dword [%s], eax\n", str);
        else //array variable e.g. [A := B]
        {
            Leaf *lower = n->SymbolTableNode->variable.lower_index;
            Leaf *upper = n->SymbolTableNode->variable.upper_index;

            if (lower->t == NUM && upper->t == NUM) //static array
            {
                // fprintf(fp, "\tmov dword [%s], eax\n", str);
                fprintf(fp, "\tmov esi, %d\n", lower->val->intValue);
                fprintf(fp, "\tpush esi\n");
                fprintf(fp, "\tmov esi, %d\n", upper->val->intValue);
                fprintf(fp, "\tpush esi\n");
                fprintf(fp, "\tcall _getarrlength\n");
                fprintf(fp, "\tpop esi\n");
                fprintf(fp, "\tmov edi, 0\n");

                int label = get_next_label();
                fprintf(fp, "\nassign_%d:\n", label);
                char *str = malloc(sizeof(char) * 20);
                sprintf(str, "%s_%d_%d + 4 * edi", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                fprintf(fp, "\tpop dword [%s]\n", str);
                fprintf(fp, "\tinc edi\n");
                fprintf(fp, "\tcmp edi, esi\n");
                fprintf(fp, "\tjnz assign_%d\n", label);
            }

            else //dynamic array
            {
                dynamicArrayHandle(node_t, node->tb, fp);

                fprintf(fp, "\tpop esi\n"); //get length of array in esi
                fprintf(fp, "\tmov edi, 0\n");
                int label = get_next_label();
                fprintf(fp, "\nassign_%d:\n", label);

                fprintf(fp, "\tmov ebx, dword[%s_%d_%d + DARRAY.ptr]\n", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                fprintf(fp, "\tmov eax, 4\n");
                fprintf(fp, "\tmul edi\n");
                fprintf(fp, "\tadd ebx, eax\n");
                fprintf(fp, "\tpop dword [ebx]\n");
                fprintf(fp, "\tdec edi\n");
                fprintf(fp, "\tcmp edi, esi\n");
                fprintf(fp, "\tjnz assign_%d\n", label);
                // fprintf(fp, "\tjmp L_END_%d\n", label);
            }
        }
    }

    // LHS is array variable
    else if (lhs->sibling->n_Name == lvalueARRStmt_ast)
    {
        astNode *index = lhs->sibling->child_list->head;

        // static array
        if (n->SymbolTableNode->variable.lower_index->t == NUM && n->SymbolTableNode->variable.upper_index->t == NUM) //static array
        {
            // static_array[3] = ...
            if (index->child_list->head->tokenInfo->t == NUM)
            {
                // printf("Hello\n");
                fprintf(fp, "\tmov ebx, %d\n", n->SymbolTableNode->variable.lower_index->val->intValue);
                fprintf(fp, "\tmov esi, %d\n", index->child_list->head->tokenInfo->val->intValue);
                fprintf(fp, "\tsub esi, ebx\n");
                fprintf(fp, "\tmov edi, esi\n");
                fprintf(fp, "\tmov dword[%s + 4 * edi], eax\n", str);
                // printf("Hello_end\n");
            }

            // static_array[i] = ...
            else //bound check-- A[i]
            {

                int start = DynamicArrayIndexHandle(node_t, node->tb, fp, index->child_list->head);

                fprintf(fp, "\tmov dword[%s + 4 * edi], eax\n", str); //eax has the value popped from stack
                fprintf(fp, "\tjmp L_END_%d\n", start);

                //Here give out of bound error
                fprintf(fp, "\nL_%d:\n", start);

                fprintf(fp, "\tmov edi, %d\n", lhs->tokenInfo->lineno);
                fprintf(fp, "\tpush edi\n");
                fprintf(fp, "\tpush OutofBoundError\n");
                fprintf(fp, "\tcall printf\n");
                fprintf(fp, "\tjmp EXIT\n");

                fprintf(fp, "\nL_END_%d:\n", start);
            }
        }

        //dynamic array
        else
        {
            int start = DynamicArrayIndexHandle(node_t, node->tb, fp, index->child_list->head);
            fprintf(fp, "\tmov ebx, dword[%s + DARRAY.ptr]\n", str);

            fprintf(fp, "\tmov esi, eax\n");
            fprintf(fp, "\tmov eax, 4\n");
            fprintf(fp, "\tmul edi\n");
            fprintf(fp, "\tadd ebx, eax\n");
            fprintf(fp, "\tmov dword [ebx], esi\n");

            fprintf(fp, "\tjmp L_END_%d\n", start);

            //Here give out of bound error
            fprintf(fp, "\nL_%d:\n", start);

            fprintf(fp, "\tmov edi, %d\n", lhs->tokenInfo->lineno);
            fprintf(fp, "\tpush edi\n");
            fprintf(fp, "\tpush OutofBoundError\n");
            fprintf(fp, "\tcall printf\n");
            fprintf(fp, "\tjmp EXIT\n");

            fprintf(fp, "\nL_END_%d:\n", start);
        }
    }
}

void evaluateAssignmentStmt(astNode *node, FILE *fp, astNode *lhs)
{

    if (node->n_Name == plus_ast || node->n_Name == minus_ast || node->n_Name == mul_ast || node->n_Name == div_ast)
    {
        astNode *left = node->child_list->head;
        astNode *right = left->sibling;

        evaluateAssignmentStmt(left, fp, lhs);

        evaluateAssignmentStmt(right, fp, lhs);

        fprintf(fp, "\tpop ebx\n");
        fprintf(fp, "\tpop eax\n");

        if (node->n_Name == plus_ast)
        {
            fprintf(fp, "\tadd eax, ebx\n");
            fprintf(fp, "\tpush eax\n");
        }

        else if (node->n_Name == minus_ast)
        {
            fprintf(fp, "\tsub eax, ebx\n");
            fprintf(fp, "\tpush eax\n");
        }

        else if (node->n_Name == mul_ast)
        {
            fprintf(fp, "\timul eax, ebx\n");
            fprintf(fp, "\tpush eax\n");
        }

        else if (node->n_Name == div_ast)
        {
            fprintf(fp, "\tdiv eax, ebx\n");
            fprintf(fp, "\tpush eax\n");
        }
    }

    else if (node->n_Name == lt_ast || node->n_Name == gt_ast || node->n_Name == le_ast || node->n_Name == ge_ast || node->n_Name == eq_ast || node->n_Name == ne_ast)
    {
        astNode *left = node->child_list->head;
        astNode *right = left->sibling;

        evaluateAssignmentStmt(left, fp, lhs);

        evaluateAssignmentStmt(right, fp, lhs);

        fprintf(fp, "\tpop ebx\n");
        fprintf(fp, "\tpop eax\n");
        fprintf(fp, "\tcmp eax, ebx\n");
        int start = get_next_label();
        int end = get_next_label();

        if (node->n_Name == le_ast)
        {
            fprintf(fp, "\tjle LABEL_%d\n", start);
            fprintf(fp, "\tmov edx, 0\n");
            fprintf(fp, "\tpush edx\n");
            fprintf(fp, "\tjmp LABEL_%d\n", end);
        }

        else if (node->n_Name == ge_ast)
        {
            fprintf(fp, "\tjge LABEL_%d\n", start);
            fprintf(fp, "\tmov edx, 0\n");
            fprintf(fp, "\tpush edx\n");
            fprintf(fp, "\tjmp LABEL_%d\n", end);
        }

        else if (node->n_Name == lt_ast)
        {
            fprintf(fp, "\tjl LABEL_%d\n", start);
            fprintf(fp, "\tmov edx, 0\n");
            fprintf(fp, "\tpush edx\n");
            fprintf(fp, "\tjmp LABEL_%d\n", end);
        }

        else if (node->n_Name == gt_ast)
        {
            fprintf(fp, "\tjg LABEL_%d\n", start);
            fprintf(fp, "\tmov edx, 0\n");
            fprintf(fp, "\tpush edx\n");
            fprintf(fp, "\tjmp LABEL_%d\n", end);
        }

        else if (node->n_Name == ne_ast)
        {
            fprintf(fp, "\tjne LABEL_%d\n", start);
            fprintf(fp, "\tmov edx, 0\n");
            fprintf(fp, "\tpush edx\n");
            fprintf(fp, "\tjmp LABEL_%d\n", end);
        }

        else if (node->n_Name == eq_ast)
        {
            fprintf(fp, "\tje LABEL_%d\n", start);
            fprintf(fp, "\tmov edx, 0\n");
            fprintf(fp, "\tpush edx\n");
            fprintf(fp, "\tjmp LABEL_%d\n", end);
        }

        fprintf(fp, "LABEL_%d:\n\n", start);
        fprintf(fp, "\tmov edx, 1\n");
        fprintf(fp, "\tpush edx\n\n");
        fprintf(fp, "LABEL_%d:\n\n", end);
    }

    else if (node->n_Name == and_ast || node->n_Name == or_ast)
    {
        astNode *left = node->child_list->head;
        astNode *right = left->sibling;

        evaluateAssignmentStmt(left, fp, lhs);
        evaluateAssignmentStmt(right, fp, lhs);

        fprintf(fp, "\tpop ebx\n");
        fprintf(fp, "\tpop eax\n");

        int start = get_next_label();
        int end = get_next_label();

        if (node->n_Name == and_ast)
        {
            fprintf(fp, "\tcmp eax, 1\n");
            fprintf(fp, "\tje LABEL_%d\n", start);
            fprintf(fp, "\tmov eax, 0\n");
            fprintf(fp, "\tpush eax\n");
            fprintf(fp, "\tjmp LABEL_END_%d\n", end);

            fprintf(fp, "\nLABEL_%d:\n", start);
            fprintf(fp, "\tcmp ebx, 1\n");
            fprintf(fp, "\tje LABEL_%d\n", end);
            fprintf(fp, "\tmov ebx, 0\n");
            fprintf(fp, "\tpush ebx\n");
            fprintf(fp, "\tjmp LABEL_END_%d\n", end);

            fprintf(fp, "\nLABEL_%d:\n", end);
            fprintf(fp, "\tmov ebx, 1\n");
            fprintf(fp, "\tpush ebx\n");

            fprintf(fp, "\nLABEL_END_%d:\n", end);
        }

        else if (node->n_Name == or_ast)
        {
            fprintf(fp, "\tcmp eax, 0\n");
            fprintf(fp, "\tje LABEL_%d\n", start);
            fprintf(fp, "\tmov eax, 1\n");
            fprintf(fp, "\tpush eax\n");
            fprintf(fp, "\tjmp LABEL_END_%d\n", end);

            fprintf(fp, "\nLABEL_%d:\n", start);
            fprintf(fp, "\tcmp ebx, 0\n");
            fprintf(fp, "\tje LABEL_%d\n", end);
            fprintf(fp, "\tmov ebx, 1\n");
            fprintf(fp, "\tpush ebx\n");
            fprintf(fp, "\tjmp LABEL_END_%d\n", end);

            fprintf(fp, "\nLABEL_%d:\n", end);
            fprintf(fp, "\tmov ebx, 0\n");
            fprintf(fp, "\tpush ebx\n");

            fprintf(fp, "\nLABEL_END_%d:\n", end);
        }
    }

    else if (node->n_Name == var_id_num_ast)
    {
        templookup *node_t = lookupIDCodegen(node->tb, node->child_list->head->tokenInfo->lexeme);
        Node *n = node_t->n;
        Table *parentTable = node_t->t;

        char *str = malloc(sizeof(char) * 30);
        sprintf(str, "%s_%d_%d", node->child_list->head->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

        if (n->SymbolTableNode->variable.var_tag != 1) //normal variable
        {
            fprintf(fp, "\tmov  eax, [%s]\n", str);
            fprintf(fp, "\tpush eax\n");
        }
        else if (n->SymbolTableNode->variable.var_tag == 1 && node->child_list->head->sibling != NULL) // array index variable
        {
            astNode *index = node->child_list->head->sibling;

            // static arrays
            if (n->SymbolTableNode->variable.lower_index->t == NUM && n->SymbolTableNode->variable.upper_index->t == NUM) //static array
            {
                if (index->child_list->head->tokenInfo->t == NUM)
                {
                    fprintf(fp, "\tmov ebx, %d\n", n->SymbolTableNode->variable.lower_index->val->intValue);
                    fprintf(fp, "\tmov eax, %d\n", index->child_list->head->tokenInfo->val->intValue);
                    fprintf(fp, "\tsub eax, ebx\n");
                    fprintf(fp, "\tmov edi, eax\n");

                    fprintf(fp, "\tpush dword[%s + 4 * edi]\n", str);
                }

                else //bound check-- A[i]
                {
                    astNode *id = index->child_list->head;
                    templookup *node_t_temp = lookupIDCodegen(node->tb, id->tokenInfo->lexeme);
                    Node *n_temp = node_t_temp->n;
                    Table *parentTable_temp = node_t_temp->t;

                    char *tempstr = malloc(sizeof(char) * 30);
                    sprintf(tempstr, "%s_%d_%d", id->tokenInfo->lexeme, parentTable_temp->parent->SymbolTableNode->block.lower_scope, parentTable_temp->parent->SymbolTableNode->block.upper_scope);

                    fprintf(fp, "\tmov edx, [%s]\n", tempstr);

                    int start = get_next_label();

                    fprintf(fp, "\tmov esi, %d\n", n->SymbolTableNode->variable.lower_index->val->intValue);
                    fprintf(fp, "\tcmp edx, esi\n");
                    fprintf(fp, "\tjl L_%d\n", start); // if less than lower bound

                    fprintf(fp, "\tmov esi, %d\n", n->SymbolTableNode->variable.upper_index->val->intValue);
                    fprintf(fp, "\tcmp edx, esi\n");
                    fprintf(fp, "\tjg L_%d\n", start); //if greater than upper bound

                    //comes here if range is within bound

                    fprintf(fp, "\tmov esi, [%s]\n", tempstr); //value of index is in esi
                    fprintf(fp, "\tmov ebx, %d\n", n->SymbolTableNode->variable.lower_index->val->intValue);

                    fprintf(fp, "\tsub esi, ebx\n");
                    fprintf(fp, "\tmov edi, esi\n");

                    fprintf(fp, "\tpush dword[%s + 4 * edi]\n", str);
                    fprintf(fp, "\tjmp L_END_%d\n", start);

                    //Here give out of bound error
                    fprintf(fp, "\nL_%d:\n", start);

                    fprintf(fp, "\tmov edi, %d\n", node->child_list->head->tokenInfo->lineno);
                    fprintf(fp, "\tpush edi\n");
                    fprintf(fp, "\tpush OutofBoundError\n");
                    fprintf(fp, "\tcall printf\n");
                    fprintf(fp, "\tjmp EXIT\n");

                    fprintf(fp, "\nL_END_%d:\n", start);
                }
            }

            else //dynamic array -- perform bound check
            {
                int start = DynamicArrayIndexHandle(node_t, node->tb, fp, index->child_list->head);

                fprintf(fp, "\tmov ebx, dword[%s + DARRAY.ptr]\n", str);

                fprintf(fp, "\tmov eax, 4\n");
                fprintf(fp, "\tmul edi\n");
                fprintf(fp, "\tadd ebx, eax\n");
                fprintf(fp, "\tpush dword [ebx]\n");

                fprintf(fp, "\tjmp L_END_%d\n", start);

                //Here give out of bound error
                fprintf(fp, "\nL_%d:\n", start);

                fprintf(fp, "\tmov edi, %d\n", node->child_list->head->tokenInfo->lineno);
                fprintf(fp, "\tpush edi\n");
                fprintf(fp, "\tpush OutofBoundError\n");
                fprintf(fp, "\tcall printf\n");
                fprintf(fp, "\tjmp EXIT\n");

                fprintf(fp, "\nL_END_%d:\n", start);
            }
        }

        else if (n->SymbolTableNode->variable.var_tag == 1)
        {
            Leaf *lower = n->SymbolTableNode->variable.lower_index;
            Leaf *upper = n->SymbolTableNode->variable.upper_index;

            // static array
            if (lower->t == NUM && upper->t == NUM)
            {
                fprintf(fp, "\tmov eax, %d\n", lower->val->intValue);
                fprintf(fp, "\tpush eax\n");
                fprintf(fp, "\tmov edi, %d\n", upper->val->intValue);
                fprintf(fp, "\tpush edi\n");
                fprintf(fp, "\tcall _getarrlength\n");
                fprintf(fp, "\tpop esi\n");
                fprintf(fp, "\tmov edi, esi\n");
                fprintf(fp, "\tdec edi\n");

                int label = get_next_label();
                fprintf(fp, "\nassign_%d:\n", label);

                char *str = malloc(sizeof(char) * 20);
                sprintf(str, "%s_%d_%d + 4 * edi", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                fprintf(fp, "\tpush dword[%s]\n", str);
                fprintf(fp, "\tinc edi\n");
                fprintf(fp, "\tcmp edi, esi\n");
                fprintf(fp, "\tcmp edi, 0\n");
                fprintf(fp, "\tjl assign_%d\n", label);
            }

            // dynamic array
            else
            {
                templookup *node_t_lhs = lookupIDCodegen(node->tb, lhs->tokenInfo->lexeme);
                Node *n_lhs = node_t_lhs->n;
                Table *parentTable_lhs = node_t_lhs->t;

                if (lower->t == NUM)
                {
                    fprintf(fp, "\tmov esi, %d\n", node_t->n->SymbolTableNode->variable.lower_index->val->intValue);
                }
                else
                {
                    templookup *n_temp = lookupIDCodegen(node->tb, lower->lexeme);
                    Table *localTable = n_temp->t;

                    char *strtemp = malloc(sizeof(char) * 30);
                    sprintf(strtemp, "%s_%d_%d", lower->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
                    fprintf(fp, "\tmov esi, [%s]\n", strtemp);
                }

                if (n_lhs->SymbolTableNode->variable.lower_index->t == NUM)
                {
                    fprintf(fp, "\tmov edi, %d\n", node_t_lhs->n->SymbolTableNode->variable.lower_index->val->intValue);
                }
                else
                {
                    templookup *n_temp = lookupIDCodegen(node->tb, n_lhs->SymbolTableNode->variable.lower_index->lexeme);
                    Table *localTable = n_temp->t;

                    char *strtemp = malloc(sizeof(char) * 30);
                    sprintf(strtemp, "%s_%d_%d", n_lhs->SymbolTableNode->variable.lower_index->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
                    fprintf(fp, "\tmov edi, [%s]\n", strtemp);
                }
                int label = get_next_label();
                fprintf(fp, "\tcmp esi, edi\n");
                fprintf(fp, "\tjne L_%d\n", label);

                //now campare upper bound
                if (upper->t == NUM)
                {
                    fprintf(fp, "\tmov esi, %d\n", upper->val->intValue);
                }

                else
                {
                    templookup *n_temp = lookupIDCodegen(node->tb, upper->lexeme);
                    Table *localTable = n_temp->t;

                    char *strtemp = malloc(sizeof(char) * 30);
                    sprintf(strtemp, "%s_%d_%d", upper->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
                    fprintf(fp, "\tmov esi, [%s]\n", strtemp);
                }

                if (n_lhs->SymbolTableNode->variable.upper_index->t == NUM)
                {
                    fprintf(fp, "\tmov edi, %d\n", node_t_lhs->n->SymbolTableNode->variable.upper_index->val->intValue);
                }

                else
                {
                    templookup *n_temp = lookupIDCodegen(node->tb, n_lhs->SymbolTableNode->variable.upper_index->lexeme);
                    Table *localTable = n_temp->t;

                    char *strtemp = malloc(sizeof(char) * 30);
                    sprintf(strtemp, "%s_%d_%d", n_lhs->SymbolTableNode->variable.upper_index->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
                    fprintf(fp, "\tmov edi, [%s]\n", strtemp);
                }

                fprintf(fp, "\tcmp esi, edi\n");
                fprintf(fp, "\tjne L_%d\n", label);

                //comes here if bound match
                dynamicArrayHandle(node_t, node->tb, fp);

                fprintf(fp, "\tpop esi\n"); //get length of array in esi
                fprintf(fp, "\tmov edi, esi\n");
                fprintf(fp, "\tdec edi\n");

                fprintf(fp, "\nassign_%d:\n", label);

                fprintf(fp, "\tmov ebx, dword[%s_%d_%d + DARRAY.ptr]\n", n->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                fprintf(fp, "\tmov eax, 4\n");
                fprintf(fp, "\tmul edi\n");
                fprintf(fp, "\tadd ebx, eax\n");
                fprintf(fp, "\tpush dword [ebx]");
                fprintf(fp, "\tdec edi\n");
                fprintf(fp, "\tcmp edi, 0\n");
                fprintf(fp, "\tjl assign_%d\n", label);
                fprintf(fp, "\tjmp L_END_%d\n", label);

                //Here give out of bound mismatch
                fprintf(fp, "\nL_%d:\n", label);

                fprintf(fp, "\tmov edi, %d\n", node->child_list->head->tokenInfo->lineno);
                fprintf(fp, "\tpush edi\n");
                fprintf(fp, "\tpush BoundMismatchError\n");
                fprintf(fp, "\tcall printf\n");
                fprintf(fp, "\tjmp EXIT\n");
                fprintf(fp, "\nL_END_%d:\n", label);
            }
        }
    }

    else if (node->tokenInfo && node->tokenInfo->t == NUM)
    {
        fprintf(fp, "\tmov  eax, %d\n", node->tokenInfo->val->intValue);
        fprintf(fp, "\tpush eax\n");
    }

    else if (node->tokenInfo && node->tokenInfo->t == RNUM)
    {
        fprintf(fp, "\tmov  eax, %f\n", node->tokenInfo->val->fValue);
        fprintf(fp, "\tpush eax\n");
    }

    else if (node->tokenInfo && node->tokenInfo->t == TRUE)
    {
        fprintf(fp, "\tmov  eax, 1\n");
        fprintf(fp, "\tpush eax\n");
    }

    else if (node->tokenInfo && node->tokenInfo->t == FALSE)
    {
        fprintf(fp, "\tmov  eax, 0\n");
        fprintf(fp, "\tpush eax\n");
    }
}

void processWhileLoop(astNode *node, FILE *fp)
{
    int start = get_next_label();
    // printf("Inside while process...\n");
    // int start = 1;
    fprintf(fp, "while_%d:\n", start);

    evaluateAssignmentStmt(node->child_list->head, fp, NULL);

    fprintf(fp, "\tpop edx\n");
    fprintf(fp, "\tcmp edx, 0\n");
    fprintf(fp, "\tjz \t while_end%d\n", start);

    // processOtherStmts(node->child_list->head->sibling->sibling, fp);
    processOtherStmts(node, fp, node->tb->nesting_level);

    fprintf(fp, "\tjmp \t while_%d\n", start);
    fprintf(fp, "while_end%d:\n\n", start);
}

void processOtherStmts(astNode *node, FILE *fp, int nesting_level)
{
    if (node == NULL)
        return;

    if (node->n_Name == ioStmt_get_value_ist)
    {
        // printf("Here -- get_value\n");
        processIO_getValue_Stmt(node, fp);
    }

    else if (node->n_Name == ioStmt_print_ast)
    {
        // printf("Here -- print\n");
        processIO_print_Stmt(node, fp);
    }

    else if (node->n_Name == assignmentStmt_ast)
    {
        // printf("Here -- assign\n");
        processAssignmentStmt_asm(node, fp);
    }

    else if (node->n_Name == moduleReuseStmt_ast)
    {
        // printf("Here -- modulecall\n");
        astNode *temp = node->child_list->head;
        astNode *moduleName;
        if (temp->n_Name == optional_ast) //module returns variables
        {
            astNode *outputList = temp->child_list->head->child_list->head;
            moduleName = temp->sibling;
            astNode *inputList = temp->sibling->sibling->child_list->head;
            processModuleReUseStmt(node, inputList, outputList, moduleName, fp);
        }

        else //function which doesn't return anything
        {
            moduleName = temp->sibling;
            astNode *inputList = temp->sibling->sibling->child_list->head;
            processModuleReUseStmt(node, inputList, NULL, moduleName, fp);
        }
    }

    else if (node->n_Name == itr_for_ast && node->tb->nesting_level != nesting_level)
    {
        // printf("Here -- for\n");
        processForLoop(node, fp);
    }

    else if (node->n_Name == itr_while_ast && node->tb->nesting_level != nesting_level)
    {
        // printf("Here -- while\n");
        processWhileLoop(node, fp);
    }

    else if (node->n_Name == conditionalStmt_ast && node->tb->nesting_level != nesting_level)
    {
        // printf("Here -- switch\n");
        processSwitchStmt(node, fp);
    }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        if (node->n_Name == conditionalStmt_ast)
        {
            temp = NULL;
        }
        while (temp)
        {
            processOtherStmts(temp, fp, nesting_level);
            temp = temp->sibling;
        }
    }
}

void processForLoop(astNode *node, FILE *fp)
{
    astNode *temp = node->child_list->head;
    astNode *range = node->child_list->head->sibling;

    templookup *node_t = lookupIDCodegen(node->tb, temp->tokenInfo->lexeme);
    Node *n = node_t->n;
    Table *parentTable = node_t->t;

    // printf("Here -- for\n");
    char *str = malloc(sizeof(char) * 30);
    sprintf(str, "%s_%d_%d", temp->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
    // printf("Here -- for\n");
    fprintf(fp, "\tmov dword [%s], %d\n", str, range->child_list->head->tokenInfo->val->intValue);
    // printf("Here -- for\n");
    int start = get_next_label();

    fprintf(fp, "\n FOR_%d: \n", start);
    fprintf(fp, "\tmov edi, %d\n", range->child_list->head->sibling->tokenInfo->val->intValue); // mov the high range

    fprintf(fp, "\tmov esi, [%s]\n", str);
    fprintf(fp, "\tcmp esi, edi\n");

    fprintf(fp, "\tjg FOR_END%d\n", start);

    processOtherStmts(node, fp, node->tb->nesting_level);

    fprintf(fp, "\tmov esi, [%s]\n", str);
    fprintf(fp, "\tinc esi\n");
    fprintf(fp, "\tmov dword [%s], esi\n", str);
    fprintf(fp, "\tjmp FOR_%d \n", start);

    fprintf(fp, "\nFOR_END%d: \n", start);
}

void processSwitchStmt(astNode *node, FILE *fp)
{
    astNode *temp = node->child_list->head;

    templookup *node_t = lookupIDCodegen(node->tb->container, temp->tokenInfo->lexeme);
    Node *n = node_t->n;
    Table *parent_table = node_t->t;

    if (strcmp(n->SymbolTableNode->variable.addr, "boolean") == 0)
    {

        char *str = malloc(sizeof(char) * 30);
        sprintf(str, "%s_%d_%d", n->val, parent_table->parent->SymbolTableNode->block.lower_scope, parent_table->parent->SymbolTableNode->block.upper_scope);

        // fprintf(fp, "\tmov esi, [%s]\n", n->val);

        fprintf(fp, "\tmov esi, [%s]\n", str);
        int start = get_next_label();
        fprintf(fp, "\tcmp esi, 1\n");
        fprintf(fp, "\tje CASE_TRUE_%d\n", start);
        fprintf(fp, "\tcmp esi, 0\n");
        fprintf(fp, "\tjne CASE_FALSEEND_%d\n", start);

        processCaseStmt(node, fp, "false"); // false case
        // printf("Hi 3\n");
        fprintf(fp, "\tjmp CASE_FALSEEND_%d\n", start);
        fprintf(fp, "\nCASE_TRUE_%d:\n", start);

        processCaseStmt(node, fp, "true"); // true case

        fprintf(fp, "\nCASE_FALSEEND_%d:\n", start);
    }

    else if (strcmp(n->SymbolTableNode->variable.addr, "integer") == 0)
    {
        char *str = malloc(sizeof(char) * 30);
        sprintf(str, "%s_%d_%d", n->val, parent_table->parent->SymbolTableNode->block.lower_scope, parent_table->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tmov esi, [%s]\n", str);
        int start = get_next_label();
        fprintf(fp, "\nCASE_%d:\n", start);
        int next = get_next_label();

        processIntSwitch(node, fp, next);

        // printf("Exit from switch\n");
    }
}

void processCaseStmt(astNode *node, FILE *fp, char *str)
{
    if (node == NULL)
        return;

    if (node->n_Name == caseStmts_ast && strcmp(str, node->child_list->head->tokenInfo->lexeme) == 0)
    {
        processOtherStmts(node, fp, node->tb->nesting_level);
    }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            processCaseStmt(temp, fp, str);
            temp = temp->sibling;
        }
    }
}

void processIntSwitch(astNode *node, FILE *fp, int label)
{

    if (node == NULL)
        return;

    if (node->n_Name == caseStmts_ast)
    {
        astNode *ast = node->child_list->head;

        fprintf(fp, "\tmov edi, %d\n", ast->tokenInfo->val->intValue);
        fprintf(fp, "\tcmp esi, edi\n");
        int start = get_next_label();
        fprintf(fp, "\tjne CASE_%d\n\n", start);

        // processCaseStmt(node, fp, node->child_list->head->tokenInfo->lexeme);
        processOtherStmts(node, fp, node->tb->nesting_level);

        fprintf(fp, "\tjmp DEFAULT_END_%d\n", label);

        fprintf(fp, "\nCASE_%d: \n", start);

        templookup *node_t = lookupIDCodegen(node->tb, node->parent->child_list->head->tokenInfo->lexeme);
        Node *n = node_t->n;
        Table *parentTable = node_t->t;

        char *str = malloc(sizeof(char) * 30);
        sprintf(str, "%s_%d_%d", node->parent->child_list->head->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tmov esi, [%s]\n", str);
    }

    if (node->n_Name == default_ast)
    {
        processOtherStmts(node, fp, node->tb->nesting_level);

        fprintf(fp, "\nDEFAULT_END_%d: \n", label);
    }

    Children *chi = node->child_list;
    astNode *temp;
    if (chi)
    {
        temp = chi->head;
        while (temp)
        {
            processIntSwitch(temp, fp, label);
            temp = temp->sibling;
        }
    }
}

void processModuleReUseStmt(astNode *node, astNode *inputList, astNode *outputList, astNode *moduleName, FILE *fp)
{

    Node *n;
    Table *parentTable_local;

    while (inputList) // push input parameters
    {

        templookup *node_t = lookupIDCodegen(node->tb, inputList->tokenInfo->lexeme);
        Node *n = node_t->n;
        Table *parentTable = node_t->t;
        if (n->SymbolTableNode->variable.var_tag != 1)
            fprintf(fp, "\tpush dword [%s_%d_%d]\n", inputList->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
        else
        {
            Leaf *lower = n->SymbolTableNode->variable.lower_index;
            Leaf *upper = n->SymbolTableNode->variable.upper_index;

            if (lower->t == NUM && upper->t == NUM) //static array
                fprintf(fp, "\tpush %s_%d_%d\n", inputList->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

            else //dynamic array
            {
                dynamicArrayHandle(node_t, node->tb, fp);
                fprintf(fp, "\tpop edi\n");
                fprintf(fp, "\tmov dword[%s_%d_%d + DARRAY.count], edi\n", inputList->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
                fprintf(fp, "\tpush dword[%s_%d_%d]\n", inputList->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
            }
        }
        inputList = inputList->sibling;
    }

    // call module
    fprintf(fp, "\tcall _%s\n", moduleName->tokenInfo->lexeme);

    // if module returns (run for every return parameter in a loop)
    while (outputList)
    {

        templookup *node_t = lookupIDCodegen(node->tb, outputList->tokenInfo->lexeme);
        Node *n = node_t->n;
        Table *parentTable = node_t->t;

        char *str = malloc(sizeof(char) * 30);
        sprintf(str, "%s_%d_%d", outputList->tokenInfo->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tpop edi\n");
        fprintf(fp, "\tmov [%s], edi\n", str);

        outputList = outputList->sibling;
    }
}

void defineAllModules(astNode *root, FILE *fp)
{
    astNode *temp = root->child_list->head;

    while (temp)
    {
        if (temp->n_Name == module_ast)
        {
            defineModule(temp, fp);
        }
        temp = temp->sibling;
    }
}

void defineModule(astNode *node, FILE *fp)
{
    Node *n = lookUpST(node->child_list->head->tokenInfo->lexeme, baseTb);

    if (n)
    {
        List *in_list = n->SymbolTableNode->block.input_list;
        List *out_list = n->SymbolTableNode->block.output_list;

        fprintf(fp, "\n_%s:\n", n->val);

        fprintf(fp, "\tpop ebp\n");
        // fprintf(fp, "\tpush ebp\n");
        // fprintf(fp, "\tpush ebx\n");
        // fprintf(fp, "\tpush esi\n");
        // fprintf(fp, "\tpush edi\n");
        while (in_list->next)
            in_list = in_list->next;

        while (in_list)
        {
            if (in_list->var_tag != 1)
                fprintf(fp, "\tpop dword [%s_%d_%d]\n", in_list->val, n->SymbolTableNode->block.lower_scope, n->SymbolTableNode->block.upper_scope);
            else
            {
                templookup *node_t = lookupIDCodegen(n->SymbolTableNode->block.scope, in_list->val);
                Node *ntemp = node_t->n;
                Table *parentTable = node_t->t;

                Leaf *lower = ntemp->SymbolTableNode->variable.lower_index;
                Leaf *upper = ntemp->SymbolTableNode->variable.upper_index;

                // static array
                if (lower->t == NUM && upper->t == NUM)
                {
                    fprintf(fp, "\tmov eax, %d\n", lower->val->intValue);
                    fprintf(fp, "\tpush eax\n");
                    fprintf(fp, "\tmov edi, %d\n", upper->val->intValue);
                    fprintf(fp, "\tpush edi\n");
                    fprintf(fp, "\tcall _getarrlength\n");
                    fprintf(fp, "\tpop esi\n"); //formal parameter length

                    fprintf(fp, "\tpop ebx\n"); // addr of actual param
                    fprintf(fp, "\tmov edi, 0\n");
                    // fprintf(fp, "\t\n");

                    int label = get_next_label();
                    fprintf(fp, "\nassign_%d:\n", label);

                    char *str = malloc(sizeof(char) * 20);
                    sprintf(str, "%s_%d_%d + 4 * edi", ntemp->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    fprintf(fp, "\tmov eax, 4\n");
                    fprintf(fp, "\tmul edi\n");
                    fprintf(fp, "\tadd ebx, edi\n");
                    fprintf(fp, "\tmov ecx, dword[ebx]\n"); // moving actual param
                    fprintf(fp, "\tmov dword[%s], ecx\n", str);
                    fprintf(fp, "\tinc edi\n");
                    fprintf(fp, "\tcmp edi, esi\n");
                    fprintf(fp, "\tdec edi\n");
                    fprintf(fp, "\tcmp edi, esi\n");
                    fprintf(fp, "\tjnz assign_%d\n", label);
                }

                // dynamic array
                else
                {

                    int label = get_next_label();

                    fprintf(fp, "\tpop ebx\n"); //actual parameter address
                    fprintf(fp, "\tmov edi, 0\n");
                    fprintf(fp, "\tmov eax, dword [ebx + DARRAY.count]\n");

                    // allocate space for formal param
                    fprintf(fp, "\tmov dword[%s_%d_%d + DARRAY.count], eax\n", ntemp->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);
                    fprintf(fp, "\tsal eax, 2\n");
                    fprintf(fp, "\tpush eax\n");
                    fprintf(fp, "\tcall malloc\n");
                    fprintf(fp, "\tadd esp, 4\n");
                    fprintf(fp, "\tmov dword [%s_%d_%d + DARRAY.ptr], eax\n", ntemp->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    fprintf(fp, "\nassign_%d:\n", label);
                    // formal param
                    // fprintf(fp, "\tmov ebx, dword[%s_%d_%d + DARRAY.ptr]\n", ntemp->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

                    fprintf(fp, "\tmov eax, 4\n");
                    fprintf(fp, "\tmul edi\n");
                    fprintf(fp, "\tadd ebx, eax\n");
                    fprintf(fp, "\tmov ecx, dword [ebx]\n");                                                                                                                                                          // move actual param
                    fprintf(fp, "\tmov dword[%s_%d_%d + DARRAY.ptr + 4 * edi], ecx\n", ntemp->val, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope); // formal

                    fprintf(fp, "\tinc edi\n");
                    fprintf(fp, "\tcmp edi, esi\n");
                    fprintf(fp, "\tjnz assign_%d\n", label);
                }
            }

            in_list = in_list->prev;
        }

        // process statements inside
        processOtherStmts(node, fp, 0);

        //output push
        if (out_list)
        {
            while (out_list->next)
                out_list = out_list->next;
        }

        while (out_list)
        {
            fprintf(fp, "\tpush dword [%s_%d_%d]\n", out_list->val, n->SymbolTableNode->block.lower_scope, n->SymbolTableNode->block.upper_scope);

            out_list = out_list->prev;
        }

        // fprintf(fp, "\tpop ebx\n");
        // fprintf(fp, "\tpop esi\n");
        // fprintf(fp, "\tpop edi\n");
        // fprintf(fp, "\tpop ebp\n");

        fprintf(fp, "\tpush ebp\n");

        fprintf(fp, "\tret\n\n");
    }
}

templookup *lookupIDCodegen(Table *tb, char *val)
{
    Table *parentTable = tb;

    Node *node = NULL;
    while (parentTable->container)
    {

        node = lookUpST(val, parentTable);
        if (node)
        {
            templookup *node_tb = malloc(sizeof(templookup));
            node_tb->n = node;
            node_tb->t = parentTable;

            return node_tb;
        }
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

                templookup *node_tb = malloc(sizeof(templookup));
                node_tb->n = tempnode;
                node_tb->t = parentTable;

                return node_tb;
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

                templookup *node_tb = malloc(sizeof(templookup));
                node_tb->n = tempnode;
                node_tb->t = parentTable;

                return node_tb;
            }
            out_list = out_list->next;
        }
    }
    return NULL;
}

void dynamicArrayHandle(templookup *node_t, Table *tb, FILE *fp)
{
    Leaf *lower = node_t->n->SymbolTableNode->variable.lower_index;
    Leaf *upper = node_t->n->SymbolTableNode->variable.upper_index;

    Table *parentTable = tb;
    if (lower->t == NUM) //  [NUM..high]
    {
        fprintf(fp, "\tmov eax, %d\n", lower->val->intValue);
        fprintf(fp, "\tpush eax\n");
    }
    else
    {

        templookup *n_temp = lookupIDCodegen(tb, lower->lexeme);
        parentTable = n_temp->t;

        char *tempstr = malloc(sizeof(char) * 30);
        sprintf(tempstr, "%s_%d_%d", lower->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tmov eax, [%s]\n", tempstr);
        fprintf(fp, "\tpush eax\n");
    }

    if (upper->t == NUM) //  [low..NUM]
    {
        fprintf(fp, "\tmov edi, %d\n", upper->val->intValue);
        fprintf(fp, "\tpush edi\n");
    }

    else //[low..high]
    {
        templookup *n_temp = lookupIDCodegen(tb, upper->lexeme);
        parentTable = n_temp->t;

        char *tempstr1 = malloc(sizeof(char) * 30);
        sprintf(tempstr1, "%s_%d_%d", upper->lexeme, parentTable->parent->SymbolTableNode->block.lower_scope, parentTable->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tmov edi, [%s]\n", tempstr1);
        fprintf(fp, "\tpush edi\n");
    }
    fprintf(fp, "\tcall _getarrlength\n");
}

int DynamicArrayIndexHandle(templookup *node_t, Table *tb, FILE *fp, astNode *index)
{
    Leaf *lower = node_t->n->SymbolTableNode->variable.lower_index;
    Leaf *upper = node_t->n->SymbolTableNode->variable.upper_index;

    // if (index->tokenInfo->t == NUM) // A[2]
    // {
    if (index->tokenInfo->t == NUM)
    {
        fprintf(fp, "\tmov edx, %d\n", index->tokenInfo->val->intValue);
    }
    else
    {
        templookup *node_t_temp = lookupIDCodegen(tb, index->tokenInfo->lexeme);
        Node *n_temp = node_t_temp->n;
        Table *parentTable_temp = node_t_temp->t;

        char *tempstr = malloc(sizeof(char) * 30);
        sprintf(tempstr, "%s_%d_%d", index->tokenInfo->lexeme, parentTable_temp->parent->SymbolTableNode->block.lower_scope, parentTable_temp->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tmov edx, [%s]\n", tempstr);
    }

    int start = get_next_label();

    if (lower->t == NUM)
    {
        fprintf(fp, "\tmov esi, %d\n", node_t->n->SymbolTableNode->variable.lower_index->val->intValue);
    }
    else
    {
        templookup *n_temp = lookupIDCodegen(tb, lower->lexeme);
        Table *localTable = n_temp->t;

        char *strtemp = malloc(sizeof(char) * 30);
        sprintf(strtemp, "%s_%d_%d", lower->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
        fprintf(fp, "\tmov esi, [%s]\n", strtemp);
    }

    fprintf(fp, "\tcmp edx, esi\n");
    fprintf(fp, "\tjl L_%d\n", start); // if less than lower bound

    if (upper->t == NUM)
    {
        fprintf(fp, "\tmov esi, %d\n", node_t->n->SymbolTableNode->variable.upper_index->val->intValue);
    }
    else
    {
        templookup *n_temp = lookupIDCodegen(tb, upper->lexeme);
        Table *localTable = n_temp->t;

        char *tempstr = malloc(sizeof(char) * 30);
        sprintf(tempstr, "%s_%d_%d", upper->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
        fprintf(fp, "\tmov esi, [%s]\n", tempstr);
    }

    fprintf(fp, "\tcmp edx, esi\n");
    fprintf(fp, "\tjg L_%d\n", start); //if greater than upper bound

    //comes here if range is within bound
    // fprintf(fp, "\tmov esi, %d\n", index->tokenInfo->val->intValue); //value of index is in esi
    if (index->tokenInfo->t == NUM)
    {
        fprintf(fp, "\tmov esi, %d\n", index->tokenInfo->val->intValue);
    }
    else
    {
        templookup *node_t_temp = lookupIDCodegen(tb, index->tokenInfo->lexeme);
        Node *n_temp = node_t_temp->n;
        Table *parentTable_temp = node_t_temp->t;

        char *tempstr = malloc(sizeof(char) * 30);
        sprintf(tempstr, "%s_%d_%d", index->tokenInfo->lexeme, parentTable_temp->parent->SymbolTableNode->block.lower_scope, parentTable_temp->parent->SymbolTableNode->block.upper_scope);

        fprintf(fp, "\tmov esi, [%s]\n", tempstr);
    }

    // fprintf(fp, "\tmov ebx, %d\n", n->SymbolTableNode->variable.lower_index->val->intValue);
    if (lower->t == NUM)
    {
        fprintf(fp, "\tmov ebx, %d\n", node_t->n->SymbolTableNode->variable.lower_index->val->intValue);
    }
    else
    {
        templookup *n_temp = lookupIDCodegen(tb, lower->lexeme);
        Table *localTable = n_temp->t;

        char *tempstr = malloc(sizeof(char) * 30);
        sprintf(tempstr, "%s_%d_%d", lower->lexeme, localTable->parent->SymbolTableNode->block.lower_scope, localTable->parent->SymbolTableNode->block.upper_scope);
        fprintf(fp, "\tmov ebx, [%s]\n", tempstr);
    }

    fprintf(fp, "\tsub esi, ebx\n");
    fprintf(fp, "\tmov edi, esi\n");

    return start;
}