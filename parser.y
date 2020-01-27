%{ 
#include <stdio.h>
#include <stdlib.h>
#include "scanType.h"

#define YYDEBUG 1

extern int yylex();
extern FILE *yyin;

void yyerror(const char *msg)
{
      printf("ERROR(PARSER): %s\n", msg);
}

%}

%union {
    TokenData *tokenData;
    double value;
}

%token <tokenData> ID NUMCONST CHARCONST STRINGCONST BOOLCONST KEYWORD OPERATOR

%%

statementlist : statement 
              | statement statementlist '\n' statementlist
              | statement '\n' statementlist
              ;

statement : NUMCONST    { printf("line: %d type: NUMCONST value: %d\n", $1->linenum, $1->numValue); }
          | KEYWORD     { printf("line: %d type: KEYWORD value: %s\n", $1->linenum, $1->tokenstr); }
          | ID          { printf("line: %d type: ID value: %s\n", $1->linenum, $1->tokenstr); }
          | CHARCONST   { printf("line: %d type: CHARCONST value: %s\n", $1->linenum, $1->tokenstr); }
          | STRINGCONST { printf("line: %d type: STRINGCONST value: %s\n", $1->linenum, $1->tokenstr); }
          | BOOLCONST   { printf("line: %d type: BOOLCONST value: %s  internal value: %d\n", $1->linenum, $1->tokenstr, $1->numValue); }
          | OPERATOR    { printf("line: %d type: OPERATOR value: %s\n", $1->linenum, $1->tokenstr); }
          ;

%%

int main()
{
    yyparse();
    
    return 0;
}