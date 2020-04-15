/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/

#include "parser.h"
// #include "stack.h"

char *nonTerminalEnumToString[] = {
    "program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module",
    "ret", "input_plist", "input_plist_dash", "output_plist", "output_plist_dash", "dataType", "range_array",
    "type", "moduleDef", "statements", "statement", "ioStmt", "var", "var_id_num", "boolConstt", "whichId",
    "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index", "moduleReuseStmt",
    "optional", "idList", "idList_dash", "expression", "unaryOrExpr", "arithmeticOrBooleanExpr", "anyTerm",
    "arithmeticExpr_dash", "bool", "arithmeticExpr", "arithmeticExpr_recur", "term", "term_dash", "factor",
    "op_plus_minus", "op_mul_div", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts",
    "caseStmt", "value", "default", "iterativeStmt", "range", "ROOT"};

char *TerminalEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT",
                                "USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
                                "DEFAULT", "WHILE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
                                "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK", "ID", "NUM", "RNUM", "ERROR", "$", "e"};

extern int endOfFile;
extern int currLine;

char *substring(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}

// Trie structure for obtaining the enum from string

node *initialize_node()
{
    node *n = (node *)malloc(sizeof(node));
    for (int i = 0; i < 28; i++)
    {
        n->next[i] = NULL;
    }
    return n;
}

trie *initialize_trie()
{
    trie *t = (trie *)malloc(sizeof(trie));
    t->num = 0;
    t->children = initialize_node();
    t->children->data.c = '$';
    return t;
}

int lookUp(trie *t, char *w)
{
    char word[strlen(w)];
    strcpy(word, w);
    for (int i = 0; i < strlen(word); i++)
    {
        if ((word[i] >= 'A') && (word[i] <= 'Z'))
            word[i] = tolower(word[i]);
    }
    // printf("%s\n",word);
    int result;
    int i = 0;
    node *traverse = t->children;
    int index;

    while (word[i] != '\0')
    {

        if ((word[i] >= 'a') && (word[i] <= 'z'))
            index = word[i] - 'a';
        else
            index = 26;
        if (traverse->next[index] == NULL)
            return -1;

        traverse = traverse->next[index];
        i++;
    }

    if (traverse->next[27] == NULL)
        return -1;

    traverse = traverse->next[27];
    result = traverse->data.pos;
    return result;
}

void insert_trie(trie *t, char *w, int key)
{

    char word[strlen(w)];
    strcpy(word, w);

    for (int i = 0; i < strlen(word); i++)
    {
        if ((word[i] >= 'A') && (word[i] <= 'Z'))
            word[i] = tolower(word[i]);
    }
    t->num++;
    int i = 0;
    node *traverse = t->children;
    int index;

    while (word[i] != '\0')
    {
        if ((word[i] >= 'a') && (word[i] <= 'z'))
            index = word[i] - 'a';
        else
            index = 26;
        if (traverse->next[index] == NULL)
        {
            traverse->next[index] = initialize_node();
            traverse->next[index]->data.c = word[i];
        }
        traverse = traverse->next[index];
        i++;
    }
    traverse->next[27] = initialize_node();
    traverse = traverse->next[27];
    traverse->data.pos = key;
}

trie *populate_trie_nonterminals(trie *t, char **nonTerminalEnumToString)
{

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        // printf("%s\n",nonTerminalEnumToString[i]);
        insert_trie(t, nonTerminalEnumToString[i], i);
    }
    return t;
}

trie *populate_trie_terminals(trie *t, char **TerminalEnumToString)
{

    for (int i = 0; i < TERMINALS; i++)
    {
        // printf("%s\n",nonTerminalEnumToString[i]);
        insert_trie(t, TerminalEnumToString[i], i);
    }
    return t;
}

Grammar *initialize()
{
    Grammar *table = (Grammar *)malloc(sizeof(Grammar));
    table->num_non_terminals = NON_TERMINALS;

    table->non_terminals = (LHSNode *)malloc(sizeof(LHSNode) * NON_TERMINALS);
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        table->non_terminals[i].num_rules = 0;
        table->non_terminals[i].rules = NULL;
    }
    return table;
}

