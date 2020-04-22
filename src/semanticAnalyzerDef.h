/* GROUP No. 29
        2017A7PS0004P -- SUBHAM KUMAR DASH
        2017A7PS0036P -- RAHUL JHA
        2017A7PS0084P -- ANIRUDDHA JAYANT KARAJGI
        2017A7PS0128P -- MEET KANANI
        2017A7PS0193P -- AYUSH GARG
*/
#ifndef _semanticAnalyzerDef_
#define _semanticAnalyzerDef_

typedef struct err
{
    int line_no;
    char *err;
    struct err *next;
} error;

typedef struct err_list
{
    error *head;
    int err_total;
} error_list;

#endif
