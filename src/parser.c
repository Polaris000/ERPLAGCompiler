
#include "parser.h"
// #include "lexer.h"
// #include "stack.h"


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

// Trie structure for obtaining the enum from string

node* initialize_node(){
    node* n=(node*) malloc(sizeof(node));
    for(int i=0;i<28;i++){
        n->next[i]=NULL;
    }
    return n;
}

trie* initialize_trie(){
    trie* t=(trie*) malloc(sizeof(trie));
    t->num=0;
    t->children=initialize_node();
    t->children->data.c='$';
    return t;
}

int lookup(trie* t,char* w){
    char word[strlen(w)];
    strcpy(word,w);
    for(int i=0;i<strlen(word);i++){
        if((word[i]>='A')&&(word[i]<='Z'))
            word[i]=tolower(word[i]);
    }
    // printf("%s\n",word);
    int result;
    int i=0;
    node* traverse=t->children;
    int index;

    while(word[i]!='\0'){

        if((word[i]>='a')&&(word[i]<='z'))
            index=word[i]-'a';
        else
            index=26;
        if(traverse->next[index]==NULL)
            return -1;

        traverse=traverse->next[index];
        i++;
    }

    if(traverse->next[27]==NULL)
        return -1;

    traverse=traverse->next[27];
    result=traverse->data.pos;
    return result;
}

void insert_trie(trie* t,char* w,int key){
    // printf("Hello\n");
    char word[strlen(w)];
    strcpy(word,w);
    // printf("%d\n",strlen(word));
    for(int i=0;i<strlen(word);i++){
        // printf("%c\n",word[i]);
        if((word[i]>='A')&&(word[i]<='Z'))
            word[i]=tolower(word[i]) ;
        // printf("%c\n",word[i]);
    }
    // printf("Hello\n");
    // printf("%s\n",word);
    t->num++;
    int i=0;
    node* traverse=t->children;
    int index;

    while(word[i]!='\0'){
        if((word[i]>='a')&&(word[i]<='z'))
            index=word[i]-'a';
        else
            index=26;
        if(traverse->next[index]==NULL){
            traverse->next[index]=initialize_node();
            traverse->next[index]->data.c=word[i];
        }
        // printf("%c\t",word[i]);
        // printf("\n");
        traverse=traverse->next[index];
        i++;
    }
    // printf("%c\n",word[i]);
    traverse->next[27]=initialize_node();
    traverse=traverse->next[27];
    traverse->data.pos=key;
    // printf("%d\n",traverse->data.pos);
}

trie* populate_trie_nonterminals(trie* t,char** nonTerminalEnumToString){

    for(int i=0;i<NON_TERMINALS;i++){
        // printf("%s\n",nonTerminalEnumToString[i]);
        insert_trie(t,nonTerminalEnumToString[i],i);
    }
    return t;
}

trie* populate_trie_terminals(trie* t,char** TerminalEnumToString){

    for(int i=0;i<TERMINALS;i++){
        // printf("%s\n",nonTerminalEnumToString[i]);
        insert_trie(t,TerminalEnumToString[i],i);
    }
    return t;
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

    //Initialize and populate the trie
    trie* t1 = initialize_trie();
    t1 = populate_trie_nonterminals(t1,nonTerminalEnumToString);
    trie* t2 = initialize_trie();
    t2 = populate_trie_terminals(t2,TerminalEnumToString);
    
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

                    // Search in the trie to get the index of the string
                    j=lookup(t1,token);

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
                                       

                    k=lookup(t1,token);

                    traverse -> type = nonterminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.nt = (NonTerminal)k;                    // Assign the value of non-terminal


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


                    k=lookup(t2,token);
                    traverse -> type = terminal;                       // Assign the token an enum of type non-terminal
                    traverse -> s.t = (Terminal)k;                    // Assign the value of non-terminal


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


                    k=lookup(t1,token);
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
                    
                    k=lookup(t2,token);
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
                    break;
                }
                else
                    i++;
            }
        }
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
// First and Follow sets computation


FirstAndFollow* initializeFirstFollow()
{
    FirstAndFollow* first_follow = (FirstAndFollow*) malloc(sizeof(FirstAndFollow));

    first_follow -> first = (First*)malloc(sizeof(First)*NON_TERMINALS);
    first_follow -> follow = (Follow*)malloc(sizeof(Follow)*NON_TERMINALS);

    for(int i = 0; i < NON_TERMINALS; i++)
    {
        first_follow -> first[i].firstSet = (int *)calloc(ceil(TERMINALS/32),sizeof(int));
        first_follow -> follow[i].followSet = (int *)calloc(ceil(TERMINALS/32),sizeof(int));
    }
    return first_follow;
}


int checkPrevComputed(FirstAndFollow* F, NonTerminal nt)
{
    for(int j = 0; j < TERMINALS; j++)
    {
        if(TestBit(F->follow[nt].followSet,(Terminal)j))
        {
            return 1;
        }
    }

    return 0;
}