Grammar *make_table(char *filename, Grammar *table)
{
    FILE *ptr = fopen(filename, "r");

    //Initialize and populate the trie
    trie *t1 = initialize_trie();
    t1 = populate_trie_nonterminals(t1, nonTerminalEnumToString);
    trie *t2 = initialize_trie();
    t2 = populate_trie_terminals(t2, TerminalEnumToString);

    char *buffer = (char *)malloc(sizeof(char) * 256);

    while (fgets(buffer, 256, ptr) != NULL)
    {
        int length = strlen(buffer);
        buffer[length] = '$';

        int i = 0, j, k;
        int start;
        int flag = 0, f = 0;
        RHSNode *traverse;
        while (buffer[i] != '\n')
        {
            if (f == 0 && flag == 0)
            {
                if (buffer[i] == '<')
                {
                    i++;
                    start = i;
                    while (buffer[i++] != '>')
                        ;
                    char *token = substring(buffer, start, i - 1);

                    // Search in the trie to get the index of the string
                    j = lookUp(t1, token);

                    table->non_terminals[j].nt = (NonTerminal)j;
                    table->non_terminals[j].num_rules += 1;
                    table->non_terminals[j].rules = (RHSNode **)realloc(table->non_terminals[j].rules, sizeof(RHSNode *) * (table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1] = (RHSNode *)malloc(sizeof(RHSNode));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1]->next = NULL;
                    traverse = table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1];

                    f = 1;
                    flag = 1;
                }
                if (buffer[i] == '$')
                    break;
                else
                    i++;
            } // Above condition is triggered for the leftmost non-terminal of a rule

            else if (f == 1 && flag == 0)
            {

                if (buffer[i] == '<')
                {
                    i++;
                    start = i;
                    while (buffer[i++] != '>')
                        ;
                    char *token = substring(buffer, start, i - 1);

                    table->non_terminals[j].num_rules += 1;
                    table->non_terminals[j].rules = (RHSNode **)realloc(table->non_terminals[j].rules, sizeof(RHSNode *) * (table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1] = (RHSNode *)malloc(sizeof(RHSNode));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1]->next = NULL;
                    traverse = table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1];

                    k = lookUp(t1, token);

                    traverse->type = nonterminal;    // Assign the token an enum of type non-terminal
                    traverse->s.nt = (NonTerminal)k; // Assign the value of non-terminal

                    traverse->next = (RHSNode *)malloc(sizeof(RHSNode));
                    traverse = traverse->next;
                    traverse->next = NULL;
                    flag = 1;
                }
                else if (((buffer[i] >= 'a') && (buffer[i] <= 'z')) || ((buffer[i] >= 'A') && (buffer[i] <= 'Z')) || buffer[i] == '_')
                {

                    start = i;
                    while (((buffer[i] >= 'a') && (buffer[i] <= 'z')) || ((buffer[i] >= 'A') && (buffer[i] <= 'Z')) || buffer[i] == '_')
                        i++;
                    char *token = substring(buffer, start, i);

                    table->non_terminals[j].num_rules += 1;
                    table->non_terminals[j].rules = (RHSNode **)realloc(table->non_terminals[j].rules, sizeof(RHSNode *) * (table->non_terminals[j].num_rules));
                    table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1] = (RHSNode *)malloc(sizeof(RHSNode));

                    table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1]->next = NULL;
                    traverse = table->non_terminals[j].rules[table->non_terminals[j].num_rules - 1];

                    k = lookUp(t2, token);
                    traverse->type = terminal;   // Assign the token an enum of type non-terminal
                    traverse->s.t = (Terminal)k; // Assign the value of non-terminal

                    traverse->next = (RHSNode *)malloc(sizeof(RHSNode));
                    traverse = traverse->next;
                    traverse->next = NULL;

                    flag = 1;
                }
                if (buffer[i] == '$')
                    break;
                else
                    i++;
            }
            else if (f == 1 && flag == 1)
            {

                if (buffer[i] == '<')
                {
                    i++;
                    start = i;
                    while (buffer[i++] != '>')
                        ;
                    char *token = substring(buffer, start, i - 1);

                    k = lookUp(t1, token);
                    traverse->type = nonterminal;    // Assign the token an enum of type non-terminal
                    traverse->s.nt = (NonTerminal)k; // Assign the value of non-terminal

                    traverse->next = (RHSNode *)malloc(sizeof(RHSNode));
                    traverse = traverse->next;
                    traverse->next = NULL;
                }

                else if (((buffer[i] >= 'a') && (buffer[i] <= 'z')) || ((buffer[i] >= 'A') && (buffer[i] <= 'Z')) || buffer[i] == '_')
                {

                    start = i;
                    while (((buffer[i] >= 'a') && (buffer[i] <= 'z')) || ((buffer[i] >= 'A') && (buffer[i] <= 'Z')) || buffer[i] == '_')
                        i++;
                    char *token = substring(buffer, start, i);

                    k = lookUp(t2, token);
                    traverse->type = terminal;    // Assign the token an enum of type non-terminal
                    traverse->s.nt = (Terminal)k; // Assign the value of non-terminal

                    traverse->next = (RHSNode *)malloc(sizeof(RHSNode));
                    traverse = traverse->next;

                    traverse->next = NULL;
                }
                else if (buffer[i] == '|')
                {
                    i++;
                    flag = 0;
                }
                if (buffer[i] == '$')
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

void printTable(Grammar *gm)
{
    printf("Printing the Grammar -----------------------------------------------------------------------\n");
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[gm->non_terminals[i].nt]);

        for (int j = 0; j < gm->non_terminals[i].num_rules; j++)
        {
            RHSNode *traverse = gm->non_terminals[i].rules[j];
            printf("Rule %d:\t", j + 1);
            while (traverse->next != NULL)
            {
                if (traverse->type == terminal)
                    printf("%s\t", TerminalEnumToString[traverse->s.t]);
                else
                    printf("%s\t", nonTerminalEnumToString[traverse->s.nt]);
                traverse = traverse->next;
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("Grammar Printing Finished ------------------------------------------------------------------------\n\n\n\n");
}

//------------------------------------------------------------------------------------------------------------------------------------
// First and Follow sets computation

void setBit(int *firstSet, Terminal t)
{
    int k = (int)t;
    int pos = k % 32;

    unsigned int f = 1;

    f = f << pos; // (shifted k positions)

    firstSet[k / 32] = firstSet[k / 32] | f; // Set the bit at the k-th position in firstSet[i]
}

int TestBit(int *firstSet, Terminal t)
{
    int k = (int)t;

    if ((firstSet[k / 32] & (1 << (k % 32))) != 0)
        return 1; // k-th bit is 1
    else
        return 0; // k-th bit is 0
}

int *setUnion(int *firstSet1, int *firstSet2)
{
    for (int i = 0; i < TERMINALS - 1; i++) // -1 is for not including EPSILON as it is the last item in our terminals enum
    {
        if (TestBit(firstSet1, (Terminal)i))
        {
            setBit(firstSet2, (Terminal)i);
        }
    }
    return firstSet2;
}

FirstAndFollow *initializeFirstFollow()
{
    FirstAndFollow *first_follow = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));

    first_follow->first = (First *)malloc(sizeof(First) * NON_TERMINALS);
    first_follow->follow = (Follow *)malloc(sizeof(Follow) * NON_TERMINALS);

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        first_follow->first[i].firstSet = (int *)calloc(ceil(TERMINALS / 32), sizeof(int));
        first_follow->follow[i].followSet = (int *)calloc(ceil(TERMINALS / 32), sizeof(int));
    }
    return first_follow;
}

