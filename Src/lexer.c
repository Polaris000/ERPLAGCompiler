// #include "hashtable.h"
#include "lexer.h"

#include <ctype.h>
#include <string.h>

//Twin Buffer Scheme
char* firstBuffer;
char* secondBuffer;

int currPtr = 0;
int currLine = 1;
int flag = 0;  //denotes the that pointer has reached an final state
int endOfFile = 0;
HT* hashtable;


const char* tokenNameEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", 
	"USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
	"DEFAULT", "WHILE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
	"COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK","ID","NUM","RNUM"};

FILE* getStream(FILE* fp)
{
	//firstBuffer = secondBuffer;
	int i;
	for(i = 0; i < strlen(firstBuffer); i++)
		firstBuffer[i] = '\0';
	int numChar;
	numChar = fread(firstBuffer,sizeof(char),2048,fp);

	if(numChar > 0)
	{
		firstBuffer[2048] = '\0';
		return fp;
	}
	else
	{
		fclose(fp);
		return NULL;
	}
}

int isValidID(char c)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <='9') || (c == '_'))
		return 1;
	else
		return 0;
}

void generateToken(Token* tk,TokenName name, char* str, int linenum)
{
	tk -> name = name;
	tk -> lexeme = str;
	tk -> lineNo = linenum;
}



