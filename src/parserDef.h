// parse table ----------------------------------------
//		t1	t2	t3	$
// Nt1
// NT2
// NT3

// pt[NT][T] <- char * 

char *ParseTable[100][100];

// grammar --------------------------------------------
typedef struct RHS_
{
	Symbol s;
	struct RHS_ * next;
}RHS;

typedef struct LHS_
{
	Symbol s;
	RHS *first;
	struct LHS_ * next;
}LHS;

typedef struct
{
	LHS* firstrule;
}Grammar;

// first and follow -----------------------------------
typedef struct
{

}First;

typedef struct
{

}Follow;

typedef struct
{
	First first;
	Follow follow;

}FirstAndFollow;

// mapping table -----------------------------------------------
typedef enum 
{
	terminal,
	nonterminal
}Type;

typedef struct
{
	char str[100];
	Symbol s;
	Type t;
}table;

typedef Table table[10]; // set size appropriately

typedef union{
	int intValue;
	float fValue;
}value;

// parse tree ------------------------------------------
typedef struct
{
	// display  info
	char lexeme[100]; // --- if not leaf node
	int lineno;
	char tokenname[25]; 
	int isLeafNode;
	Symbol ParentSymbol; // ROOT for root node
	Symbol NodeSymbol; // if not leaf
	value * val;

	// misc
	ParseTree *children;
}
ParseTree;

// symbol --------------------------------------------

typedef union symbol
{
	Terminal t;
	NonTerminal nt;
}Symbol;