int checkPrevComputed(FirstAndFollow *F, NonTerminal nt)
{
    for (int j = 0; j < TERMINALS; j++)
    {
        if (TestBit(F->follow[nt].followSet, (Terminal)j))
        {
            return 1;
        }
    }
    return 0;
}

// Following function computes the follow set of non-terminals in a recursive way

int *computeFollow(Grammar *G, FirstAndFollow *F, NonTerminal nt)
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        for (int j = 0; j < G->non_terminals[i].num_rules; j++)
        {
            RHSNode *rhs = G->non_terminals[i].rules[j];

            while (rhs->next != NULL)
            {
                if (rhs->type == nonterminal && rhs->s.nt == nt)
                {
                    rhs = rhs->next;
                    if (rhs->next != NULL)
                    {
                        while (rhs->next != NULL)
                        {
                            if (rhs->type == terminal)
                            {
                                setBit(F->follow[nt].followSet, rhs->s.t);
                                break;
                            }
                            else
                            {
                                for (int k = 0; k < TERMINALS - 1; k++)
                                {
                                    if (TestBit(F->first[rhs->s.nt].firstSet, (Terminal)k))
                                    {
                                        setBit(F->follow[nt].followSet, (Terminal)k);
                                        // printf("%s\n", TerminalEnumToString[(Terminal)k]);
                                    }
                                }
                                if (TestBit(F->first[rhs->s.nt].firstSet, EPSILON) == 0)
                                    break;
                            }
                            rhs = rhs->next;
                            if (rhs->next == NULL)
                            {
                                if (checkPrevComputed(F, G->non_terminals[i].nt))
                                {
                                    F->follow[nt].followSet = setUnion(F->follow[G->non_terminals[i].nt].followSet, F->follow[nt].followSet);
                                }
                                else
                                {
                                    F->follow[nt].followSet = setUnion(computeFollow(G, F, G->non_terminals[i].nt), F->follow[nt].followSet);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (nt == G->non_terminals[i].nt)
                            continue;
                        else
                            F->follow[nt].followSet = setUnion(computeFollow(G, F, G->non_terminals[i].nt), F->follow[nt].followSet);
                    }
                }
                rhs = rhs->next;
                if (rhs == NULL)
                    break;
            }
        }
    }
    return (F->follow[nt].followSet);
}

//Following function computes the first set of every non-terminal
int *computeFirst(Grammar *G, FirstAndFollow *F, NonTerminal nt)
{

    for (int i = 0; i < G->non_terminals[nt].num_rules; i++)
    {
        RHSNode *rhs = G->non_terminals[nt].rules[i];

        while (rhs->next != NULL)
        {
            if (rhs->type == terminal)
            {
                setBit(F->first[nt].firstSet, rhs->s.t);
                break;
            }
            else
            {

                F->first[nt].firstSet = setUnion(computeFirst(G, F, rhs->s.nt), F->first[nt].firstSet);
                if (TestBit(F->first[rhs->s.nt].firstSet, EPSILON) == 0)
                    break;
            }
            rhs = rhs->next;
        }
    }
    return (F->first[nt].firstSet);
}

void ComputeFirstAndFollowSets(Grammar *G, FirstAndFollow *F)
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        computeFirst(G, F, G->non_terminals[i].nt);
    }

    setBit(F->follow[0].followSet, DOLLAR); //Include DOLLAR in follow of start symbol
    for (int i = 1; i < NON_TERMINALS; i++)
    {

        if (!checkPrevComputed(F, (NonTerminal)i))
        {
            computeFollow(G, F, G->non_terminals[i].nt);
        }
    }
}

