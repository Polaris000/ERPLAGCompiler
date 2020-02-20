// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
#include "parser.h"

// #define NON_TERMINALS 55
// #define TERMINALS 58

// const char* TerminalEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", 
// 	"USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
// 	"DEFAULT", "WHILE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
// 	"COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK","ID","NUM","RNUM","ERROR"};


// typedef enum {
//     INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, GET_VALUE, PRINT, 
//     USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK,
//     DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, DRIVERDEF, DRIVERENDDEF,
//     COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, COMMENTMARK,ID,NUM,RNUM,ERROR
// }Terminal;


// typedef enum 
// {
//     program,moduleDeclarations,moduleDeclaration,otherModules, driverModule, module,
//     ret, input_plist, input_plist_dash, output_plist, output_plist_dash, dataType, range_array,
//     type, moduleDef, statements, statement, ioStmt, var, var_id_num, boolConstt, whichId,
//     simpleStmt, assignmentStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, Index, moduleReuseStmt,
//     optional, idList, idList_dash, expression, unaryOrExpr, arithmeticOrBooleanExpr, anyTerm,
//     arithmeticExpr_dash, bool, arithmeticExpr, arithmeticExpr_recur, term, term_dash, factor, 
//     op_plus_minus, op_mul_div, logicalOp, relationalOp, declareStmt, conditionalStmt, caseStmts,
//     caseStmt, value,Default,iterativeStmt, range 
// } NonTerminal;

// const char* nonTerminalEnumToString[] = {
//     "program","moduleDeclarations","moduleDeclaration","otherModules", "driverModule", "module",
//     "ret", "input_plist", "input_plist_dash", "output_plist", "output_plist_dash", "dataType", "range_array",
//     "type", "moduleDef", "statements", "statement", "ioStmt", "var", "var_id_num", "boolConstt", "whichId",
//     "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index", "moduleReuseStmt",
//     "optional", "idList", "idList_dash", "expression", "unaryOrExpr", "arithmeticOrBooleanExpr", "anyTerm",
//     "arithmeticExpr_dash", "bool", "arithmeticExpr", "arithmeticExpr_recur", "term", "term_dash", "factor", 
//     "op_plus_minus", "op_mul_div", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts",
//     "caseStmt", "value", "default", "iterativeStmt", "range"
// };

// typedef union symbol
// {
//     Terminal t;
//     NonTerminal nt;
// }Symbol;

// typedef enum 
// {
    
//     terminal,
//     nonterminal
// }Type;

// struct RHSNode
// {
//     Symbol s;
//     Type type;
//     char* token;
//     struct RHSNode * next;
// };

// typedef struct RHSNode RHSNode;

// typedef struct
// {
//     NonTerminal nt;
//     int num_rules;
//     char* start;
//     RHSNode **rules;
// }LHSNode;

// typedef struct
// {
//     int num_non_terminals;
//     LHSNode * non_terminals;
// }Grammar;


char* substring(const char *src, int m, int n)
{
	int len = n - m;
	char *dest = (char*)malloc(sizeof(char) * (len + 1));
	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
		*dest = *(src + i);
		dest++;
	}
	*dest = '\0';
	return dest - len;
}


Grammar* initialize(){
    Grammar * table=(Grammar*) malloc(sizeof(Grammar));
    table->num_non_terminals = NON_TERMINALS;

    table->non_terminals=(LHSNode*) malloc(sizeof(LHSNode) * NON_TERMINALS);
    for(int i=0;i < NON_TERMINALS;i++){
        table->non_terminals[i].num_rules=0;
        table->non_terminals[i].rules=NULL;
    }
    return table;
}