void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow*  F)
{
    for(int i = 0; i < NON_TERMINALS; i++)
    {
        computeFirst(G, F , G -> non_terminals[i].nt);
    }

    setBit(F->follow[0].followSet,DOLLAR);  //Include DOLLAR in follow of start symbol
    for(int i = 1; i < NON_TERMINALS; i++)
    {

        if(!checkPrevComputed(F, (NonTerminal)i))
        {
            computeFollow(G,F,G -> non_terminals[i].nt);
        }
    }

}


int* computeFollow(Grammar* G, FirstAndFollow* F, NonTerminal nt)
{
    for(int i = 0; i < NON_TERMINALS; i++)
    {
        for(int j = 0; j < G->non_terminals[i].num_rules; j++)
        {
            RHSNode* rhs = G -> non_terminals[i].rules[j];

            while(rhs -> next != NULL)
            {
                if(rhs -> type == nonterminal && rhs -> s.nt == nt)
                {
                    rhs = rhs -> next;
                    if(rhs -> next != NULL)
                    {
                        while(rhs -> next != NULL)
                        {
                            if(rhs -> type == terminal)
                            {
                                setBit(F->follow[nt].followSet,rhs->s.t);
                                break;
                            }
                            else
                            {
                                for(int k = 0; k < TERMINALS-1; k++)
                                {
                                    if(TestBit(F->first[rhs->s.nt].firstSet,(Terminal)k))
                                    {
                                        setBit(F->follow[nt].followSet,(Terminal)k);
                                        // printf("%s\n", TerminalEnumToString[(Terminal)k]);
                                    }
                                }
                                if(TestBit(F->first[rhs->s.nt].firstSet,EPSILON) == 0)
                                    break;
                            }
                            rhs = rhs -> next;
                            if(rhs -> next == NULL)
                            {
                                // printf("Hello\n");
                                if(checkPrevComputed(F, G->non_terminals[i].nt))
                                {
                                    // printf("Hello\n");
                                    F->follow[nt].followSet = setUnion(F->follow[G->non_terminals[i].nt].followSet,F->follow[nt].followSet);
                                    // printf("Hi\n");
                                }
                                else
                                {
                                    // printf("Here\n");
                                    F->follow[nt].followSet = setUnion(computeFollow(G,F,G->non_terminals[i].nt),F->follow[nt].followSet);
                                }
                            }
                        }
                    }
                    else
                    {
                        if(nt == G -> non_terminals[i].nt)
                            continue;
                        else
                            F->follow[nt].followSet = setUnion(computeFollow(G,F,G->non_terminals[i].nt),F->follow[nt].followSet);
                    }

                }
                rhs = rhs -> next;
                if(rhs == NULL)
                    break;
            }
        }
    }
    return (F->follow[nt].followSet);
}


int* computeFirst(Grammar* G, FirstAndFollow* F, NonTerminal nt)
{

    for(int i = 0; i < G -> non_terminals[nt].num_rules; i++)
    {
        RHSNode* rhs = G -> non_terminals[nt].rules[i];

        while(rhs -> next != NULL)
        {
            if(rhs -> type == terminal)
            {
                setBit(F -> first[nt].firstSet,rhs -> s.t);
                break;
            }
            else
            {

                F -> first[nt].firstSet = setUnion(computeFirst(G,F,rhs -> s.nt),F->first[nt].firstSet);
                if(TestBit(F->first[rhs -> s.nt].firstSet, EPSILON) == 0)
                    break;
            }
            rhs = rhs -> next;
        }
    }
    return (F->first[nt].firstSet);
}


void setBit(int* firstSet, Terminal t)
{
    int k = (int) t;        
    int pos = k % 32;          

    unsigned int flag = 1;  

    flag = flag << pos;                           // (shifted k positions)

    firstSet[k/32] = firstSet[k/32] | flag;      // Set the bit at the k-th position in firstSet[i]
}

int TestBit(int* firstSet, Terminal t)
{
    int k = (int)t;

    if ( (firstSet[k/32] & (1 << (k%32) )) != 0 )       
        return 1;         // k-th bit is 1
    else
        return 0;         // k-th bit is 0
}

int* setUnion(int* firstSet1, int* firstSet2)
{
    for(int i = 0; i < TERMINALS-1; i++)     // -1 is for not including EPSILON as it is the last item in our terminals enum
    {
        if(TestBit(firstSet1,(Terminal)i))
        {
            setBit(firstSet2,(Terminal)i);
        }
    }
    return firstSet2;
}

void printFirstSet(FirstAndFollow* F)
{
    printf("Printing the First Set*************************************************************************\n");

    for(int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[i]);

        for(int j = 0; j < TERMINALS; j++)
        {
            if(TestBit(F->first[i].firstSet,(Terminal)j))
            {
                printf("%s\t", TerminalEnumToString[j]);
            }
        }
        printf("\n");
    }

    printf("Printing Finished*****************************************************************************\n\n\n\n");
}

