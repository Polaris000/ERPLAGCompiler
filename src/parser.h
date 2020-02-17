typedef struct
{

}Grammar;

typedef struct
{

}FirstAndFollow;

typedef Table table[10];

typedef struct
{
	char str[100];
	Symbol s;
	Type t;
}table;

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
