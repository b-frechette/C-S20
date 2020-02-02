%{ 
    /* 
    *  parser.y
    *
    *  Flex file used to match with the stream of characters
    *  in a given file or stdin.
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include "scanType.h"

    #define YYDEBUG 1

    extern int yylex();
    extern int yyparse();
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

%token <tokenData> ID NUMCONST CHARCONST STRINGCONST BOOLCONST KEYWORD OPERATOR INVALIDCHAR

%%

statementlist : statement statementlist
              | statement
              ;

statement : NUMCONST    { printf("Line %d Token: %s Value: %d Input: %s\n", $1->linenum, $1->tokenname, $1->numValue, $1->tokenstr); }
          | KEYWORD     { printf("Line %d Token: %s\n", $1->linenum, $1->tokenname); }
          | ID          { printf("Line %d Token: %s Value: %s\n", $1->linenum, $1->tokenname, $1->tokenstr); }
          | CHARCONST   { printf("Line %d Token: %s Value: '%c' Input: %s\n", $1->linenum, $1->tokenname, $1->charValue, $1->tokenstr); }
          | INVALIDCHAR { printf("Line %d Token: %s Value: '%c' Input: %s\n", $1->linenum, $1->tokenname, $1->charValue, $1->tokenstr); }
          | STRINGCONST { printf("Line %d Token: %s Value: \'%s\'  Input: %s\n", $1->linenum, $1->tokenname, $1->stringValue, $1->tokenstr); }
          | BOOLCONST   { printf("Line %d Token: %s Value: %d  Input: %s\n", $1->linenum, $1->tokenname, $1->numValue, $1->tokenstr); }
          | OPERATOR    { printf("Line %d Token: %s\n", $1->linenum, $1->tokenname); }
          | '\n'
          ;

%%

int main(int argc, char **argv)
{
    FILE *filename;
    //yydebug = 1;

    //Check if a filename was passed
    if(argc > 1)
    {
        filename  = fopen(argv[1], "r");

        //Check if file is valid
        if(filename == NULL)
        {
            printf("ERROR(ARGLIST): file \"%s\" could not be opened.\n", argv[1]);
            exit(1);
        }
        else
        {
           yyin = filename; 
        } 
    }
    //Otherwise use stdin
    else
    {
        yyin = stdin;
    }

    yyparse();
    
    return 0;
}