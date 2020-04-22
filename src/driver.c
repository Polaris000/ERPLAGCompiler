/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "semanticAnalyzer.h"
#include "symbolTable.h"
#include "codegen.h"

#include <time.h>
extern Table *baseTb;

int main(int argc, char *argv[])
{
    printf("\n\nLEVEL 4: Symbol table / Type Checking / Semantic rules modules works / handled static and dynamic arrays in type checking and code generation\n");
    printf("------------------------------------------------------------------------------------------\n\n");

    if (argc < 3)
    {
        printf(
            "USAGE\n"
            "=====\n\n"
            "The command line arguments for execution of the driver should be as follows:\n\n"
            "\t\t$./compiler   testcase.txt   code.asm\n\n");
        exit(0);
    }

    Grammar *gm = initialize();
    gm = make_table("grammar.txt", gm);
    printf("-------Grammar Loaded into the appropriate data structure------------\n\n");

    FirstAndFollow *firstFol = initializeFirstFollow();
    ComputeFirstAndFollowSets(gm, firstFol);
    printf("-------First and Follow Sets Created-------\n\n");

    ParseTable *pt = initializeParseTable();
    populateParseTable(firstFol, gm, pt);
    printf("-------Parsing Table Populated-------\n\n");

    ParseTree *pTree = NULL;
    astNode *ast = NULL;

    int code_parsed = 0;
    int num_syn_err = 0; //number of syntax error
    int num_sem_err = 0; //number of semantic error
    int astCreated = 0;

    while (1)
    {
        printf("\nPlease choose an option\n");
        printf("========================= \n\n");
        printf("0: For exiting.\n");
        printf("1: For printing the token list generated by the lexer\n");
        printf("2: For parsing to verify the syntactic correctness of the input source code and to produce parse tree\n");
        printf("3: For printing the Abstract Syntax Tree in appropriate format\n");
        printf("4: For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree\n");
        printf("5: For printing the Symbol Table\n");
        printf("6: For printing the total memory requirement for each module\n");
        printf("7: For printing the type expressions and width of array variables\n");
        printf("8: For compiling to verify the syntactic and semantic correctness of the input source code\n");
        printf("9: For producing assembly code\n");

        printf("\n\nYour Input: ");
        int option;
        scanf("%d", &option);

        printf("\n");
        printf("Selected option: %d\n", option);
        printf("----------------------\n\n");
        getchar();

        switch (option)
        {
        case 0:
            printf("Exiting...\n");
            exit(0);
            break;

        case 1:
            printTokens(argv[1]);
            break;

        case 2:
            pTree = parseInputSourceCode(argv[1], pt, gm, firstFol, &num_syn_err);

            if (pTree)
            {
                code_parsed = 1;
                printf("------------------ Parse Tree Built --------------\n\n");

                int temp;
                printf("Do you want to print the Parse Tree : '1' for YES and '0' for NO: ");
                scanf("%d", &temp);
                if (temp == 1)
                    printParseTree_inorder(pTree->root);
            }
            break;

        case 3:

            // num_syn_err = 0;
            // num_sem_err = 0;
            // pTree = parseInputSourceCode(argv[1], pt, gm, firstFol, &num_syn_err);
            if (code_parsed == 0)
                printf("\n-----------Parse Tree not yet created. Select option 2 first ----------------\n");
            else
            {
                if (num_syn_err)
                    printf("\n\nAST can't be constructed since the code has Syntax Errors\n\n");
                else
                {
                    ast = postOrder_ParseTree(pTree);
                    printf("\n\n-----------------------------------Printing AST (Preorder Traversal)----------------------------------\n\n");
                    printAst(ast);
                    printf("\n\n----------------------------AST Printed in Preorder Fashion (Node first, and then children)----------------------------\n\n");

                    astCreated = 1;
                }
            }
            break;

        case 4:
        {
            if (pTree == NULL || ast == NULL)
            {
                printf("\n--------ParseTree or AST not built yet------------\n");
            }
            else
            {
                Compression_Ratio_calc(pTree->root, ast);
            }
        }
        break;

        case 5:
            if (baseTb == NULL)
            {

                if (pTree == NULL)
                    pTree = parseInputSourceCode(argv[1], pt, gm, firstFol, &num_syn_err);
                if (ast == NULL)
                    ast = postOrder_ParseTree(pTree);
                error_list *errors = init_errors();
                Table *tb = populateSymbolTable(ast, errors);
                printSymbolTable(tb);
            }
            else
            {
                printSymbolTable(baseTb);
            }

            break;

        case 6:
        {
            if (baseTb == NULL)
            {

                printf("\n------------SymbolTable not yet built. Select option 5 first --------------\n");
            }

            else
            {
                calcMemoryofFunctions(baseTb);
            }
        }
        break;

        case 7:
            if (baseTb == NULL)
            {
                if (pTree == NULL)
                    pTree = parseInputSourceCode(argv[1], pt, gm, firstFol, &num_syn_err);
                if (ast == NULL)
                    ast = postOrder_ParseTree(pTree);
                error_list *errors = init_errors();
                Table *tb = populateSymbolTable(ast, errors);
                printArrays(tb);
                printf("\n------------SymbolTable not yet built. Select Option 5 first --------------\n");
            }
            else
            {
                // printf("Hello\n");
                printArrays(baseTb);
            }

            break;

        case 8:
        {
            num_syn_err = 0;
            num_sem_err = 0;

            clock_t start_time, end_time;

            double total_CPU_time, total_CPU_time_in_seconds;

            start_time = clock();

            if (pTree == NULL)
                pTree = parseInputSourceCode(argv[1], pt, gm, firstFol, &num_syn_err);
            if (pTree)
            {
                if (num_syn_err)
                    printf("\n\n------------------------Syntax Errors Reported-----------------------\n\n");
                else
                {
                    printf("\n\n---------------------------Semantic Analysis Begins---------------------------\n\n");
                    if (ast == NULL)
                        ast = postOrder_ParseTree(pTree);
                    error_list *errors = init_errors();
                    Table *tb = populateSymbolTable(ast, errors);
                    traverse_AST(ast, errors);

                    if (errors->head != NULL)
                    {
                        num_sem_err = errors->err_total;
                        printf("\n\n------------------------Semantic Errors Reported -------------------------\n");
                        printSemanticErrors(errors);
                    }

                    else
                    {
                        printf("\n\n------------------No syntax or semantic errors----------------\n\n");
                    }
                }
            }
            end_time = clock();
            printf("\n");
            printf("Execution Summary\n");
            printf("=================\n");
            printf("Start Time: \t\t\t %lf\n", (double)start_time);
            printf("End Time: \t\t\t %lf\n", (double)end_time);

            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / (double)CLOCKS_PER_SEC;

            printf("Total CPU time: \t\t %8.10f\n", total_CPU_time);
            printf("Total CPU time (in sec): \t %8.10f \n", total_CPU_time_in_seconds);
        }
        break;

        case 9:
        {
            num_syn_err = 0;
            num_sem_err = 0;

            if (pTree == NULL)
                pTree = parseInputSourceCode(argv[1], pt, gm, firstFol, &num_syn_err);
            if (pTree)
            {
                if (num_syn_err)
                    printf("\n\n------------------------Syntax Errors Reported-----------------------\n\n");
                else
                {
                    printf("\n\n---------------------------Semantic Analysis Begins---------------------------\n\n");
                    if (ast == NULL)
                        ast = postOrder_ParseTree(pTree);
                    error_list *errors = init_errors();
                    Table *tb = populateSymbolTable(ast, errors);
                    traverse_AST(ast, errors);

                    if (errors->head != NULL)
                    {
                        num_sem_err = errors->err_total;
                        printSemanticErrors(errors);
                        printf("\n\n------------------------Semantic Errors Reported : Unsuccessful Compilation-------------------------\n\n");
                    }

                    else
                    {
                        printf("\n\n------------------Compilation Successful----------------\n\n");
                        generate_assembly_code(ast, argv[2]);
                        printf("\n\n-------------------ASM Code Generated----------------\n");
                    }
                }
            }
        }
        break;

        default:
            printf("Invalid option provided....\n");
            continue;
        }
    }
}
