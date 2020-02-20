// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
#include "parser.h"




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
                    // table->non_terminals[j].start = token;
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
                    // traverse -> token = token;

                    traverse -> next = (RHSNode*) malloc(sizeof(RHSNode));
                    traverse = traverse -> next;
                    traverse -> next = NULL;
                    flag=1;
                }
                else if(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z')) || buffer[i] == '_'){

                    start=i;
                    while(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z')) || buffer[i] == '_')
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
                    // traverse -> token = token;


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

                    // traverse -> token = token;
                    traverse -> type = nonterminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.nt = (NonTerminal)k;                    // Assign the value of non-terminal

                    traverse->next=(RHSNode*) malloc(sizeof(RHSNode));
                    traverse=traverse->next;
                    traverse->next=NULL;
                }

                else if(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z')) || buffer[i] == '_'){

                    start=i;
                    while(((buffer[i]>='a')&&(buffer[i]<='z'))||((buffer[i]>='A')&&(buffer[i]<='Z')) || buffer[i] == '_')
                        i++;
                    char* token=substring(buffer,start,i);

                    for(k = 0; k < TERMINALS; k++)
                    {
                        if(strcmp(TerminalEnumToString[k],token) == 0)
                        {
                            break;
                        }
                    }

                    // traverse->token=token;
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
        // printf("%s\n", gm->non_terminals[i].start);
        printf("%s\n", nonTerminalEnumToString[gm -> non_terminals[i].nt]);

        for(int j = 0; j < gm->non_terminals[i].num_rules; j++)
        {
            RHSNode* traverse = gm -> non_terminals[i].rules[j];
            printf("Rule %d:\t",j+1);
            while(traverse -> next != NULL)
            {
                // printf("%s\t", traverse->token);
                if(traverse -> type == terminal)
                    printf("%s\t", TerminalEnumToString[traverse -> s.t]);
                else
                    printf("%s\t", nonTerminalEnumToString[traverse -> s.nt]);
                traverse  = traverse -> next;
            }
            printf("\n");
        }
        printf("\n");

    }
}


//------------------------------------------------------------------------------------------------------------------------------------
// Following part s deal with the first and follow sets computation


// FirstAndFollow* initializeFirstFollow()
// {
//     FirstAndFollow* first_follow = (FirstAndFollow*) malloc(sizeof(FirstAndFollow));

//     for(int i = 0; i < NON_TERMINALS; i++)
//     {
//         first_follow -> first[i] = (int *)malloc(sizeof(int)*ceil(TERMINALS/32));
//         first_follow -> follow[i] = (int *)malloc(sizeof(int)*ceil(TERMINALS/32));
//     }
//     return first_follow;
// }

// void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow*  F)
// {
//     for(int i = 0; i < NON_TERMINALS; i++)
//     {
//         computeFirst(G, F -> first[i], G -> non_terminals[i] -> nt);
//     }
//     F -> follow = computeFollow(G, F -> follow);
// }


// Follow* computeFollow(Grammar* G, Follow* follow )
// {
//     // compute follow
//     return follow;
// }


// First* computeFirst(Grammar* G, int* first, NonTerminal nt)
// {
//     int loop = 2;
//     while(loop > 0)
//     {
//         for(int i = 0; i < G -> num_non_terminals; i++)
//         {
//             for(int j = 0; j < G -> non_terminals[i].num_rules; j++)
//             {
//                 RHSNode* r = G -> non_terminals[i] -> rules[j];
//                 if(r -> type == terminal)
//                 {
//                     first[G -> non_terminals[i] -> nt] = first[G -> non_terminals[i] -> nt] | TokenName[r -> s -> t];
//                 }
//                 else
//                 {
//                     first[G -> non_terminals[i] -> nt] = first[G -> non_terminals[i] -> nt] | first[r -> s -> nt];
//                 }
//             }

//         }
//         loop--;
//     }

//     return first;
// }


// void setBit(int* firstSet, Terminal t)
// {
//     int k = (int) t;
//     int i = k / 32;
//     int pos = k % 32;

//     unsigned int flag = 1;

//     flag = flag << pos;     // (shifted k positions)

//     firstSet[i] = firstSet[i] | flag;      // Set the bit at the k-th position in firstSet[i]
// }

int *arr getsetbits(int *x)
{

}

int has(int *arr, int length, int term)
{
    for (int i = length - 1; i > -1; i --)
        if (arr[i] == term)
            return 1;

    return 0;
}

ParseTable* populateParseTable(FirstandFollow*F, Grammar *G, ParseTable * pt)
{
    for (int i = 0; i < NON_TERMINALS, i ++)
    {
        LHSNode *lhs = G -> non_terminals[i];
        for (int j = 0; j < lhs -> num_rules; j ++)
        {
            RHSNode *rhs = lhs -> rules[j];

            int setbits[] = getsetbits(First[rhs -> s . nt]);
            int num_terminals = sizeof(setbits) / sizeof(int);

            // case1: termianl
            if (rhs -> type == terminal)
            {
                *(*(pt + i) + j) = j;
                continue;
            }

            // case2: $ and epsilon
        
            if (has(setbits, num_terminals, EPSILON) && has(setbits_follow, num_terminals, DOLLAR))
            {
                    int setbits_follow = getsetbits(Follow[lhs -> nt]);
                    for (int k = 0; k < setbits_follow; k ++)
                    {
                        *(*(pt + i) + DOLLAR) = j;
                    }
            }

            // case3: epsilon
			if (has(setbits, num_terminals, EPSILON))
			{
					int setbits_follow = getsetbits(Follow[lhs -> nt]);
                    for (int k = 0; k < setbits_follow; k ++)
                    {
                        *(*(pt + i) + setbits[k]) = j;
                    }
			}
        }
    }
    return pt;
}



int main(){

    Grammar* gm = initialize();
    // printf("%d\n", gm->num_non_terminals);
    gm = make_table("grammar.txt",gm);
    printTable(gm);
}