void printFirstSet(FirstAndFollow *F)
{
    printf("Printing the First Set*************************************************************************\n");

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[i]);

        for (int j = 0; j < TERMINALS; j++)
        {
            if (TestBit(F->first[i].firstSet, (Terminal)j))
            {
                printf("%s\t", TerminalEnumToString[j]);
            }
        }
        printf("\n");
    }

    printf("Printing Finished*****************************************************************************\n\n\n\n");
}

void printFollowSet(FirstAndFollow *F)
{
    printf("Printing the Follow Set*************************************************************************\n");

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[i]);

        for (int j = 0; j < TERMINALS; j++)
        {
            if (TestBit(F->follow[i].followSet, (Terminal)j))
            {
                printf("%s\t", TerminalEnumToString[j]);
            }
        }
        printf("\n");
    }

    printf("Printing Finished*****************************************************************************\n\n\n\n");
}
// ParseTable Construction---------------------------------------------------------------------------------------------------------------

ParseTable *initializeParseTable()
{
    ParseTable *pt = (ParseTable *)malloc(sizeof(ParseTable));

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        memset(pt->Parse[i], -1, sizeof(int) * TERMINALS);
    }
    return pt;
}

void populateParseTable(FirstAndFollow *F, Grammar *G, ParseTable *pt)
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        LHSNode lhs = G->non_terminals[i];
        for (int j = 0; j < lhs.num_rules; j++)
        {
            RHSNode *rhs = lhs.rules[j];
            int flag = 0;

            // case1: terminal but not EPSILON
            if (rhs->type == terminal && rhs->s.t != EPSILON)
            {
                pt->Parse[i][rhs->s.t].rule_no = j;
            }

            //case2: EPSILON
            else if (rhs->s.t == EPSILON)
            {
                for (int k = 0; k < TERMINALS; k++)
                {
                    if (TestBit(F->follow[lhs.nt].followSet, (Terminal)k))
                    {
                        pt->Parse[i][k].rule_no = j;
                    }
                }
            }

            //case3: Non-Terminal
            else
            {
                while (rhs->next != NULL)
                {
                    flag = 0;                  // it becomes 1 if first(rhs -> s.nt) contains EPSILON
                    if (rhs->type == terminal) //this condition will be useful onwards 2nd iteratons
                    {
                        pt->Parse[i][rhs->s.t].rule_no = j;
                        break;
                    }

                    for (int k = 0; k < TERMINALS; k++)
                    {
                        if (TestBit(F->first[rhs->s.nt].firstSet, (Terminal)k))
                        {
                            if (k == (int)EPSILON)
                            {
                                flag = 1;
                                continue;
                            }
                            pt->Parse[i][k].rule_no = j;
                        }
                    }

                    if (flag) // first(rhs -> s.nt) contains EPSILON. So place the rule in the follow of lhs->nt
                    {
                        rhs = rhs->next;
                    }
                    else
                    {
                        break;
                    }
                }
                if (flag)
                {
                    for (int k = 0; k < TERMINALS; k++)
                    {
                        if (TestBit(F->follow[lhs.nt].followSet, (Terminal)k))
                        {
                            pt->Parse[i][k].rule_no = j;
                        }
                    }
                }
            }
        }
    }
}

