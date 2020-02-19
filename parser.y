%{ 
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "treeUtils.h"
#include "scanType.h"
#include "ourGetopt.h"

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
    TreeNode *tree;
}

// token specifies the token classes from the scanner
%token <tokenData> NUMCONST CHARCONST STRINGCONST FALSE TRUE ID
%token <tokenData> GRT LESS PLUS MINUS MUL DIV RAND TRUNC RANGE ADDASS SUBASS MULASS DIVASS INC DEC LESSEQ GRTEQ EQ NOTEQ ASSIGN
%token INVALIDCHAR  
%token STATIC INT BOOL CHAR ELSIF THEN IF ELSE WHILE DO FOREVER LOOP RETURN BREAK OR AND NOT
%token SEMI COMMA LBRACKET RBRACKET COLON LPAREN RPAREN LCURL RCURL 

%type <tree> constant mutable

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
                            //{ $$ = $1; }
                        | unmatched
                            //{ $$ = $1; }
                        ;

matchedelsif            : ELSIF simpleExpression THEN matched matchedelsif
                        | ELSE matched
                        ;

unmatchedelsif          : ELSIF simpleExpression THEN matched unmatchedelsif
                        | ELSE unmatched
                        | /*epsilon*/
                        ;

iterationRange          : ID ASSIGN simpleExpression RANGE simpleExpression
                        | ID ASSIGN simpleExpression RANGE simpleExpression COLON simpleExpression
                        ;

matched                 : IF simpleExpression THEN matched matchedelsif
                        | WHILE simpleExpression DO matched
                        | LOOP FOREVER matched
                        | LOOP iterationRange DO matched
                        | other_statements
                            //{ $$ = $1; }
                        ;

unmatched               : IF simpleExpression THEN unmatched 
                        | IF simpleExpression THEN matched unmatchedelsif
                        | WHILE simpleExpression DO unmatched
                        | LOOP FOREVER unmatched
                        | LOOP iterationRange DO unmatched
                        ;

other_statements        : expressionStmt
                        | compoundStmt
                        | returnStmt
                        | breakStmt
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
                            //{ $$ = $1; }
                        ;

simpleExpression        : simpleExpression OR andExpression
                        | andExpression  
                            //{ $$ = $1; }
                        ;

andExpression           : andExpression AND unaryRelExpression
                        | unaryRelExpression
                            //{ $$ = $1; }
                        ;

unaryRelExpression      : NOT unaryRelExpression
                        | relExpression
                            //{ $$ = $1; }
                        ;

relExpression           : relExpression relop sumExpression
                        | sumExpression  
                            //{ $$ = $1; }
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
                            //{ $$ = $1; }
                        ;

sumop                   : PLUS
                        | MINUS
                        ;

mulExpression           : mulExpression mulop unaryExpression
                        | unaryExpression
                            //{ $$ = $1; }
                        ;

mulop                   : MUL
                        | DIV
                        | TRUNC
                        ;

unaryExpression         : unaryop unaryExpression 
                        | factor
                            //{ $$ = $1; }
                        ;

unaryop                 : MINUS
                        | MUL
                        | RAND
                        ;

factor                  : immutable
                            //{ $$ = $1; }
                        | mutable
                            //{ $$ = $1; }                   
                        ;

mutable                 : ID 
                            { 
                                $$ = newExpNode(IdK); 
                                $$->attr.name = $1->tokenname;
                                //$$->expType = Char;
                                $$->lineno = $1->linenum;
                            }
                        | mutable LBRACKET expression RBRACKET
                        ;

immutable               : LPAREN expression RPAREN  //$$ = $2
                        | ID call
                        | constant                 
                            //{ $$ = $1; }
                        ;

call                    : LPAREN args RPAREN        //$$ = $2
                        ;

args                    : argList                
                            //{ $$ = $1; }
                        | /* epsilon */
                        ;

argList                 : argList COMMA expression
                        | expression 
                            //{ $$ = $1; }
                        ;

constant                : NUMCONST  
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.value = $1->numValue;
                                $$->expType = Integer;
                                $$->lineno = $1->linenum;
                            }        
                        | CHARCONST   
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.cvalue = $1->charValue;
                                $$->expType = Char;
                                $$->lineno = $1->linenum;
                            }      
                        | STRINGCONST    
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.string = $1->stringValue;
                                //$$->expType = Char;
                                $$->lineno = $1->linenum;
                            }   
                        | TRUE         
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.value = $1->numValue;
                                $$->expType = Boolean;
                                $$->lineno = $1->linenum; 
                            }     
                        | FALSE     
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.value = $1->numValue;
                                $$->expType = Boolean;
                                $$->lineno = $1->linenum; 
                            }        
                        ;

%%

int main(int argc, char **argv)
{
    extern int opterr;
    extern int optind;
    extern char *optarg;
    int c, dflg = 0, pflg = 0, filerr = 0;
    char *oarg = NULL;
    FILE *filename;

    while ((c = ourGetopt(argc, argv, (char *)":dp")) != EOF)
    {
        switch(c)
        {
            case 'd':
                ++dflg;
                break;
            
            case 'p':
                ++pflg;
                break;

            case '?':
                printf("Error\n");
                exit(1);
                break;

            default:
                printf("default\n");
                break;
        }
    }

    if(dflg) 
    {
        printf("d\n");
        yydebug = 1;
    }

    if(pflg) 
    {
        printf("p\n");
    }

    if (optind < argc) 
    {
        (void)printf("file: %s\n", argv[optind]);
        oarg = strdup(argv[optind]);
        filerr++;
        optind++;
    }

    if(filerr == 1)
    {
        filename = fopen(oarg, "r");

        if(filename == NULL)
        {
            printf("ERROR(ARGLIST): file \"%s\" could not be opened.\n", oarg);
            exit(1);
        }
        else
        {
            printf("file\n");
            yyin = filename;
        }
    }
    else
    {
        printf("stdin\n");
        yyin = stdin;
    }

    yyparse();

    return 0;
}