void printFollowSet(FirstAndFollow* F)
{
    printf("Printing the Follow Set*************************************************************************\n");

    for(int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[i]);

        for(int j = 0; j < TERMINALS; j++)
        {
            if(TestBit(F->follow[i].followSet,(Terminal)j))
            {
                printf("%s\t", TerminalEnumToString[j]);
            }
        }
        printf("\n");
    }

    printf("Printing Finished*****************************************************************************\n");
}
// ParseTable Construction---------------------------------------------------------------------------------------------------------------

ParseTable* initializeParseTable()
{
    ParseTable* pt = (ParseTable*)malloc(sizeof(ParseTable));

    for(int i = 0; i < NON_TERMINALS; i++)
    {
        memset(pt->Parse[i],-1,sizeof(int)*TERMINALS);
    }
    return pt;   
}

void populateParseTable(FirstAndFollow*F, Grammar *G, ParseTable * pt)
{
    for(int i = 0; i < NON_TERMINALS; i++)
    {
        LHSNode lhs = G -> non_terminals[i];
        for (int j = 0; j < lhs.num_rules; j++)
        {
            RHSNode *rhs = lhs.rules[j];
            int flag = 0;

            // case1: terminal but not EPSILON
            if (rhs -> type == terminal && rhs -> s.t != EPSILON)
            {
                pt -> Parse[i][rhs->s.t].rule_no = j;
            }

            //case2: EPSILON
            else if(rhs -> s.t == EPSILON)
            {
                for(int k = 0; k < TERMINALS; k++)
                {
                    if(TestBit(F->follow[lhs.nt].followSet,(Terminal)k))
                    {
                        pt -> Parse[i][k].rule_no = j;
                    }
                }
            }

            //case3: Non-Terminal
            else
            {
                while(rhs -> next != NULL)
                {
                    flag = 0;   // it becomes 1 if first(rhs -> s.nt) contains EPSILON
                    if(rhs -> type == terminal)  //this condition will be useful onwards 2nd iteratons
                    {
                        pt -> Parse[i][rhs->s.t].rule_no = j;
                        break;
                    }

                    for(int k = 0; k < TERMINALS; k++)
                    {
                        if(TestBit(F->first[rhs -> s.nt].firstSet,(Terminal)k))
                        {
                            if(k == (int)EPSILON)
                            {
                                flag = 1;
                                continue;
                            }
                            pt -> Parse[i][k].rule_no = j;
                        }
                    }

                    if(flag)  // first(rhs -> s.nt) contains EPSILON. So place the rule in the follow of lhs->nt
                    {
                        rhs = rhs -> next;
                    }
                    else
                    {
                        break;
                    }
                }
                if(flag)
                {
                    for(int k = 0; k < TERMINALS; k++)
                    {
                        if(TestBit(F->follow[lhs.nt].followSet,(Terminal)k))
                        {
                            pt -> Parse[i][k].rule_no = j;
                        }
                    }
                }

            }
        }
    }
    
}

void printParseTable(ParseTable* pt)
{
    for(int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[i]);
        for(int j = 0; j < TERMINALS; j++)
        {
            if(pt->Parse[i][j].rule_no != -1)
            printf("%s -- %d\t", TerminalEnumToString[j],pt->Parse[i][j].rule_no);
        }
        printf("\n");
    }
}


//Parse the input source code and build the parse tree

// void parseInputSourceCode(char * testCaseFile, ParseTable* table, Grammar* gm)
// {
//     //Initialize the lexer by giving the file input


//     //initialize stack
//     Stack* st = initialize_stack();
//     // push DOLLAR and Start non-terminals into the stack
//     push(st,terminal,DOLLAR);
//     push(st,nonterminal,program);

//     // Get the first token from lexer
//     Token* tk = getNextToken(fp);

//     while(tk != NULL)
//     {
//         StackNode* snode = top(st);

//         int index_lhs_grm = (int)(snode -> s.nt);
//         int rule_no = table[index_lhs_grm][tk -> name];    // this will give me the rule number corresponding to the given stack top and current token given by lexer

//         if(rule_no == -1)
//         {
//             // Generate syntax error

//         }
        
//         RhsNode* rhs = gm -> non-terminals[index_lhs_grm].rules[rule_no];


//     }
// }


int main(){

    Grammar* gm = initialize();

    //Populate Grammar and Print it
    gm = make_table("grammar.txt",gm);
    printTable(gm);

    //Compute first and follow and print them
    FirstAndFollow* firstFol = initializeFirstFollow();
    ComputeFirstAndFollowSets(gm,firstFol);
    printFirstSet(firstFol);
    printFollowSet(firstFol);

    //Populate Parse Tree and print it
    ParseTable* pt = initializeParseTable();
    populateParseTable(firstFol,gm,pt);
    printParseTable(pt);
}