void printParseTable(ParseTable *pt)
{
    printf("Printing the Parse Table*************************************************************************\n");
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s\n", nonTerminalEnumToString[i]);
        for (int j = 0; j < TERMINALS; j++)
        {
            if (pt->Parse[i][j].rule_no != -1)
                printf("%s -- %d\t", TerminalEnumToString[j], pt->Parse[i][j].rule_no);
        }
        printf("\n");
    }
    printf("ParseTable Printed **********************************************************************************\n\n\n\n");
}

// Parse the input source code and build the parse tree

TreeNode *initialize_internalnode(int tag, char *lexeme, NonTerminal parent, NonTerminal nt)
{
    TreeNode *t = malloc(sizeof(TreeNode));
    t->tag = tag;
    // t -> t.internalnode.lexeme = lexeme;
    strcpy(t->t.internalnode.lexeme, lexeme);
    t->t.internalnode.parent = parent;
    t->t.internalnode.nodesym = nt;
    t->t.internalnode.rhs_size = 0;

    return t;
}

TreeNode *initialize_leafnode(int tag, Terminal ter, NonTerminal parent)
{
    TreeNode *t = malloc(sizeof(TreeNode));
    t->tag = tag;
    t->t.leafnode.t = ter;
    t->t.leafnode.parent = parent;

    return t;
}

ParseTree *initialize_tree()
{
    ParseTree *pt = (ParseTree *)malloc(sizeof(ParseTree));
    pt->levels = 0;
    pt->root = NULL;
    return pt;
}