Token* getNextToken(FILE* fp)
{
	Token* tk = (Token *)malloc(sizeof(Token));

	int state = 0, currLexemeLength = 0;
	char strToken[50];
	flag = 0;
	while(1)
	{
		if(flag == 1)
		{
			break;
		}
		switch(state)
		{
			case 0:
				if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;
					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 0;
					}
				}
				else if(firstBuffer[currPtr] == ' ' || firstBuffer[currPtr] == '\t' || firstBuffer[currPtr] == '\r')
				{
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '\n')
				{
					currLine++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					state = 1;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if((firstBuffer[currPtr] >= 'a' && firstBuffer[currPtr] <= 'z') || (firstBuffer[currPtr] >= 'A' && firstBuffer[currPtr] <= 'Z'))
				{
					state = 11;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '<')
				{
					state = 13;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '>')
				{
					state = 17;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == ':')
				{
					state = 21;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '=')
				{
					state = 24;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '!')
				{
					state = 26;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '*')
				{
					state = 28;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '+')
				{
					state = 33;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '-')
				{
					state = 34;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '/')
				{
					state = 35;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == ';')
				{
					state = 36;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == ',')
				{
					state = 37;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '[')
				{
					state = 38;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == ']')
				{
					state = 39;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '(')
				{
					state = 40;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == ')')
				{
					state = 41;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '.')
				{
					state = 42;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else
				{
					// Unexpected character encountered
				   //Goto Trap State
					printf("Invalid Character encountered at state 0\n");
					currPtr++;
				}
				break;
			case 1:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					while(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
					{
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;
					}
					if(firstBuffer[currPtr] == '.')
					{
						state = 3;
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;
					}
					else if(firstBuffer[currPtr] == '\0')
					{
						fp = getStream(fp);
						currPtr = 0;

						if(fp == NULL)
						{
							printf("Comes Here\n");
							endOfFile = 1;
							flag = 1;
						}
						else
						{
							state = 1;
						}
					}
					else
					{
						state = 2;   // Others character encountered. So return NUM
					}
				}
				else if(firstBuffer[currPtr] == '.')
				{
					state = 3;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++; 
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 1;
					}
				}
				else
				{
					state = 2;  // Retract
				}
				break;

			case 2:
				strToken[currLexemeLength] = '\0';
				generateToken(tk,NUM,strToken,currLine);
				flag = 1;
				break;

			case 3:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					state = 4;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '.')   // This will handle the RANGEOP operator immediately after an integer 
				{
					currLexemeLength--;
					strToken[currLexemeLength] = '\0';
					generateToken(tk,NUM,strToken,currLine);
					flag = 1;
					// printf("Comes Here\n");
					currPtr--;  //retract twice
					// printf("%c\n", firstBuffer[currPtr-1]);
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 3;
					}
				}
				else
				{
					// Give Lexical error
					printf("Error at line %d\n", currLine);
				}
				break;

			case 4:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					while(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
					{
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;

					}
					if(firstBuffer[currPtr] == 'e' || firstBuffer[currPtr] == 'E')
					{
						state = 6;
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;
					}
					else if(firstBuffer[currPtr] == '\0')
					{
						fp = getStream(fp);
						currPtr = 0;

						if(fp == NULL)
						{
							endOfFile = 1;
						}
						else
						{
							state = 4;
						}
					}
					else
					{
						state = 5;
					}
				}
				else if(firstBuffer[currPtr] == 'e' || firstBuffer[currPtr] == 'E')
				{
					state = 6;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 4;
					}
				}
				else
				{
					state = 5;  // Retract
				}
				break;

			case 5:
				strToken[currLexemeLength] = '\0';
				generateToken(tk,RNUM,strToken,currLine);
				flag = 1;
				break;
			case 6:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					state = 8;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if(firstBuffer[currPtr] == '+' || firstBuffer[currPtr] == '-')
				{
					state = 7;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 6;
					}
				}
				else
				{
					//Send to trap state 
					//Generate error
					printf("Error at line %d\n", currLine);
				}
				break;
			case 7:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					state = 8;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 7;
					}
				}
				else
				{
					//Send to trap state 
					//Generate error	
					printf("Error at line %d\n", currLine);
				}
				break;

			case 8:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
				{
					while(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
					{
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;

					}
					if(firstBuffer[currPtr] == '\0')
					{
						fp = getStream(fp);
						currPtr = 0;

						if(fp == NULL)
						{
							endOfFile = 1;
						}
						else
						{
							state = 8;
						}
					}
					else
						state = 9;
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 8;
					}
				}
				else
				{
					state = 9;   // Retract
				}
				break;
			case 9:
				strToken[currLexemeLength] = '\0';
				generateToken(tk,RNUM,strToken,currLine);
				flag = 1;
				break;

			//Following part handles the keywords and identifiers
			case 11:
				if(isValidID(firstBuffer[currPtr]))
				{
					while(isValidID(firstBuffer[currPtr]))
					{
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;
					}
					if(firstBuffer[currPtr] == '\0')
					{
						fp = getStream(fp);
						currPtr = 0;

						if(fp == NULL)
						{
							endOfFile = 1;
						}
						else
						{
							state = 11;
						}
					}
					else
						state = 12; //Retract
				}
				else if(firstBuffer[currPtr] == '\0')
				{
					fp = getStream(fp);
					currPtr = 0;

					if(fp == NULL)
					{
						endOfFile = 1;
					}
					else
					{
						state = 11;
					}
				}
				else
				{
					state = 12;  // Retract
				}
				break;

			case 12:
				strToken[currLexemeLength] = '\0';
				if(strlen(strToken) > 20)
				{
					printf("Error: %d, length of identifier is greater than 20\n", currLine);
					flag = 1;
					break;
				}
				else
				{
					keyword* check = lookup(hashtable,strToken);
					if(check)
					{
						generateToken(tk,check->name,strToken,currLine);  // which is a keyword
						flag = 1;
					}
					else
					{
						generateToken(tk,ID,strToken,currLine);  // which is an identifier
						flag = 1;
					}
				}
				break;

			case 13:
				if(firstBuffer[currPtr] == '<')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 14;
				}
				else if(firstBuffer[currPtr] == '=')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 16;	
				}
				else
				{
					state = 15;  // Retract
				}
				break;
			case 14:
				if(firstBuffer[currPtr] == '<')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 45;
				}
				else
				{
					state = 46;
				}
				break;
			case 46:
				generateToken(tk,DEF,"<<",currLine); // Retract
				flag = 1;
				break;
			case 45:
				generateToken(tk,DRIVERDEF,"<<<",currLine); // Retract
				flag = 1;
				break;
			case 16:
				generateToken(tk,LE,"<=",currLine);
				flag = 1;
				break;
			case 15:
				generateToken(tk,LT,"<",currLine);
				flag = 1;
				break;
				//Retract

			case 17:
				if(firstBuffer[currPtr] == '>')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 18;
				}
				else if(firstBuffer[currPtr] == '=')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 20;	
				}
				else
				{
					state = 19;  // Retract
				}
				break;

			case 18:
				if(firstBuffer[currPtr] == '>')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 47;
				}
				else
				{
					state = 48;
				}
				break;
			case 19:
				generateToken(tk,GT,">",currLine); // Retract
				flag = 1;
				break;
			case 47:
				generateToken(tk,DRIVERENDDEF,">>>",currLine); // Retract
				flag = 1;
				break;
			case 48: 
				generateToken(tk,ENDDEF,">>",currLine); // Retract
				flag = 1;
				break;
			case 20:
				generateToken(tk,GE,">=",currLine);
				flag = 1;
				break;

			case 21:
				if(firstBuffer[currPtr] == '=')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 23;
				}
				else
				{
					state = 22; // Retract
				}
				break;
			case 22:
				//Retract
				generateToken(tk,COLON,":",currLine);
				flag = 1;
				break;
			case 23:
				generateToken(tk,ASSIGNOP,":=",currLine);
				flag = 1;
				break;

			case 24:
				if(firstBuffer[currPtr] == '=')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 25;
				}
				else
				{
					//Generate Error Unexpected character
				}
				break;
			case 25:
				generateToken(tk,EQ,"==",currLine);
				flag = 1;
				break;

			case 26:
				if(firstBuffer[currPtr] == '=')
				{
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
					state = 27;
				}
				else
				{
					//Generate Error Unexpected character
				}
				break;
			case 27:
				generateToken(tk,NE,"!=",currLine);
				break;

			//Following part deals with the comments and multiplication operations
			case 28:
				if(firstBuffer[currPtr] == '*')
				{
					state = 29;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currLexemeLength++;
					currPtr++;
				}
				else
				{
					state = 32; //Retract
				}
				break;
			case 29:
				if(firstBuffer[currPtr] == '*')
				{
					state = 30;
					currPtr++;
					// Store or not
				}
				else if(firstBuffer[currPtr] == '\n')
				{
					currLine++;
					currPtr++;
				}
				else
				{
					currPtr++;
				}
				break;
			case 30:
				if(firstBuffer[currPtr] == '*')
				{
					state = 31;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '\n')
				{
					currLine++;
					currPtr++;
				}
				else
				{
					currPtr++;
					state = 29;
				}
				break;
			case 31:
				generateToken(tk,COMMENTMARK,"**",currLine);
				flag = 1;
				break;
			case 32:
				generateToken(tk,MUL,"*",currLine);//Retract
				flag = 1;
				break;

			case 33:
				generateToken(tk,PLUS,"+",currLine);
				flag = 1;
				break;

			case 34:
				generateToken(tk,MINUS,"-",currLine);
				flag = 1;
				break;

			case 35:
				generateToken(tk,DIV,"/",currLine);
				flag = 1;
				break;

			case 36:
				generateToken(tk,SEMICOL,";",currLine);
				flag = 1;
				break;

			case 37:
				generateToken(tk,COMMA,",",currLine);
				flag = 1;
				break;

			case 38:
				generateToken(tk,SQBO,"[",currLine);
				flag = 1;
				break;

			case 39:
				generateToken(tk,SQBC,"]",currLine);
				flag = 1;
				break;

			case 40:
				generateToken(tk,BO,"(",currLine);
				flag = 1;
				break;

			case 41:
				generateToken(tk,BC,")",currLine);
				flag = 1;
				break;

			case 42:
				if(firstBuffer[currPtr] == '.')
				{
					state = 43;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else
				{
					//Generate error
				}
			case 43:
				generateToken(tk,RANGEOP,"..",currLine);
				flag = 1;
				break;
			case 44:
				break;
		}
	}

	return tk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HT* generate_hashtable(int s){
   HT* hash=(HT*)malloc(s*sizeof(HT));
   hash->size=s;
   hash->arr=(entry**) malloc((s)*sizeof(entry*));
   for(int i=0;i<s;i++){
       hash->arr[i]=(entry*) malloc(sizeof(entry));
       hash->arr[i]->chain_size=0;
       hash->arr[i]->ptr=NULL;
   }
   return hash;
}
 
