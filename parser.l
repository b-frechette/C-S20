%{
    /*
    *   parser.l
    */

    int linenum = 1;
%}

%option noyywrap

ID          [a-z|A-Z|_][a-z|A-Z|_|0-9]*
NUMCONST    [0-9]+
CHARCONST   \'[\\]*[^\n\0]\'
STRINGCONST \"[^\n\0]*\"

%%

and |
bool |
break |
char |
do |
else |
elsif |
forever |
if | 
int |
loop | 
not |
or |
return |
static |
then |
while           {printf("line:%d type:KEYWORD value:%s\n",linenum, yytext);} //Keywords -> line, type, value

\; |
\, |
\[ |
\] |
\: |
\( |
\) |
\{ |
\} |
\= |
\> |
\< |
\+ |
\- |
\* |
\/ |
\% |
\? |
\.\. |
\+\= |
\-\= |
\*\= |
\/\= |
\+\+ |
\-\- |
\<\= |
\>\= |
\=\= |
\!\=            {printf("line:%d type:OPERATOR value:%s\n",linenum, yytext);} //OPERATOR -> line, type, value

{ID}            {printf("line:%d type:ID value:%s\n",linenum, yytext);} //ID -> line, type, value     

{NUMCONST}      {printf("line:%d type:NUMCONST value:%s\n",linenum, yytext);} //NUMCONST -> line, type, value

{CHARCONST}     {printf("line:%d type:CHARCONST value:%s\n",linenum, yytext);} //CHARCONST -> line, type, value

{STRINGCONST}   {printf("line:%d type:STRINGCONST value:%s\n",linenum, yytext);} //STRINGCONST -> line, type, value

\n              { linenum++; } //New Lines -> Discard and incremente line counter

"//".*\n        { linenum++; } //Comments -> grab newline for counter

[ \t]           ; //Whitespace -> Discard

.               {printf("line:%d ERROR \n",linenum);} //Error -> Return an error message

%%

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        yyin = fopen(argv[1], "r");
    }
    else
    {
        yyin=stdin;
    }

    yylex();
}