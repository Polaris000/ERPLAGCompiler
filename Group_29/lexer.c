/* GROUP No. 29
		2017A7PS0004P -- SUBHAM KUMAR DASH
		2017A7PS0036P -- RAHUL JHA
		2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
		2017A7PS0128P -- MEET KANANI
		2017A7PS0193P -- AYUSH GARG
*/



#include "lexer.h"
#include <string.h>

#define bufferSize 512


//Single Buffer Scheme
char* firstBuffer;
int currPtr = 0;		   //denotes the current pointer for the buffer
int currLine = 1;         //denotes the current line number
int flag = 0;  		     //denotes the that pointer has reached an final state
int endOfFile = 0;       // denotes that we have read all the characters from the file
HT* hashtable;
int f = 0;

char* tokenNameEnumToString[] = {"INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", 
	"USE", "WITH", "PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK",
	"DEFAULT", "WHILE", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF",
	"COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "COMMENTMARK","ID","NUM","RNUM","ERROR"};


FILE* getStream(FILE* fp)
{
	// printf("Filling the Buffer---------------------------------------------------------------------------\n");
	int numChar;
	numChar = fread(firstBuffer,sizeof(char),bufferSize-1,fp);
	if(numChar > 0)
	{
		firstBuffer[numChar] = '$';                       //sentinel character added to denote the end
		currPtr = 0;
		return fp;
	}
	else
	{
		printf("End of File Reached, Closing the file\n\n\n");
		fclose(fp);                                        // close the file if no more character left and return NULL
		endOfFile = 1;
		return NULL;
	}
}


void generateToken(Token* tk,Terminal name, char str[], int linenum)
{
	tk -> name = name;
	strcpy(tk->lexeme,str);
	tk -> lineNo = linenum;
	tk -> val = malloc(sizeof(value));
	if(tk -> name == NUM)
		tk -> val -> intValue = atoi(str);
	else if(tk -> name == RNUM)
		tk -> val -> fValue = atof(str);
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
		if(endOfFile)
			return NULL;
		switch(state)
		{
			case 0:
				if(firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <= '9')
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
				else
				{
					switch(firstBuffer[currPtr])
					{
						case '$':
						{
							fp = getStream(fp);
							break;
						}
						case ' ' :
						case '\t':
						case '\r':
							currPtr++;
							break;
						case '\n':
							currLine++;
							currPtr++;
							break;
						case '<':
							state = 13;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;
							break;
						case '>':
							state = 17;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;
							break;
						case ':':
							state = 21;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;
							break;
						case '=':
							state = 24;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;
							break;
						case '!':
							state = 26;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;
							break;
						case '*':
							state = 28;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;
							break;
						case '+':
							state = 33;
							currPtr++;
							break;
						case '-':
							state = 34;
							currPtr++;
							break;
						case '/':
							state = 35;
							currPtr++;
							break;
						case ';':
							state = 36;
							currPtr++;
							break;
						case ',':
							state = 37;
							currPtr++;
							break;
						case '[':
							state = 38;
							currPtr++;
							break;
						case ']':
							state = 39;
							currPtr++;
							break;
						case '(':
							state = 40;
							currPtr++;
							break;
						case ')':
							state = 41;
							currPtr++;
							break;
						case '.':
							state = 42;
							strToken[currLexemeLength] = firstBuffer[currPtr];
							currLexemeLength++;
							currPtr++;		
							break;
						default:
							// printf("Invalid Character encountered at state 0\n");
							currPtr++;
							state = 44;
							break;
					}
				
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
					else if(firstBuffer[currPtr] == '$')
					{
						fp = getStream(fp);
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
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
					if(currPtr == 0)
					{
						f = 1;
					}
					else
						currPtr--;  //retract twice
				}
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
				}
				else
				{
					// Give Lexical error
					state = 44;
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
					else if(firstBuffer[currPtr] == '$')
					{
						fp = getStream(fp);
					}
					else
					{
						state = 5;   //Retract
					}
				}
				else if(firstBuffer[currPtr] == 'e' || firstBuffer[currPtr] == 'E')
				{
					state = 6;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
				}
				else
				{
					//Send to trap state 
					state = 44;	
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
				}
				else
				{
					//Send to trap state 
					state = 44;
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
					if(firstBuffer[currPtr] == '$')
					{
						fp = getStream(fp);
					}
					else
						state = 9;
				}
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
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
				if((firstBuffer[currPtr] >= 'a' && firstBuffer[currPtr] <= 'z') || (firstBuffer[currPtr] >= 'A' && firstBuffer[currPtr] <= 'Z') || (firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <='9') || (firstBuffer[currPtr] == '_'))
				{
					while((firstBuffer[currPtr] >= 'a' && firstBuffer[currPtr] <= 'z') || (firstBuffer[currPtr] >= 'A' && firstBuffer[currPtr] <= 'Z') || (firstBuffer[currPtr] >= '0' && firstBuffer[currPtr] <='9') || (firstBuffer[currPtr] == '_'))
					{
						strToken[currLexemeLength] = firstBuffer[currPtr];
						currPtr++;
						currLexemeLength++;
					}
					if(firstBuffer[currPtr] == '$')
					{
						fp = getStream(fp);
						
						if(fp == NULL)
						{
							state = 12;
						}
					}
					else
						state = 12; //Retract
				}
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);

					if(fp == NULL)
					{
						state = 12;
					}
				}
				else
				{
					state = 12;    // Retract
				}
				break;

			case 12:
				strToken[currLexemeLength] = '\0';

				// If length of identifier exceeds 20 then generate error
				if(strlen(strToken) > 20)
				{
					// printf("Error: %d, length of identifier is greater than 20\n", currLine);
					state = 44;
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
				}
				else
				{
					//Generate Error Unexpected character
					state = 44;
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
				}
				else
				{
					//Generate Error Unexpected character
					state = 44;
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 32;
					}
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
				}
				else if(firstBuffer[currPtr] == '\n')
				{
					currLine++;
					currPtr++;
				}
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
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
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
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
				if(f == 1)
				{
					state = 43;
					strToken[currLexemeLength] = '.';
					currLexemeLength++;
					f = 0;
				}
				else if(firstBuffer[currPtr] == '.')
				{
					state = 43;
					strToken[currLexemeLength] = firstBuffer[currPtr];
					currPtr++;
					currLexemeLength++;
				}
				else if(firstBuffer[currPtr] == '$')
				{
					fp = getStream(fp);
						
					if(fp == NULL)
					{
						state = 44;
					}
				}
				else
				{
					//Generate error
					state = 44;
				}
				break;
			case 43:
				generateToken(tk,RANGEOP,"..",currLine);
				flag = 1;
				break;
			case 44:
				strToken[currLexemeLength] = '\0';
				generateToken(tk,ERROR,strToken,currLine);
				flag = 1;
				break;
		}
	}

	return tk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Hash Table implementation 