ParseTree *parseInputSourceCode(char *testCaseFile, ParseTable *table, Grammar *gm, FirstAndFollow *F)
{
    //Initialize the lexer by giving the file input
    FILE *fp = initializeLexer(testCaseFile);
    int flag1 = 0;
    int flag2 = 0, flag3 = 0;
    int count = 0;
    //initialize stack
    Stack *st = initialize_stack();
    Stack *buffer = initialize_stack();

    ParseTree *pt = initialize_tree();
    pt->root = initialize_internalnode(0, "----", ROOT, program);

    TreeNode *dummy = malloc(sizeof(TreeNode));
    dummy->t.leafnode.t = DOLLAR;
    dummy->tag = 2; //tag = 2 for DOLLAR

    push(st, dummy);    //initially push DOLLAR into the stack
    push(st, pt->root); //Push the start symbol of the grammar into the stack

    // Get the first token from lexer
    Token *tk = getNextToken(fp);

    TreeNode *snode;

    while (tk != NULL && endOfFile != 1)
    {
        flag1 = 0;
        snode = top(st);

        // if(snode -> tag == 0)
        // 	printf("%s -- Hello\n", nonTerminalEnumToString[snode->t.internalnode.nodesym]);
        // else
        // 	printf("%s -- Hello\n", TerminalEnumToString[snode->t.leafnode.t]);

        if (tk->name == COMMENTMARK) // If the token returned is a comment or lexical error ignore it
        {
            tk = getNextToken(fp);
            continue;
        }
        if (tk->name == ERROR)
        {
            printf("Line %d: Lexical Error encountered, unexpected token %s\n", tk->lineNo, tk->lexeme);
            tk = getNextToken(fp);
            flag3 = 1;
            continue;
        }

        // If top of the stack is non-terminal
        if (snode->tag == 0)
        {
            int index_lhs_grm = (int)(snode->t.internalnode.nodesym);    //Extract the non-terminal enum from it
            int rule_no = table->Parse[index_lhs_grm][tk->name].rule_no; // this will give me the rule number corresponding to the given stack top and current token given by lexer
            if (rule_no == -1)                                           // Parse Table entry corresponds to an error
            {
                count++;
                // Generate syntax error
                if (!flag2 && !flag3)
                    printf("Line %d: Syntax Error in the input\n", tk->lineNo);

                flag2 = 0;
                flag3 = 0;
                //Error Recovery using both first and follow set method of panic mode

                while (tk != NULL && TestBit(F->follow[index_lhs_grm].followSet, tk->name) != 1)
                {
                    if (TestBit(F->first[index_lhs_grm].firstSet, tk->name))
                    {
                        flag1 = 1;
                        break;
                    }
                    tk = getNextToken(fp);
                }
                if (!flag1)
                {
                    pop(st);
                    snode = top(st);
                    if (snode->tag == 1)
                        continue;
                    // position of NT in enum nodesym
                    index_lhs_grm = (int)(snode->t.internalnode.nodesym);
                }

                if (tk == NULL)
                {
                    endOfFile = 0;
                    currLine = 1;
                    return pt;
                }
            }

            // rhs linkedlist

            rule_no = table->Parse[index_lhs_grm][tk->name].rule_no;
            // no rule for that NT and T (error case)
            if (rule_no == -1)
            {
                // printf("Here\n");
                continue;
            }

            RHSNode *rhs = gm->non_terminals[index_lhs_grm].rules[rule_no];
            RHSNode *tmp = rhs;

            // snode->rule_no1 = index_lhs_grm; //store the rule number corresponding to the grammar
            snode->rule_no = rule_no;
            snode->addr = NULL;
            snode->inh = NULL;
            snode->t.internalnode.rhs_size = 0;
            snode->t.internalnode.children = malloc(sizeof(TreeNode *));

            if (rhs->type == terminal && rhs->s.t == EPSILON) // If the rule is EPSILON
            {
                pop(st);
                TreeNode *tmpnode = initialize_leafnode(1, rhs->s.t, snode->t.internalnode.nodesym);
                snode->t.internalnode.children = realloc(snode->t.internalnode.children, sizeof(TreeNode *) * (snode->t.internalnode.rhs_size + 1));
                snode->t.internalnode.children[snode->t.internalnode.rhs_size] = tmpnode;
                snode->t.internalnode.rhs_size += 1;
                continue;
            }

            while (tmp->next) // Using two stacks to push the rule in reverse order
            {
                TreeNode *tmpnode;
                if (tmp->type == nonterminal)
                {
                    tmpnode = initialize_internalnode(0, "----", snode->t.internalnode.nodesym, tmp->s.nt);
                    push(buffer, tmpnode);
                }

                else
                {
                    tmpnode = initialize_leafnode(1, tmp->s.t, snode->t.internalnode.nodesym);
                    push(buffer, tmpnode);
                }

                snode->t.internalnode.children = realloc(snode->t.internalnode.children, sizeof(TreeNode *) * (snode->t.internalnode.rhs_size + 1));
                snode->t.internalnode.children[snode->t.internalnode.rhs_size] = tmpnode;

                snode->t.internalnode.rhs_size += 1;

                tmp = tmp->next;
            }

            TreeNode *non_terminal_top = pop(st); //pop the current NT

            while (!isempty(buffer))
            {
                push(st, pop(buffer));
            }
        }

        // Top of the stack is terminal
        else
        {
            if (snode->t.leafnode.t == tk->name) // Terminal - Treminal match
            {
                snode->t.leafnode.lineno = tk->lineNo;
                strcpy(snode->t.leafnode.lexeme, tk->lexeme);
                if (tk->name == NUM || tk->name == RNUM)
                    snode->t.leafnode.val = tk->val;
                pop(st); //pop the Terminal
                tk = getNextToken(fp);
                flag2 = 0;
                flag3 = 0;
            }
            else if (snode->tag == 2 && tk != NULL)
            {
                printf("Syntax Error, stack became empty \n");
                endOfFile = 0;
                currLine = 1;
                return pt;
            }
            else if (snode->t.leafnode.t != tk->name) //Terminal - Terminal miss-match, Error recovery using Panic Mode
            {
                count++;
                if (snode->t.leafnode.t == SEMICOL)
                {
                    printf("Line %d: Error in the input as expected %s before this line\n", tk->lineNo, TerminalEnumToString[snode->t.leafnode.t]);
                    pop(st);
                    flag2 = 1;
                    continue;
                }
                printf("Line %d: Error in the input as expected token is %s\n", tk->lineNo, TerminalEnumToString[snode->t.leafnode.t]);

                //Error Recovery using first set method of panic mode
                pop(st);
                flag2 = 1;
            }
        }
    }

    if (snode->tag == 2)
    {
        printf("Parsing Completed*************************************************\n");
    }
    else
    {
        snode = pop(st);
        snode->rule_no = 1;
        snode->addr = NULL;
        snode->inh = NULL;
        TreeNode *tmpnode = initialize_leafnode(1, EPSILON, snode->t.internalnode.nodesym);
        snode->t.internalnode.children = realloc(snode->t.internalnode.children, sizeof(TreeNode *) * (snode->t.internalnode.rhs_size + 1));
        snode->t.internalnode.children[snode->t.internalnode.rhs_size] = tmpnode;
        snode->t.internalnode.rhs_size += 1;

        if (top(st)->tag == 2)
        {
            printf("*************************  Parsing Completed Successfully ******************************\n");
            printf("----------------------- Parse Tree is printed in the specified file ----------------------\n\n");
        }
        if (count == 0)
            printf("*****************  Input Source Code is Syntatically Correct **********************\n\n\n\n");
    }

    endOfFile = 0;
    currLine = 1;
    return pt;
}