HT* make_array(HT* hash,char** arr, int s){
    for(int i=0;i<s;i++){
        int j=0,hash_val=0;
        while(arr[i][j]!='\0'){
            hash_val=(((hash_val*47)%hash->size)+(arr[i][j]%hash->size))%hash->size;
            j++;
        }
 
    TokenName arrTokens[] = {INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,PROGRAM,
                        GET_VALUE,PRINT,USE,WITH,PARAMETERS,TRUE,FALSE,TAKES,INPUT,RETURNS,AND,OR,
                        FOR,IN,SWITCH,CASE,BREAK,DEFAULT,WHILE};
    
        keyword* new=(keyword*) malloc(sizeof(keyword));
        new->token=(char*) malloc((j+1)*sizeof(char));
        new->token=arr[i];
        new -> name = arrTokens[i];
        new->k=NULL;
        
        if(hash->arr[hash_val]->ptr!=NULL){
            keyword* temp=hash->arr[hash_val]->ptr;
            while(temp->k!=NULL)
                temp=temp->k;
            temp->k=new;
            hash->arr[hash_val]->chain_size+=1;
        }
        else{
            hash->arr[hash_val]->ptr=new;
            hash->arr[hash_val]->chain_size=1;
        }
    }
    return hash;
}

void printTable(HT* hash){
    for(int i=0;i<hash->size;i++){
        keyword* temp=hash->arr[i]->ptr;
        printf("%d :\t",(i+1));
        while(temp!=NULL){
            printf("%s\t",temp->token);
            temp=temp->k;
        }
        printf("\n");
    }
}

