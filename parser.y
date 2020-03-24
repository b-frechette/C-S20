%{ 
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "treeUtils.h"
#include "scanType.h"
#include "printtree.h"
#include "ourGetopt.h"
#include "semantic.h"

#define YYDEBUG 1

int numErrors, numWarnings;

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *msg)
{
      printf("ERROR(PARSER): %s\n", msg);
}

/* descr: Recursively add sibling to end of the list
*
* param: TreeNode -> Pointer to the node to add the sibling
* param: TreeNode -> Pointer to the sibling node
* return: void
*/
void addSibling(TreeNode *t1, TreeNode *t2)
{
    if(t1 != NULL) 
    {
        if(t1->sibling == NULL)
        {
            t1->sibling = t2;
        }
        else
        {
            addSibling(t1->sibling, t2);
        }
    }
    else
    { printf("error\n"); }
}

/* descr: Recursively add the type for a var or param kind. This is 
* done by adding the unidentified type to the id assignment for 
* param or var, then checking for it here to tell whether it needs 
* to be changed.
*
* param: VarData -> Struct that contains the type and static data
* param: TreeNode -> Pointer to the param or var node to change
* return: void
*/
void setType(VarData v, TreeNode *t1)
{
    if(t1 != NULL)
    {
        if(t1->expType == UndefinedType)
        {
            t1->expType = v.expType;
            t1->isStatic = v.isStatic;

            setType(v, t1->sibling);
        }
        else
        { return; }
    }
}

TreeNode * savedTree;

%}

%union 
{
    VarData vardata; 
    TokenData *tokenData;
    TreeNode *tree;
}


/*******************
* TERMINALS        *
*******************/

/* CONSTANTS */
%token <tokenData> NUMCONST 
%token <tokenData> CHARCONST 
%token <tokenData> STRINGCONST  
%token <tokenData> BOOLCONST

/* ID */
%token <tokenData> ID

/* OPERATORS */
%token <tokenData> GRT 
%token <tokenData> LESS 
%token <tokenData> PLUS 
%token <tokenData> MINUS 
%token <tokenData> MUL 
%token <tokenData> DIV 
%token <tokenData> RAND 
%token <tokenData> TRUNC
%token <tokenData> RANGE
%token <tokenData> ADDASS
%token <tokenData> SUBASS
%token <tokenData> MULASS
%token <tokenData> DIVASS
%token <tokenData> INC
%token <tokenData> DEC
%token <tokenData> LESSEQ
%token <tokenData> GRTEQ
%token <tokenData> EQ
%token <tokenData> NOTEQ
%token <tokenData> ASSIGN
%token <tokenData> OR
%token <tokenData> AND
%token <tokenData> NOT

/* TYPE */
%token <tokenData> STATIC 
%token <tokenData> INT
%token <tokenData> BOOL
%token <tokenData> CHAR

/* STATEMENTS */
%token <tokenData> ELSIF
%token <tokenData> THEN
%token <tokenData> IF
%token <tokenData> ELSE
%token <tokenData> WHILE
%token <tokenData> DO
%token <tokenData> FOREVER
%token <tokenData> LOOP
%token <tokenData> RETURN
%token <tokenData> BREAK

/* PUNCTUATION */
%token <tokenData> SEMI
%token <tokenData> COMMA
%token <tokenData> LBRACKET
%token <tokenData> RBRACKET
%token <tokenData> COLON
%token <tokenData> LPAREN
%token <tokenData> RPAREN
%token <tokenData> LCURL
%token <tokenData> RCURL 

/* UNSURE */
%token INVALIDCHAR 

/*******************
* NONTERMINALS     *
*******************/

/* VARDATA */
%type <vardata> typeSpecifier
%type <vardata> scopedTypeSpecifier

/* TOKENDATA */
%type <tokenData> unaryop
%type <tokenData> mulop
%type <tokenData> sumop
%type <tokenData> relop

/* TREE */

%type <tree> constant
%type <tree> iterationId

%type <tree> argList                
%type <tree> args
%type <tree> call 
%type <tree> immutable
%type <tree> mutable
%type <tree> factor

%type <tree> unaryExpression
%type <tree> mulExpression
%type <tree> sumExpression
%type <tree> relExpression
%type <tree> unaryRelExpression
%type <tree> andExpression
%type <tree> simpleExpression
%type <tree> expression

