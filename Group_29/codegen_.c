#include <stdio.h>
#include <stdlib.h>


// global counts, as required
int numrelop = 0;
int numleft = -1;

void writeInit(FILE *fp, FILE *utilfp, astNode *root)
{
    
    fprintf(fp, "section .data\n\n");
        // for global variables
    fprintf(fp, "section .bss\n");

    writeData(t, fp); // from ST

    fprintf(fp, "\tbuffer	resb	120\n");
    fprintf(fp, "\tintg	resd	1\n");
    fprintf(fp, "\tisneg	resb	1\n\n");
    fprintf(fp, "\tleftchild resb  1\n");
    fprintf(fp, "\trightchild resb 1\n");
    fprintf(fp, "section .text\n");
    fprintf(fp, "\tglobal _start");
    fprintf(fp, "global _start\n");

    parseAST(root, fp);
    fprintf(fp, "call exit\n");

    writeprocedures(fp, utilfp);
    // some other data...
}

// implement writeData (using ST)
// reserve space for variables and scope
// of the form: `<var_name>_<scope>`
void writeData()
{
    ...
}

void writeprocedures(FILE *fp, FILE *utilfp)
{
    FILE *utilfp = fopen("abcd.asm", "r");
    char buffer[2500];
    int bytes = fread(buffer, 1, sizeof(buffer), utilfp);
    buffer[bytes] = '\0';
    fprintf(fp, "%s\n",buffer);
}

void writeNode(astNode *node, FILE *fp)
{
    // for non-terminals
        // var
        // expression...

    // for terminals
    // arithmetic op
    if (node -> nodename == plus_ast)
    {
        // make sure its rule 0
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "add ax, [rightchild]\n");
        numleft --;
    }
    
    if (node -> nodename == minus_ast)
    {
        // make sure its rule 0
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "sub ax, [rightchild]\n");
        numleft --;
    }

    if (node -> nodename == mul_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "mul ax, [rightchild]\n");
        numleft --;
    }

    if (node -> nodename == div_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "div ax, [rightchild]\n");
        numleft --;
    }
    
    if (node -> nodename == and_ast)
    {
        
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "and ax, [rightchild]\n");
        numleft --;
    }

    if (node -> nodename == or_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "or ax, [rightchild]\n");
        numleft --;
    }

    // relational op
    if (node -> nodename == gt_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "cmp ax, [rightchild]\n");
        fprintf(fp, "jg true_%d\n", numrelop);
        fprintf(fp, "mov ax, 0\n");
        fprintf(fp, "jmp cont_%d\n", numrelop);
        fprintf(fp, "true_%d:\n\tmov ax, 1\n" , numrelop);
        fprintf(fp, "cont_%d:\n\t", numrelop);
        numrelop ++;
    }

    if (node -> nodename == lt_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "cmp ax, [rightchild]\n");
        fprintf(fp, "jl true_%d\n", numrelop);
        fprintf(fp, "mov ax, 0\n");
        fprintf(fp, "jmp cont_%d\n", numrelop);
        fprintf(fp, "true_%d:\n\tmov ax, 1\n" , numrelop);
        fprintf(fp, "cont_%d:\n\t", numrelop);
        numrelop ++;
    }

    if (node -> nodename == ge_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "cmp ax, [rightchild]\n");
        fprintf(fp, "jg true_%d\n", numrelop);
        fprintf(fp, "mov ax, 0\n");
        fprintf(fp, "jmp cont_%d\n", numrelop);
        fprintf(fp, "true_%d: mov ax,1\n" , numrelop);
        fprintf(fp, "cont_%d:", numrelop);
        numrelop ++;
    }

    if (node -> nodename == le_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "cmp ax, [rightchild]\n");
        fprintf(fp, "jg true_%d\n", numrelop);
        fprintf(fp, "mov ax, 0\n");
        fprintf(fp, "jmp cont_%d\n", numrelop);
        fprintf(fp, "true_%d: mov ax,1\n" , numrelop);
        fprintf(fp, "cont_%d:", numrelop);
        numrelop ++;
    }

    if (node -> nodename == eq_ast)
    {
        fprintf(fp, "mov ax, [leftchild + %d]\n", numleft * 2);
        fprintf(fp, "cmp ax, [rightchild]\n");
        fprintf(fp, "jg true_%d\n", numrelop);
        fprintf(fp, "mov ax, 0\n");
        fprintf(fp, "jmp cont_%d\n", numrelop);
        fprintf(fp, "true_%d: mov ax,1\n" , numrelop);
        fprintf(fp, "cont_%d:", numrelop);
        numrelop ++;
    }

    if (node -> nodename == assign_ast)
    {

    }


}

void parseAST(astNode *a, FILE *fp)
{
    // for non-terminals
        // statements
        //var


    // for terminals
    // will require global counts
        // for
        // while
        // switch
        // getvalue
        if (a -> nodeName == ioStmt_get_value_ast)
        {
            // if num
            fprintf(fp, "call _get_user_input")
            // input stored in rsi
        }

        // print
        if (a -> nodeName == ioStmt_print_ast)
        {
            {

            }
        }
}