keyword* lookup(HT* hash, char* key){
    int j=0,hash_val=0;
    while(key[j]!='\0'){
        hash_val=(((hash_val*47)%hash->size)+(key[j]%hash->size))%hash->size;
        j++;
    }

    keyword* temp=hash->arr[hash_val]->ptr;
    while(temp!=NULL){
        if(strcmp(temp->token,key)==0)
            return temp;
        temp=temp->k;
    }
    return NULL;
}

HT* insert(HT* hash,char* key){
    int j=0,hash_val=0;
    while(key[j]!='\0'){
        hash_val=(((hash_val*47)%hash->size)+(key[j]%hash->size))%hash->size;
        j++;
    }
    
    keyword* new=(keyword*) malloc(sizeof(keyword));
    new->token=(char*) malloc(j*sizeof(char));
    new->token=key;
    new->k=NULL;
    
    if(hash->arr[hash_val]->ptr!=NULL){
        keyword* temp=hash->arr[hash_val]->ptr;
        if(strcmp(temp->token,key)==0){
            printf("Keyword of the same name already exists!\n");
            return hash;
        }
        while(temp->k!=NULL){ 
            temp=temp->k;
            if(strcmp(temp->token,key)==0){
                printf("Keyword of the same name already exists!\n");
                return hash;
            }
        }
        temp->k=new;
        hash->arr[hash_val]->chain_size+=1;
    }
    else{
        hash->arr[hash_val]->ptr=new;
        hash->arr[hash_val]->chain_size=1;
    }
}

char** populate_keywords(char* filename){
    FILE* ptr;
    ptr=fopen(filename,"r");
    int size=0;

    char s[30];
    while(fgets(s,30,ptr)!=NULL){
        size++;
    }

    fseek(ptr,0,SEEK_SET);
    int str_len[size];
    char c=fgetc(ptr);
    int i=0;
    while(c!=EOF){
        int len=0;
        while(c!='\n'){
            len++;
            c=fgetc(ptr);
        }
        str_len[i]=len+1;
        i++;
        c=fgetc(ptr);
    }

    fseek(ptr,0,SEEK_SET);
    char** table=(char**) malloc(size*sizeof(char*));
    for(i=0;i<size;i++){
        table[i]=(char*)malloc(str_len[i]*sizeof(char));
        fgets(table[i],str_len[i],ptr);
        table[i][str_len[i]-1]='\0';
        fgetc(ptr);
    }
    fclose(ptr);
    return table;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printTokens(char * testCaseFile)
{
	FILE *fp = fopen(testCaseFile,"r");
	if(fp == NULL)
	{
		printf("Error in opening file\n");
	}
	else
	{
		printf("File opened with success\n");
	}
	firstBuffer = (char*)malloc(sizeof(char)*2049);
	fp = getStream(fp);
	fclose(fp);
	// for(int i = 0; i < strlen(firstBuffer); i++)
	// {
	// 	printf("%c ", firstBuffer[i]);
	// }

	//Construct the hashtable
	char** keys = populate_keywords("keywords.txt");
    
    hashtable = generate_hashtable(41);
    hashtable = make_array(hashtable,keys,30);

    Token* tk = (Token*)getNextToken(fp);
    int n = 200;
    while(n--)
    {
    	if(tk->name != COMMENTMARK)
    		printf("Line: %d\t TokenName: %s\t  lexeme: %s\n",tk->lineNo,tokenNameEnumToString[tk->name],tk->lexeme);
    	tk = getNextToken(fp);
    }
    printf("Finished\n");
}

int main()
{
	printTokens("testcase8.txt");
}