%type <tree> breakStmt
%type <tree> returnStmt
%type <tree> statementList
%type <tree> localDeclarations
%type <tree> compoundStmt
%type <tree> expressionStmt
%type <tree> other_statements
%type <tree> unmatched
%type <tree> matched
%type <tree> iterationRange
%type <tree> unmatchedelsif
%type <tree> matchedelsif
%type <tree> statement

%type <tree> paramId
%type <tree> paramTypeList
%type <tree> paramIdList
%type <tree> paramList
%type <tree> params

%type <tree> funDeclaration

%type <tree> varDeclId
%type <tree> varDeclInitialize
%type <tree> varDeclList
%type <tree> scopedVarDeclaration
%type <tree> varDeclaration
%type <tree> declaration
%type <tree> declarationList
 
%%
program                 : declarationList
                            { savedTree = $1; }
                        ;

declarationList         : declarationList declaration
                            {
                                if($1 == NULL)
                                {
                                    $$ = $2;
                                }
                                else
                                {
                                    addSibling($1, $2);
                                    $$ = $1;
                                }
                            }
                        | declaration
                            { $$ = $1; }
                        ;

declaration             : varDeclaration
                            { $$ = $1; }
                        | funDeclaration
                            { $$ = $1; }
                        ;

varDeclaration          : typeSpecifier varDeclList SEMI
                            {
                                setType($1, $2);
                                $$ = $2;
                            }
                        ;

scopedVarDeclaration    : scopedTypeSpecifier varDeclList SEMI
                            {
                                setType($1, $2);
                                $$ = $2;
                            }
                        ;

varDeclList             : varDeclList COMMA varDeclInitialize
                            {
                                if($1 == NULL)
                                {
                                    $$ = $3;
                                }
                                else
                                {
                                    addSibling($1, $3);
                                    $$ = $1;
                                }
                            }
                        | varDeclInitialize
                            { $$ = $1; }
                        ;

varDeclInitialize       : varDeclId
                            { $$ = $1; }
                        | varDeclId COLON simpleExpression
                            {
                                $$ = $1;
                                $$->child[0] = $3;
                            }
                        ;

varDeclId               : ID 
                            {
                                $$ = newDeclNode(VarK);
                                $$->lineno = $1->linenum;
                                $$->attr.name = $1->tokenstr;
                                $$->expType = UndefinedType;
                            }
                        | ID LBRACKET NUMCONST RBRACKET
                            {
                                $$ = newDeclNode(VarK);
                                $$->lineno = $1->linenum;
                                $$->attr.name = $1->tokenstr;
                                $$->isArray = true;
                                $$->expType = UndefinedType;
                            }
                        ;

scopedTypeSpecifier     : STATIC typeSpecifier
                            {
                                $$ = $2;
                                $$.isStatic = true;
                            }
                        | typeSpecifier
                            { $$ = $1; }
                        ;

typeSpecifier           : INT
                            {
                                $$.linenum = $1->linenum;
                                $$.expType = Integer;
                            }
                        | BOOL 
                            {
                                $$.linenum = $1->linenum;
                                $$.expType = Boolean;
                            }
                        | CHAR 
                            {
                                $$.linenum = $1->linenum;
                                $$.expType = Char;
                            }
                        ;

funDeclaration          : typeSpecifier ID LPAREN params RPAREN statement
                            {
                                $$ = newDeclNode(FuncK);
                                $$->child[0] = $4;
                                $$->child[1] = $6;
                                $$->expType = $1.expType;
                                $$->attr.name = $2->tokenstr;
                                $$->lineno = $1.linenum;
                            }
                        | ID LPAREN params RPAREN statement
                            {
                                $$ = newDeclNode(FuncK);
                                $$->child[0] = $3;
                                $$->child[1] = $5;
                                $$->attr.name = $1->tokenstr;
                                $$->expType = Void;
                                $$->lineno = $1->linenum;
                            }
                        ;

/**** EPSILON ****/
params                  : paramList
                            { $$ = $1; }
                        | /* epsilon */
                            { $$ = NULL; }
                        ;

paramList               : paramList SEMI paramTypeList
                            {
                                if($1 == NULL)
                                {
                                    $$ = $3;
                                }
                                else
                                {
                                    $$ = $1;
                                    addSibling($1, $3);
                                }
                            }
                        | paramTypeList
                            { $$ = $1; }
                        ;

paramTypeList           : typeSpecifier paramIdList
                            {
                                setType($1, $2);
                                $$ = $2;
                            }
                        ;