Grammar* make_table(char* filename, Grammar* table){
    FILE* ptr=fopen(filename,"r");

    char* buffer=(char*) malloc(sizeof(char)*256);
   
    while(fgets(buffer,256,ptr)!=NULL)
    {
        int length = strlen(buffer);
        buffer[length] = '$';

        int i=0,j,k;
        int start;
        int flag=0,f=0;
        RHSNode* traverse;
        while(buffer[i]!='\n')
        {
            if(f==0 && flag == 0){
                if(buffer[i]=='<'){
                    i++;
                    start=i;
                    while(buffer[i++]!='>');
                    char* token=substring(buffer,start,i-1);

                    // Search in the mapping to get the index of the string
                    for(j = 0; j < NON_TERMINALS; j++)
                    {
                        if(strcmp(nonTerminalEnumToString[j],token) == 0)
                        {
                            break;
                        }
                    }
                    table->non_terminals[j].nt = (NonTerminal)j;
                    table->non_terminals[j].start = token;
                    table->non_terminals[j].num_rules += 1;
                    table->non_terminals[j].rules=(RHSNode**) realloc(table->non_terminals[j].rules,sizeof(RHSNode*)*(table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]=(RHSNode*) malloc(sizeof(RHSNode));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->next=NULL;
                    traverse=table->non_terminals[j].rules[table->non_terminals[j].num_rules-1];
                
                    f=1;
                    flag=1;
                }
                if(buffer[i] == '$')
                    break;
                else
                    i++;
            }  // Above condition is triggered for the leftmost non-terminal of a rule

            else if(f==1 && flag==0){ 
                
                if(buffer[i]=='<'){
                    i++;
                    start=i;
                    while(buffer[i++]!='>');
                    char* token=substring(buffer,start,i-1);

                    table->non_terminals[j].num_rules+=1;
                    table->non_terminals[j].rules=(RHSNode**) realloc(table->non_terminals[j].rules,sizeof(RHSNode*)*(table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]=(RHSNode*) malloc(sizeof(RHSNode));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->next=NULL;
                    traverse=table->non_terminals[j].rules[table->non_terminals[j].num_rules-1];
                                       

                    for(k = 0; k < NON_TERMINALS; k++)
                    {
                        if(strcmp(nonTerminalEnumToString[k],token) == 0)
                        {
                            break;
                        }
                    }

                    traverse -> type = nonterminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.nt = (NonTerminal)k;                    // Assign the value of non-terminal
                    traverse -> token = token;

                    traverse -> next = (RHSNode*) malloc(sizeof(RHSNode));
                    traverse = traverse -> next;
                    traverse -> next = NULL;
                    flag=1;
                }
                else if(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z'))){
                    
                    start=i;
                    while(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z')))
                        i++;
                    char* token=substring(buffer,start,i);
            

                    table->non_terminals[j].num_rules+=1;
                    table->non_terminals[j].rules=(RHSNode**) realloc(table->non_terminals[j].rules,sizeof(RHSNode*)*(table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]=(RHSNode*) malloc(sizeof(RHSNode));

                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->next=NULL;
                    traverse=table->non_terminals[j].rules[table->non_terminals[j].num_rules-1];

                    for(k = 0; k < TERMINALS; k++)
                    {
                        if(strcmp(TerminalEnumToString[k],token) == 0)
                        {
                            break;
                        }
                    }

                    traverse -> type = terminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.t = (Terminal)k;                    // Assign the value of non-terminal
                    traverse -> token = token;


                    traverse -> next = (RHSNode*) malloc(sizeof(RHSNode));
                    traverse = traverse -> next;
                    traverse -> next = NULL;

                    flag=1;
                }
                if(buffer[i] == '$')
                    break;
                else
                    i++;
            }
            else if(f==1 && flag==1){ 
                
                if(buffer[i]=='<'){
                    i++;
                    start=i;
                    while(buffer[i++]!='>');
                    char* token=substring(buffer,start,i-1);

                    for(k = 0; k < NON_TERMINALS; k++)
                    {
                        if(strcmp(nonTerminalEnumToString[k],token) == 0)
                        {
                            break;
                        }
                    }

                    traverse -> token = token;
                    traverse -> type = nonterminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.nt = (NonTerminal)k;                    // Assign the value of non-terminal

                    traverse->next=(RHSNode*) malloc(sizeof(RHSNode));
                    traverse=traverse->next;
                    traverse->next=NULL;
                }
                
                else if(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z'))){
                    
                    start=i;
                    while(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z')))
                        i++;
                    char* token=substring(buffer,start,i);

                    for(k = 0; k < TERMINALS; k++)
                    {
                        if(strcmp(TerminalEnumToString[k],token) == 0)
                        {
                            break;
                        }
                    }

                    traverse->token=token;
                    traverse -> type = terminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.nt = (Terminal)k;                    // Assign the value of non-terminal

                    traverse->next=(RHSNode*) malloc(sizeof(RHSNode));
                    traverse=traverse->next;

                    traverse->next=NULL;
                    
                }
                else if(buffer[i]=='|')
                {
                    i++;
                    flag=0;
                }
                if(buffer[i] == '$')
                {
                    // printf("$ reached\n");
                    break;
                }
                else
                    i++;
            }
        }
        // strcpy(buffer,"");
        // free(buffer);
        // buffer=(char*) malloc(sizeof(char)*100);
    }
    return table;    
}


void printTable(Grammar* gm)
{
    printf("Printing the structure------------------------------------------------------------------------\n");
    for(int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", gm->non_terminals[i].start);
        
        for(int j = 0; j < gm->non_terminals[i].num_rules; j++)
        {
            RHSNode* traverse = gm -> non_terminals[i].rules[j];
            printf("Rule %d:\t",j+1);
            while(traverse -> next != NULL)
            {
                printf("%s\t", traverse->token);  
                traverse  = traverse -> next;
            }
            printf("\n");    
        }
        printf("\n");
        
    }
}



First* computeFirst(Grammar* G, First* first)
{
    int loop = 2;
    while(loop > 0)
    {
        for(int i = 0; i < G -> num_non_terminals; i++)
        {
            for(int j = 0; j < G -> non_terminals[i].num_rules; j++)
            {
                RHSNode* r = G -> non_terminals[i] -> rules[j];
                if(r -> type == terminal)
                {
                    first[G -> non_terminals[i] -> nt] = first[G -> non_terminals[i] -> nt] | TokenName[r -> s -> t];
                }    
                else
                {
                    first[G -> non_terminals[i] -> nt] = first[G -> non_terminals[i] -> nt] | first[r -> s -> nt];
                }
            }
        
        }  
        loop--; 
    }
    
    return first;
}


Follow* computeFollow(Grammar* G, Follow* follow )
{
    // compute follow
    return follow;
}


void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow*  F)
{
        F -> first =  computeFirst(G, F -> first);
        F -> follow = computeFollow(G, F -> follow);
}

int main(){

    Grammar* gm = initialize();
    // printf("%d\n", gm->num_non_terminals);
    gm = make_table("grammar.txt",gm);
    printTable(gm);
}