// NaryTree Functions

Queue *initialize_queue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->top = 0;
    q->size = 0;
    q->capacity = 50;
    q->end = 0;
    q->t = (TreeNode **)malloc(sizeof(TreeNode *) * q->capacity);

    return q;
}

void enQueue(Queue *q, TreeNode *t)
{
    if (q->size == q->capacity)
    {
        q->capacity += 20;
        q->t = realloc(q->t, sizeof(TreeNode *) * q->capacity);
    }
    q->t[q->end] = t;
    q->end = (q->end + 1) % q->capacity;
    q->size += 1;
}

TreeNode *deQueue(Queue *q)
{
    if (q->size == 0)
    {
        printf("Underflow!");
        return NULL;
    }
    TreeNode *result = q->t[q->top];
    q->top = (q->top + 1) % q->capacity;
    q->size -= 1;
    return result;
}

TreeNode *first(Queue *q)
{
    if (q->size == 0)
    {
        // printf("Tree is empty");
        return NULL;
    }
    return q->t[q->top];
}

TreeNode *last(Queue *q)
{
    if (q->size == 0)
    {
        //printf("Tree is empty");
        return NULL;
    }
    return q->t[q->end];
}

void printNode(TreeNode *node, FILE *fp)
{
    // if internal
    if (node->tag == 0)
    {
        fprintf(fp, "%20s\t %20s\t %20s\t %20s\t %25s\t %20s\t %25s\n", node->t.internalnode.lexeme, "-----", "-----", "-----", nonTerminalEnumToString[node->t.internalnode.parent], "Not Leaf", nonTerminalEnumToString[node->t.internalnode.nodesym]);
    }
    // if leaf
    else if (node->tag == 1)
    {
        if (node->t.leafnode.t == NUM)
            fprintf(fp, "%20s\t %20d\t %20s\t %20d\t %25s\t %20s\t %25s\n", node->t.leafnode.lexeme, node->t.leafnode.lineno, TerminalEnumToString[node->t.leafnode.t], node->t.leafnode.val->intValue, nonTerminalEnumToString[node->t.leafnode.parent], "Is Leaf", "-----");
        else if (node->t.leafnode.t == RNUM)
            fprintf(fp, "%20s\t %20d\t %20s\t %5.15f\t %25s\t %20s\t %25s\n", node->t.leafnode.lexeme, node->t.leafnode.lineno, TerminalEnumToString[node->t.leafnode.t], node->t.leafnode.val->fValue, nonTerminalEnumToString[node->t.leafnode.parent], "Is Leaf", "-----");
        else if (node->t.leafnode.t == EPSILON)
            fprintf(fp, "%20s\t %20d\t %20s\t %20s\t %25s\t %20s\t %25s\n", "----", 0, TerminalEnumToString[node->t.leafnode.t], "-----", nonTerminalEnumToString[node->t.leafnode.parent], "Is Leaf", "-----");
        else
            fprintf(fp, "%20s\t %20d\t %20s\t %20s\t %25s\t %20s\t %25s\n", node->t.leafnode.lexeme, node->t.leafnode.lineno, TerminalEnumToString[node->t.leafnode.t], "-----", nonTerminalEnumToString[node->t.leafnode.parent], "Is Leaf", "-----");
    }
}

