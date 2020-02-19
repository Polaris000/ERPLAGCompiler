// parse table ----------------------------------------
//		t1	t2	t3	$
// Nt1
// NT2
// NT3

// pt[NT][T] <- char * 

typedef enum 
{
	program,moduleDeclarations,moduleDeclaration,otherModules, driverModule, module,
	ret, input_plist, input_plist_dash, output_plist, output_plist_dash, dataType, range_array,
	type, moduleDef, statements, statement, ioStmt, var, var_id_num, boolConstt, whichId,
	simpleStmt, assignmentStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, index, moduleReuseStmt,
	optional, idList, idList_dash, expression, unaryOrExpr, arithmeticOrBooleanExpr, anyTerm,
	arithmeticExpr_dash, bool, arithmeticExpr, arithmeticExpr_recur, term, term_dash, factor, 
	op_plus_minus, op_mul_div, logicalOp, relationalOp, declareStmt, conditionalStmt, caseStmts,
	caseStmt, value, default, iterativeStmt, range 
}NonTerminal;

char *ParseTable[100][100];

// grammar --------------------------------------------
typedef struct RHSNode
{
	Symbol s;
	Type t;
	struct RHSNode * next;
}RHSNode;

typedef struct LHS_
{
	NonTerminal nt;
	int num_rules;

	RHSNode **rules;
}LHSNode;

typedef struct
{
	int num_rules;
	LHSNode ** non_terminals;
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
