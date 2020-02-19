#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// const char* tokenNameEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", 
// 	"USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
// 	"DEFAULT", "WHILE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
// 	"COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK","ID","NUM","RNUM","ERROR"};

// typedef enum {
// 	INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, GET_VALUE, PRINT, 
// 	USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK,
// 	DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, DRIVERDEF, DRIVERENDDEF,
// 	COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, COMMENTMARK,ID,NUM,RNUM,ERROR
// }TokenName;

// typedef enum{
//     program,
// }non_terminals;

typedef struct RHS{
    char* token;
    struct RHS* next;
}RHS;

typedef struct LHS{
    char* start;
    int num_rules;
    RHS** rules;
}LHS;

typedef struct Grammar
{
    int num_non_terminals;
    LHS* non_terminals;
}Grammar;

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
    table->num_non_terminals=4;
    // printf("%d\n",table->num_non_terminals );
    table->non_terminals=(LHS*) malloc(sizeof(LHS)*table->num_non_terminals);
    for(int i=0;i<table->num_non_terminals;i++){
        table->non_terminals[i].num_rules=0;
        table->non_terminals[i].rules=NULL;
        table->non_terminals[i].start = "";
    }
    return table;
}

Grammar* make_table(char* filename, Grammar* table){
    FILE* ptr=fopen(filename,"r");

    char* buffer=(char*) malloc(sizeof(char)*100);
   
    while(fgets(buffer,100,ptr)!=NULL){
        // printf("%s\n", buffer);
        int i=0,j=0;
        int start;
        int flag=0;
        RHS* traverse;
        while(buffer[i]!='\n'){
         
            if(flag==0){

                if(buffer[i]=='<'){
                    i++;
                    start=i;
                    while(buffer[i++]!='>');
                    char* token=substring(buffer,start,i-1);
                    // printf("%s\n",token );
                    // printf("%d\n", table->num_non_terminals);
                    // printf("Hello\n");
                    for(j=0;j<table->num_non_terminals;j++){
                        // printf("%s\n",table->non_terminals[j].start);
                        if(strcmp(table->non_terminals[j].start,token)==0)
                            break;
                    }
                    // printf("Hello\n");
                    if(j==table->num_non_terminals){
                        table->non_terminals=(LHS*) realloc(table->non_terminals,sizeof(LHS)*(table->num_non_terminals+1));
                        table->num_non_terminals+=1;
                        table->non_terminals[j].start=token;
                        table->non_terminals[j].num_rules=0;
                        table->non_terminals[j].rules=NULL;
                    }

                    table->non_terminals[j].num_rules+=1;
                    table->non_terminals[j].rules=(RHS**) realloc(table->non_terminals[j].rules,sizeof(RHS*)*(table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]=(RHS*) malloc(sizeof(RHS));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->token=NULL;
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->next=NULL;
                    traverse=table->non_terminals[j].rules[table->non_terminals[j].num_rules-1];
                
                    flag=1;
                }

                else if((buffer[i]>='a'&&buffer[i]<='z')||(buffer[i]>='A'&&buffer[i]<='Z')){

                    start=i;
                    while((buffer[i]>='a'&&buffer[i]<='z')||(buffer[i]>='A'&&buffer[i]<='Z'))
                        i++;
                    char* token=substring(buffer,start,i);

                    table->non_terminals[j].num_rules+=1;
                    table->non_terminals[j].rules=(RHS**) realloc(table->non_terminals[j].rules,sizeof(RHS*)*(table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]=(RHS*) malloc(sizeof(RHS));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->token=NULL;
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules-1]->next=NULL;
                    traverse=table->non_terminals[j].rules[table->non_terminals[j].num_rules-1];

                    flag=1;
                }
                else
                    i++;
            }
            else{ 
                
                if(buffer[i]=='<'){
                    i++;
                    start=i;
                    while(buffer[i++]!='>');
                    char* token=substring(buffer,start,i-1);

                    traverse->token=token;
                    traverse->next=(RHS*) malloc(sizeof(RHS));
                    traverse=traverse->next;
                    traverse->token=NULL;
                    traverse->next=NULL;
                    
                }
                
                else if((buffer[i]>='a'&&buffer[i]<='z')||(buffer[i]>='A'&&buffer[i]<='Z')){
                    
                    start=i;
                    while((buffer[i]>='a'&&buffer[i]<='z')||(buffer[i]>='A'&&buffer[i]<='Z'))
                        i++;
                    char* token=substring(buffer,start,i);

                    traverse->token=token;
                    traverse->next=(RHS*) malloc(sizeof(RHS));
                    traverse=traverse->next;
                    traverse->token=NULL;
                    traverse->next=NULL;
                    
                }
                else if(buffer[i]=='|')
                {
                    i++;
                    flag=0;
                }
                else
                    i++;
            }

        }
        
    }
    return table;    
}
struct sample{
    char* token;
};

void printTable(Grammar* gm)
{
    for(int i = 0; i < gm-> num_non_terminals; i++)
    {
        printf("%s\n", gm->non_terminals[i].start);
        
        for(int j = 0; j < gm->non_terminals[i].num_rules; j++)
        {
            RHS* traverse = gm -> non_terminals[i].rules[j];
            while(traverse->token != NULL)
            {
                printf("%s\t", traverse->token);  
                traverse  = traverse -> next;
            }
            printf("\n");    
        }
        
    }
}

int main(){
    // struct sample* ptr1=malloc(sizeof(struct sample));
    // struct sample* ptr2=ptr1;
    // ptr2->token="Hello";
    // printf("%s\n",ptr1->token);
    Grammar* gm = initialize();
    // printf("%d\n", gm->num_non_terminals);
    gm = make_table("grammar.txt",gm);
    printTable(gm);
}