HT* generate_hashtable(int s){
   HT* hash=(HT*)malloc(s*sizeof(HT));
   hash->size=s;
   hash->arr=(hashentry**) malloc((s)*sizeof(hashentry*));
   for(int i=0;i<s;i++){
       hash->arr[i]=(hashentry*) malloc(sizeof(hashentry));
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
 
    Terminal arrTokens[] = {INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,PROGRAM,
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

// void printTable(HT* hash){
//     for(int i=0;i<hash->size;i++){
//         keyword* temp=hash->arr[i]->ptr;
//         printf("%d :\t",(i+1));
//         while(temp!=NULL){
//             printf("%s\t",temp->token);
//             temp=temp->k;
//         }
//         printf("\n");
//     }
// }

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
FILE* initializeLexer(char* testCaseFile)                         // Used in the ParseInputSourceCode Function
{
	FILE* fp = fopen(testCaseFile,"r");
	if(fp == NULL)
	{
		printf("Error in opening file\n");
	}
	else
	{
		printf("File opened with success\n\n\n");
	}

	firstBuffer = (char*)malloc(sizeof(char)*bufferSize);
	fp = getStream(fp);


	//Construct the hashtable
	char** keys = populate_keywords("keywords.txt");
    
    hashtable = generate_hashtable(41);
    hashtable = make_array(hashtable,keys,30);

    return fp;
}



void printTokens(char * testCaseFile)
{
	FILE* fp = fopen(testCaseFile,"r");
	if(fp == NULL)
	{
		printf("Error in opening file\n");
	}
	else
	{
		printf("File opened with success\n\n\n");
	}
	firstBuffer = (char*)malloc(sizeof(char)*bufferSize);
	fp = getStream(fp);


	//Construct the hashtable
	char** keys = populate_keywords("keywords.txt");
    
    hashtable = generate_hashtable(41);
    hashtable = make_array(hashtable,keys,30);

    Token* tk = (Token*)getNextToken(fp);

    while(endOfFile != 1)
    {
    	if(tk->name != COMMENTMARK)
    		printf("Line: %4d\t Lexeme: %25s\t  TokenName: %15s\n",tk->lineNo,tk -> lexeme, tokenNameEnumToString[tk->name]);
    	
		tk = getNextToken(fp);
    }
    endOfFile = 0;
    currLine = 1;
    // printf("Finished\n");
}


//// Remove comments from the input sourceCode file and print on the console


char *buffer;


FILE* fillBuffer(FILE* fp, char * buffer)
{
	// printf("Filling the buffer for removing comments -----------------------------------------\n");
	int numChar = 0;
	numChar = fread(buffer,sizeof(char),bufferSize-1,fp);
	// for(int i = 0; i < numChar; i++)
	// {
	// 	printf("%c\t", buffer[i]);
	// }
	// printf("\n");
	if(numChar > 0)
	{
		buffer[numChar] = '$';                                // adds a sentinel at the end
		return fp;
	}
	else
	{
		fclose(fp);
		// printf("No character left in file\n");
		return NULL;
	}
}


// Following function will remove comments from the file and creates a new file having comment-free code
void removeComments(char* testCaseFile)
{
	FILE* fptr1 = fopen(testCaseFile,"r");    //opens the testcase file in read mode

	int file1_ptr = 0;                        //pointer which traverse the buffer
    buffer = (char*)malloc(sizeof(char)*bufferSize);
	fptr1 = fillBuffer(fptr1,buffer);                 // Initially load the buffer stream
	int flag = 0;                             // this will become 1 when we are inside a comment
	int eof = 0;
	int state = 0;
	printf("----------------------- Started removing comments --------------------------------------------\n");

	while(1)
	{
		if(eof)
			break;
		switch(state)
		{
			case 0:
				if(buffer[file1_ptr] == '*')
				{
					file1_ptr++;
					state = 1;
				}
				else if(buffer[file1_ptr] == '$')
				{
					fptr1 = fillBuffer(fptr1,buffer);
					file1_ptr = 0;
					if(fptr1 == NULL)
						eof = 1;
				}
				else
				{
					printf("%c", buffer[file1_ptr]);
					file1_ptr++;
				}
			break;

			case 1:
				if(buffer[file1_ptr] == '*')
				{
					file1_ptr++;
					state = 2;
				}
				else if(buffer[file1_ptr] == '$')
				{
					fptr1 = fillBuffer(fptr1,buffer);
					file1_ptr = 0;
					if(fptr1 == NULL)
						eof = 1;
				}
				else
				{
					printf("%c", '*');
					state = 0;
				}
			break;

			case 2:
				if(buffer[file1_ptr] == '*')
				{
					state = 3;
					file1_ptr++;
				}
				else if(buffer[file1_ptr] == '$')
				{
					fptr1 = fillBuffer(fptr1,buffer);
					file1_ptr = 0;
					if(fptr1 == NULL)
						eof = 1;
				}
				else
				{
					file1_ptr++;
				}
			break;

			case 3:
				if(buffer[file1_ptr] == '*')
				{
					state = 0;
					file1_ptr++;
				}
				else if(buffer[file1_ptr] == '$')
				{
					fptr1 = fillBuffer(fptr1,buffer);
					file1_ptr = 0;
					if(fptr1 == NULL)
						eof = 1;
				}
				else
				{
					state = 2;
					file1_ptr ++;
				}
			break;
		}
	}
	printf("----------------------------- Finished Removing comments --------------------------------------\n\n\n");
}
