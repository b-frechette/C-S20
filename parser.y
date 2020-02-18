%{ 
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
}

// token specifies the token classes from the scanner
%token <tokenData> NUMCONST CHARCONST STRINGCONST TRUE FALSE ID 
%token INVALIDCHAR
%token STATIC INT BOOL CHAR ELSIF THEN IF ELSE WHILE DO FOREVER LOOP RETURN BREAK OR AND NOT
%token SEMI COMMA LBRACKET RBRACKET COLON LPAREN RPAREN LCURL RCURL ASSIGN
%token GRT LESS PLUS MINUS MUL DIV RAND TRUNC RANGE ADDASS SUBASS MULASS DIVASS INC DEC LESSEQ GRTEQ EQ NOTEQ

%%
program                 : declarationList
                        ;

declarationList         : declarationList declaration
                        | declaration
                        ;

declaration             : varDeclaration
                        | funDeclaration
                        ;
        
varDeclaration          : typeSpecifier varDeclList SEMI
                        ;

scopedVarDeclaration    : scopedTypeSpecifier varDeclList SEMI
                        ;

varDeclList             : varDeclList COMMA varDeclInitialize
                        | varDeclInitialize
                        ;

varDeclInitialize       : varDeclId
                        | varDeclId COLON simpleExpression
                        ;

varDeclId               : ID 
                        | ID LBRACKET NUMCONST RBRACKET
                        ;

scopedTypeSpecifier     : STATIC typeSpecifier
                        | typeSpecifier
                        ;

typeSpecifier           : INT
                        | BOOL
                        | CHAR
                        ;

funDeclaration          : typeSpecifier ID LPAREN params RPAREN statement
                        | ID LPAREN params RPAREN statement
                        ;

params                  : paramList
                        | /* epsilon */
                        ;

paramList               : paramList SEMI paramTypeList
                        | paramTypeList
                        ;

paramTypeList           : typeSpecifier paramIdList
                        ;

paramIdList             : paramIdList COMMA paramId 
                        | paramId
                        ;

paramId                 : ID
                        | ID LBRACKET RBRACKET
                        ;

statement               : matched
                        | unmatched
                        ;

matched                 : ifmatched
                        | expressionStmt
                        | compoundStmt
                        | returnStmt
                        | breakStmt
                        | matchediterationStmt
                        ;

unmatched               : ifunmatched
                        | unmatchediterationStmt
                        ;

expressionStmt          : expression SEMI
                        | SEMI
                        ;

compoundStmt            : LCURL localDeclarations statementList RCURL
                        ;

localDeclarations       : localDeclarations scopedVarDeclaration
                        | /* epsilon */
                        ;

statementList           : statementList statement
                        | /* epsilon */
                        ;

elsifMatched            : elsifStmt elseMatched
                        ;

elsifUnmatched          : elsifStmt elseUnmatched
                        ;

elsifStmt               : elsifStmt ELSIF simpleExpression THEN matched
                        | /* epsilon */
                        ;

elseMatched             : ELSE matched
                        ;

elseUnmatched           : ELSE unmatched
                        ;

ifmatched               : IF simpleExpression THEN matched elsifMatched
                        ;

ifunmatched             : IF simpleExpression THEN matched 
                        | IF simpleExpression THEN unmatched 
                        | IF simpleExpression THEN matched elsifUnmatched
                        ;

iterationRange          : ID ASSIGN simpleExpression RANGE simpleExpression
                        | ID ASSIGN simpleExpression RANGE simpleExpression COLON simpleExpression
                        ;

matchediterationStmt    : WHILE simpleExpression DO matched
                        | LOOP FOREVER matched
                        | LOOP iterationRange DO matched
                        ;

unmatchediterationStmt  : WHILE simpleExpression DO unmatched
                        | LOOP FOREVER unmatched
                        | LOOP iterationRange DO unmatched
                        ;

returnStmt              : RETURN SEMI
                        | RETURN expression SEMI
                        ;

breakStmt               : BREAK SEMI
                        ;

expression              : mutable ASSIGN expression
                        | mutable ADDASS expression
                        | mutable SUBASS expression
                        | mutable MULASS expression
                        | mutable DIVASS expression
                        | mutable INC
                        | mutable DEC 
                        | simpleExpression
                        ;

simpleExpression        : simpleExpression OR andExpression
                        | andExpression
                        ;

andExpression           : andExpression AND unaryRelExpression
                        | unaryRelExpression
                        ;

unaryRelExpression      : NOT unaryRelExpression
                        | relExpression 
                        ;

relExpression           : relExpression relop sumExpression
                        | sumExpression
                        ;

relop                   : LESSEQ
                        | LESS
                        | GRT
                        | GRTEQ
                        | EQ
                        | NOTEQ
                        ;

sumExpression           : sumExpression sumop mulExpression
                        | mulExpression
                        ;

sumop                   : PLUS
                        | MINUS
                        ;

mulExpression           : mulExpression mulop unaryExpression
                        | unaryExpression
                        ;

mulop                   : MUL
                        | DIV
                        | TRUNC
                        ;

unaryExpression         : unaryop unaryExpression 
                        | factor
                        ;

unaryop                 : MINUS
                        | MUL
                        | RAND
                        ;

factor                  : immutable                 //$$ = $1
                        | mutable                   //$$ = $1
                        ;

mutable                 : ID 
                        | mutable LBRACKET expression RBRACKET
                        ;

immutable               : LPAREN expression RPAREN  //$$ = $2
                        | ID call
                        | constant                  //$$ = $1
                        ;

call                    : LPAREN args RPAREN        //$$ = $2
                        ;

args                    : argList                   //$$ = $1
                        | /* epsilon */
                        ;

argList                 : argList COMMA expression
                        | expression                //$$ = $1
                        ;

constant                : NUMCONST          
                        | CHARCONST         
                        | STRINGCONST       
                        | TRUE              
                        | FALSE             
                        ;

%%

int main(int argc, char **argv)
{
    int i;

    yydebug = 1;

    FILE *filename;
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
    yyparse();   // call the parser

    return 0;
}