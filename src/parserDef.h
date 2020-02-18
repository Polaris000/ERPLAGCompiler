
typedef struct
{

}Grammar;

typedef struct
{

}FirstAndFollow;


typedef struct
{
	char str[100];
	Symbol s;
	Type t;
}table;

typedef Table table[10];

typedef struct
{
	char lexeme[100];
	int lineno;
	char tokenname[25];


}ParserTreeNode;

typedef struct
{

}ParseTree;


typedef enum 
{
	terminal,
	nonterminal
}Type;

typedef union symbol
{
	Terminal t;
	NonTerminal nt;
}Symbol;
