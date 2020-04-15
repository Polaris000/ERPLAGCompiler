#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

HT *generate_hashtable(int s)
{
    HT *hash = (HT *)malloc(s * sizeof(HT));
    hash->size = s;
    hash->arr = (hashentry **)malloc((s) * sizeof(hashentry *));
    for (int i = 0; i < s; i++)
    {
        hash->arr[i] = (hashentry *)malloc(sizeof(hashentry));
        hash->arr[i]->chain_size = 0;
        hash->arr[i]->ptr = NULL;
    }
    return hash;
}

HT *make_array(HT *hash, char **arr, int s)
{
    for (int i = 0; i < s; i++)
    {
        int j = 0, hash_val = 0;
        while (arr[i][j] != '\0')
        {
            hash_val = (((hash_val * 47) % hash->size) + (arr[i][j] % hash->size)) % hash->size;
            j++;
        }

        Terminal arrTokens[] = {INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM,
                                GET_VALUE, PRINT, USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR,
                                FOR, IN, SWITCH, CASE, BREAK, DEFAULT, WHILE};

        keyword *new = (keyword *)malloc(sizeof(keyword));
        new->token = (char *)malloc((j + 1) * sizeof(char));
        new->token = arr[i];
        new->name = arrTokens[i];
        new->k = NULL;

        if (hash->arr[hash_val]->ptr != NULL)
        {
            keyword *temp = hash->arr[hash_val]->ptr;
            while (temp->k != NULL)
                temp = temp->k;
            temp->k = new;
            hash->arr[hash_val]->chain_size += 1;
        }
        else
        {
            hash->arr[hash_val]->ptr = new;
            hash->arr[hash_val]->chain_size = 1;
        }
    }
    return hash;
}

void printTable(HT *hash)
{
    for (int i = 0; i < hash->size; i++)
    {
        keyword *temp = hash->arr[i]->ptr;
        printf("%d :\t", (i + 1));
        while (temp != NULL)
        {
            printf("%s\t", temp->token);
            temp = temp->k;
        }
        printf("\n");
    }
}

keyword *lookup(HT *hash, char *key)
{
    int j = 0, hash_val = 0;
    while (key[j] != '\0')
    {
        hash_val = (((hash_val * 47) % hash->size) + (key[j] % hash->size)) % hash->size;
        j++;
    }

    keyword *temp = hash->arr[hash_val]->ptr;
    while (temp != NULL)
    {
        if (strcmp(temp->token, key) == 0)
            return temp;
        temp = temp->k;
    }
    return NULL;
}

HT *insert(HT *hash, char *key)
{
    int j = 0, hash_val = 0;
    while (key[j] != '\0')
    {
        hash_val = (((hash_val * 47) % hash->size) + (key[j] % hash->size)) % hash->size;
        j++;
    }

    keyword *new = (keyword *)malloc(sizeof(keyword));
    new->token = (char *)malloc(j * sizeof(char));
    new->token = key;
    new->k = NULL;

    if (hash->arr[hash_val]->ptr != NULL)
    {
        keyword *temp = hash->arr[hash_val]->ptr;
        if (strcmp(temp->token, key) == 0)
        {
            printf("Keyword of the same name already exists!\n");
            return hash;
        }
        while (temp->k != NULL)
        {
            temp = temp->k;
            if (strcmp(temp->token, key) == 0)
            {
                printf("Keyword of the same name already exists!\n");
                return hash;
            }
        }
        temp->k = new;
        hash->arr[hash_val]->chain_size += 1;
    }
    else
    {
        hash->arr[hash_val]->ptr = new;
        hash->arr[hash_val]->chain_size = 1;
    }
}

char **populate_keywords(char *filename)
{
    FILE *ptr;
    ptr = fopen(filename, "r");
    int size = 0;

    char s[30];
    while (fgets(s, 30, ptr) != NULL)
    {
        size++;
    }

    fseek(ptr, 0, SEEK_SET);
    int str_len[size];
    char c = fgetc(ptr);
    int i = 0;
    while (c != EOF)
    {
        int len = 0;
        while (c != '\n')
        {
            len++;
            c = fgetc(ptr);
        }
        str_len[i] = len + 1;
        i++;
        c = fgetc(ptr);
    }

    fseek(ptr, 0, SEEK_SET);
    char **table = (char **)malloc(size * sizeof(char *));
    for (i = 0; i < size; i++)
    {
        table[i] = (char *)malloc(str_len[i] * sizeof(char));
        fgets(table[i], str_len[i], ptr);
        table[i][str_len[i] - 1] = '\0';
        fgetc(ptr);
    }
    fclose(ptr);
    return table;
}
// int main(){
//     char** keys=populate_keywords("keywords.txt");

//     HT* hashtable=generate_hashtable(41);
//     hashtable=make_array(hashtable,keys,30);
//     insert(hashtable,"AND");
//     insert(hashtable,"integer");
//     printTable(hashtable);
//     // printf("%d\n",lookup(hashtable,"OR"));
//     return 0;
// }