void printParseTree_levelOrder(TreeNode *pt, char *filename)
{
    FILE *fptr = fopen(filename, "w");

    TreeNode *tmp = pt;
    if (!tmp)
    {
        return;
    }
    fprintf(fptr, "%20s\t %20s\t %20s\t %20s\t %25s\t %20s\t %25s\n", "Lexeme", "LineNo", "TokenName", "ValuelfNumber", "parentNodeSymbol", "IsLeafNode", "NodeSymbol");

    // level order
    Queue *q = initialize_queue();
    TreeNode *dummy = (TreeNode *)malloc(sizeof(TreeNode));
    dummy->tag = 2;

    enQueue(q, tmp);
    enQueue(q, dummy); //marks the end of a level

    while (first(q) != NULL)
    {
        tmp = deQueue(q);
        if (tmp->tag == 0)
        {
            for (int i = 0; i < tmp->t.internalnode.rhs_size; i++)
            {
                enQueue(q, tmp->t.internalnode.children[i]);
            }
            printNode(tmp, fptr);
        }
        else if (tmp->tag == 1)
        {
            printNode(tmp, fptr);
        }
        else if (tmp->tag == 2)
        {
            if (first(q) != NULL)
                enQueue(q, dummy);
        }
    }

    fclose(fptr);
}

void printParseTree_inorder(TreeNode *pt, char *filename)
{
    FILE *fptr = fopen(filename, "w");

    fprintf(fptr, "%20s\t %20s\t %20s\t %20s\t %25s\t %20s\t %25s\n", "Lexeme", "LineNo", "TokenName", "ValuelfNumber", "parentNodeSymbol", "IsLeafNode", "NodeSymbol");
    printParseTree_inorder_util(pt, fptr);

    fclose(fptr);
}

void printParseTree_inorder_util(TreeNode *pt, FILE *fpt)
{
    TreeNode *tmp = pt;
    if (!tmp)
    {
        //printf("Parse Tree empty");
        return;
    }

    //inorder
    if (tmp->tag == 0)
    {
        if (tmp->t.internalnode.rhs_size > 0)
            printParseTree_inorder_util(tmp->t.internalnode.children[0], fpt);
        printNode(tmp, fpt);
        for (int i = 1; i < tmp->t.internalnode.rhs_size; i++)
        {
            //  printf("%d\n",tmp->t.internalnode.rhs_size);
            printParseTree_inorder_util(tmp->t.internalnode.children[i], fpt);
        }
    }
    else
    {
        printNode(tmp, fpt);
        // printf("\n");
    }
}

//**************************************************************************************

Stack *initialize_stack()
{
    Stack *st = malloc(sizeof(Stack));
    st->top = -1;
    st->size = 0;
    st->capacity = 100;
    st->t = malloc(sizeof(TreeNode *) * st->capacity);
    return st;
}

void push(Stack *st, TreeNode *node)
{
    if (st->size == st->capacity)
    {
        st->capacity += 20;
        st->t = realloc(st->t, sizeof(TreeNode *) * st->capacity);
    }
    st->top++;
    st->t[st->top] = node;
    st->size += 1;
}

TreeNode *pop(Stack *st)
{
    if (st->size < 1)
        return NULL;

    TreeNode *tmp = st->t[st->top];
    st->top--;
    st->size -= 1;
    return tmp;
}

TreeNode *top(Stack *st)
{
    if (st->size > 0)
    {
        return st->t[st->top];
    }
    else
        return NULL;
}

int isempty(Stack *st)
{
    return st->size ? 0 : 1;
}

////////////////////////////////////////////////////////////////////