paramIdList             : paramIdList COMMA paramId 
                            {
                                if($1 == NULL)
                                {
                                    $$ = $3;
                                }
                                else
                                {
                                    addSibling($1, $3);
                                    $$ = $1;
                                }
                            }
                        | paramId
                            { $$ = $1; }
                        ;

paramId                 : ID
                            {
                                $$ = newDeclNode(ParamK);
                                $$->attr.name = $1->tokenstr;
                                $$->lineno = $1->linenum;
                                $$->expType = UndefinedType;
                            }
                        | ID LBRACKET RBRACKET
                            {
                                $$ = newDeclNode(ParamK);
                                $$->attr.name = $1->tokenstr;
                                $$->isArray = true;
                                $$->lineno = $1->linenum;
                                $$->expType = UndefinedType;
                            }
                        ;

statement               : matched
                            { $$ = $1; }
                        | unmatched
                            { $$ = $1; }
                        ;

matchedelsif            : ELSIF simpleExpression THEN matched matchedelsif
                            {
                                $$ = newStmtNode(ElsifK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->child[2] = $5;
                                $$->lineno = $1->linenum;
                            }
                        | ELSE matched
                            { $$ = $2; }
                        ;

/**** EPSILON ****/
unmatchedelsif          : ELSIF simpleExpression THEN matched unmatchedelsif
                            {
                                $$ = newStmtNode(ElsifK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->child[2] = $5;
                                $$->lineno = $1->linenum;
                            }
                        | ELSE unmatched
                            { $$ = $2; }
                        | /*epsilon*/
                            { $$ = NULL; }
                        ;

/**** QUESTION ****/
iterationRange          : ASSIGN simpleExpression RANGE simpleExpression
                            {
                                $$ = newStmtNode(RangeK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;

                                //Solution for Exp node that appears in test cases - Ask why it's there
                                TreeNode *t = newExpNode(ConstantK);
                                t->attr.value = 1;
                                t->attr.name = "1";
                                t->expType = Integer;
                                t->lineno = $3->linenum;

                                $$->child[2] = t;
                                $$->lineno = $3->linenum;
                            }
                        | ASSIGN simpleExpression RANGE simpleExpression COLON simpleExpression
                            { 
                                $$ = newStmtNode(RangeK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->child[2] = $6;
                                $$->lineno = $1->linenum;
                            }
                        ;

iterationId             : ID    
                            { 
                                //$$ = newExpNode(IdK); 
                                $$ = newDeclNode(VarK);
                                $$->attr.name = $1->tokenstr;
                                $$->expType = Integer;
                                $$->lineno = $1->linenum;
                            }
                        ;

/**** QUESTION ****/
matched                 : IF simpleExpression THEN matched matchedelsif
                            {
                                $$ = newStmtNode(IfK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->child[2] = $5;
                                $$->lineno = $1->linenum;
                            }
                        | WHILE simpleExpression DO matched
                            {
                                $$ = newStmtNode(WhileK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->lineno = $1->linenum;
                            }
                        | LOOP FOREVER matched
                            {
                                $$ = newStmtNode(LoopForeverK);
                                $$->child[1] = $3;                  //Why is the child 1 and not 0 in tests?
                                $$->lineno = $1->linenum;
                            }
                        | LOOP iterationId iterationRange DO matched
                            {
                                $$ = newStmtNode(LoopK);
                                $$->child[0] = $2;
                                $$->child[1] = $3;
                                $$->child[2] = $5;
                                $$->lineno = $1->linenum;
                            }
                        | other_statements
                            { $$ = $1; }
                        ;

/**** QUESTION ****/
unmatched               : IF simpleExpression THEN unmatched 
                            {
                                $$ = newStmtNode(IfK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->lineno = $1->linenum;
                            }
                        | IF simpleExpression THEN matched unmatchedelsif
                            {
                                $$ = newStmtNode(IfK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->child[2] = $5;
                                $$->lineno = $1->linenum;
                            }
                        | WHILE simpleExpression DO unmatched
                            {
                                $$ = newStmtNode(WhileK);
                                $$->child[0] = $2;
                                $$->child[1] = $4;
                                $$->lineno = $1->linenum;
                            }
                        | LOOP FOREVER unmatched
                            {
                                $$ = newStmtNode(LoopForeverK);
                                $$->child[1] = $3;                  //Why is the child 1 and not 0 in tests?
                                $$->lineno = $1->linenum;
                            }
                        | LOOP iterationId iterationRange DO unmatched
                            {
                                $$ = newStmtNode(LoopK);
                                $$->child[0] = $2;
                                $$->child[1] = $3;
                                $$->child[2] = $5;
                                $$->lineno = $1->linenum;
                            }
                        ;

other_statements        : expressionStmt
                            { $$ = $1; }
                        | compoundStmt
                            { $$ = $1; }
                        | returnStmt
                            { $$ = $1; }
                        | breakStmt
                            { $$ = $1; }
                        ;

expressionStmt          : expression SEMI
                            { 
                                $$ = $1; 
                                $$->isExp = true;
                            }
                        | SEMI
                            { $$ = NULL; }
                        ;

compoundStmt            : LCURL localDeclarations statementList RCURL
                            {
                                $$ = newStmtNode(CompoundK);
                                $$->child[0] = $2;
                                $$->child[1] = $3;
                                $$->lineno = $1->linenum;
                            }
                        ;

/**** EPSILON ****/
localDeclarations       : localDeclarations scopedVarDeclaration
                            {
                                if($1 == NULL)
                                {
                                    $$ = $2;
                                }
                                else
                                {
                                    addSibling($1, $2);
                                    $$ = $1;
                                }
                            }
                        | /* epsilon */
                            { $$ = NULL; }
                        ;

/**** EPSILON ****/
statementList           : statementList statement
                            {
                                if($1 == NULL)
                                {
                                    $$ = $2;
                                }
                                else
                                {
                                    addSibling($1, $2);
                                    $$ = $1;
                                }
                            }
                        | /* epsilon */
                            { $$ = NULL; } 
                        ;

returnStmt              : RETURN SEMI
                            {
                                $$ = newStmtNode(ReturnK);
                                $$->lineno = $1->linenum;
                            }
                        | RETURN expression SEMI
                            {
                                $$ = newStmtNode(ReturnK);
                                $$->child[0] = $2;
                                $$->lineno = $1->linenum;
                            }
                        ;

breakStmt               : BREAK SEMI
                            {
                                $$ = newStmtNode(BreakK);
                                $$->lineno = $1->linenum;
                            }
                        ;

expression              : mutable ASSIGN expression
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 1;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mutable ADDASS expression
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->child[1] = $3; 
                                $$->op = 2; 
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mutable SUBASS expression
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                                $$->op = 3;  
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mutable MULASS expression
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->child[1] = $3; 
                                $$->op = 4; 
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mutable DIVASS expression
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 5;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mutable INC
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->op = 6;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mutable DEC 
                            {
                                $$ = newExpNode(AssignK);
                                $$->child[0] = $1;
                                $$->op = 7;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | simpleExpression       
                            { $$ = $1; }
                        ;

simpleExpression        : simpleExpression OR andExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 1;         
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | andExpression  
                            { $$ = $1; }
                        ;

andExpression           : andExpression AND unaryRelExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 2;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | unaryRelExpression
                            { $$ = $1; }
                        ;

unaryRelExpression      : NOT unaryRelExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $2;
                                $$->op = 3;
                                $$->attr.name = $1->tokenstr; 
                                $$->lineno = $1->linenum;
                            }
                        | relExpression
                            { $$ = $1; }
                        ;

relExpression           : relExpression relop sumExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 4;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | sumExpression  
                            { $$ = $1; }
                        ;

relop                   : LESSEQ
                            { $$ = $1; }
                        | LESS
                            { $$ = $1; }
                        | GRT
                            { $$ = $1; }
                        | GRTEQ
                            { $$ = $1; }
                        | EQ
                            { $$ = $1; }
                        | NOTEQ
                            { $$ = $1; }
                        ;

sumExpression           : sumExpression sumop mulExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 5;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | mulExpression
                            { $$ = $1; }
                        ;

sumop                   : PLUS
                            { $$ = $1; }
                        | MINUS
                            { $$ = $1; }
                        ;

mulExpression           : mulExpression mulop unaryExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;  
                                $$->op = 6;
                                $$->attr.name = $2->tokenstr; 
                                $$->lineno = $2->linenum;
                            }
                        | unaryExpression
                            { $$ = $1; }
                        ;

mulop                   : MUL
                            { $$ = $1; }
                        | DIV
                            { $$ = $1; }
                        | TRUNC
                            { $$ = $1; }
                        ;

unaryExpression         : unaryop unaryExpression
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $2;
                                $$->op = 7;
                                $$->attr.name = $1->tokenstr;
                                $$->lineno = $1->linenum;
                            }
                        | factor
                            { $$ = $1; }
                        ;

unaryop                 : MINUS
                            { $$ = $1; }
                        | MUL
                            { $$ = $1; }
                        | RAND
                            { $$ = $1; }
                        ;

factor                  : immutable
                            { $$ = $1; }
                        | mutable
                            { $$ = $1; }                   
                        ;

mutable                 : ID 
                            { 
                                $$ = newExpNode(IdK); 
                                $$->attr.name = $1->tokenstr;
                                $$->lineno = $1->linenum;
                            }
                        | mutable LBRACKET expression RBRACKET
                            {
                                $$ = newExpNode(OpK);
                                $$->child[0] = $1;
                                $$->child[1] = $3;
                                $$->op = 8;
                                $$->attr.name = $2->tokenstr;
                                $$->lineno = $2->linenum;
                            }
                        ;

immutable               : LPAREN expression RPAREN
                            { $$ = $2; }
                        | ID call
                            {
                                $$ = newExpNode(CallK);
                                $$->child[0] = $2;  
                                $$->attr.name = $1->tokenstr;
                                $$->lineno = $1->linenum;            
                            }
                        | constant                 
                            { $$ = $1; }
                        ;

call                    : LPAREN args RPAREN
                            { $$ = $2; }
                        ;

/**** EPSILON ****/
args                    : argList                
                            { $$ = $1; }
                        | /* epsilon */
                            { $$ = NULL; }
                        ;

argList                 : argList COMMA expression
                            {
                                if($1 == NULL)
                                {
                                    $$ = $3;
                                }
                                else
                                {
                                    addSibling($1, $3);
                                    $$ = $1;
                                }
                            }
                        | expression 
                            { $$ = $1; }
                        ;

/**** CHECK ****/
constant                : NUMCONST  
                            { 
                                $$ = newExpNode(ConstantK); 
                                //$$->attr.value = $1->numValue;
                                $$->attr.name = $1->tokenstr;
                                $$->expType = Integer;
                                $$->lineno = $1->linenum;
                            }        
                        | CHARCONST   
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.cvalue = $1->charValue;
                                //$$->expType = CharInt;
                                $$->expType = Char;
                                $$->op = 1;     //char flag - bad idea I know please don't judge;
                                $$->lineno = $1->linenum;
                            }      
                        | STRINGCONST    
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.string = $1->stringValue;
                                //$$->attr.name = $1->stringValue;
                                $$->expType = Char;
                                $$->lineno = $1->linenum;
                            }   
                        | BOOLCONST         
                            { 
                                $$ = newExpNode(ConstantK); 
                                $$->attr.value = $1->numValue;
                                $$->attr.name = $1->tokenstr;
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
    int c, dflg = 0, pflg = 0, filerr = 0, Pflg = 0;
    char *oarg = NULL;
    FILE *filename;

    while ((c = ourGetopt(argc, argv, (char *)":dpP")) != EOF)
    {
        switch(c)
        {
            case 'd':
                ++dflg;
                break;
            
            case 'p':
                ++pflg;
                break;

            case 'P':
                ++Pflg;
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
        yydebug = 1;
    }

    if (optind < argc) 
    {
        oarg = argv[optind];
        filerr++;
        optind++;
    }

    //if(filerr == 1||filerr == 0)
    if(filerr == 1)
    {
        filename = fopen(oarg, "r");
        //filename = fopen("tests/undefined.c-", "r");

        if(filename == NULL)
        {
            printf("ERROR(ARGLIST): file \"%s\" could not be opened.\n", oarg);
            exit(1);
        }
        else
        {
            yyin = filename;
        }
    }
    else
    {
        yyin = stdin;
    }

    numErrors = 0;
    numWarnings = 0;

    yyparse();

    if(numErrors == 0)
    {
        if(pflg) 
        {
            printTree(savedTree, 1, 0);
        }

        semantic(savedTree);

        if(Pflg) 
        {
            semanticPrintTree(savedTree, 1, 0);
        }

    }
    
    printf("Number of warnings: %d\n", numWarnings);
    printf("Number of errors: %d\n", numErrors);

    return 0;
}