%{
#include <stdlib.h>
// must come before calc.tab.h because it refers to this datatype
#include "scanType.h"     // shared token value struct
#include "parser.tab.h"     // token defs

%}

%option noyywrap

ID          [a-z|A-Z|_][a-z|A-Z|_|0-9]*
NUMCONST    [0-9]+
CHARCONST   \'(\\.|[^'\n])\'
STRINGCONST \"(\\.|[^"\n])*\"

%%
\;                                  { return SEMI; } 
\,                                  { return COMMA; } 
\[                                  { return RBRACKET; }
\]                                  { return LBRACKET; }
\:                                  { return COLON; }
\(                                  { return LPAREN; }
\)                                  { return RPAREN; }
\{                                  { return LCURL; }
\}                                  { return RCURL; }
\=                                  { return ASSIGN; }
\>                                  { return GRT; }
\<                                  { return LESS; }
\+                                  { return PLUS; }
\-                                  { return MINUS; }
\*                                  { return MUL; }
\/                                  { return DIV; }
\%                                  { return TRUNC; }
\?                                  { return RAND; }
\.\.                                { return RANGE; }
\+\=                                { return ADDASS; }
\-\=                                { return SUBASS; }
\*\=                                { return MULASS; }
\/\=                                { return DIVASS; }
\+\+                                { return INC; }
\-\-                                { return DEC; }
\<\=                                { return LESSEQ; }
\>\=                                { return GRTEQ; }
\=\=                                { return EQ; }
\!\=                                { return NOTEQ; }

"static"                            { return STATIC; }
"int"                               { return INT; } 
"bool"                              { return BOOL; }
"char"                              { return CHAR; }
"elsif"                             { return ELSIF; }
"then"                              { return THEN; }
"if"                                { return IF; }
"else"                              { return ELSE; }
"while"                             { return WHILE; }
"do"                                { return DO; }
"forever"                           { return FOREVER; }
"loop"                              { return LOOP; }
"return"                            { return RETURN; }
"break"                             { return BREAK; }
"or"                                { return OR; }
"and"                               { return AND; }
"not"                               { return NOT; }

"true"                              { return TRUE; }
"false"                             { return FALSE; }

{ID}                                { return ID; }
{STRINGCONST}                       { return STRINGCONST; }
{CHARCONST}                         { return CHARCONST; }
{NUMCONST}                          { return NUMCONST; }

[ \t]                               ;  // white space
\n                                  ;
.                                   { printf("It's a no bro\n"); }
%%
