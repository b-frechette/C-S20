%{
    /*
    *   parser.l
    */

    #include <stdlib.h>
    #include "scanType.h"
    #include "parser.tab.h"

    static int line = 1;

    static int setValue(int lineNum, int tokClass, char *value)
    {
        // create and handle default data storage
        yylval.tokenData = new TokenData;
        yylval.tokenData->tokenclass = tokClass;
        yylval.tokenData->linenum = lineNum;
        yylval.tokenData->tokenstr = strdup(value);

        // handle special conversions
        switch (tokClass) {
        case NUMCONST:
            yylval.tokenData->numValue = atof(value); 
            break;
        case BOOLCONST:
            if(strcmp(value, "true") == 0)
            { yylval.tokenData->numValue = 1; }
            else
            { yylval.tokenData->numValue = 0; }
            break;
    }

    return tokClass;
    }
%}

%option noyywrap

ID          [a-z|A-Z|_][a-z|A-Z|_|0-9]*
NUMCONST    [0-9]+
CHARCONST   \'[\\]*[^\n\0]\' 
STRINGCONST \"[^\n\0]*\"   

%%

"static"|"int"|"bool"|"char"|"elsif"|"then"|"if"|"else"|"while"|"do"|"loop forever"|"loop"|"return"|"break"|"or"|"and"|"not"         { return setValue(line, KEYWORD, yytext); }

"true"|"false"  { return setValue(line, BOOLCONST, yytext); }

{ID}            { return setValue(line, ID, yytext); }

{NUMCONST}      { return setValue(line, NUMCONST, yytext); }

{CHARCONST}     { return setValue(line, CHARCONST, yytext); } //Need to fix - parse out char 

{STRINGCONST}   { return setValue(line, STRINGCONST, yytext); } //Need to fix - maybe parse out the string?

[ \t]           ; //Whitespace -> Discard

"//".*\n        { line++; } //Comments -> grab newline for counter

\n              { line++ ; return setValue(line, yytext[0], yytext); }

.               ;//{printf("line:%d ERROR \n",line);} //Error -> Return an error message

%%

// int main(int argc, char **argv)
// {
//     if(argc > 1)
//     {
//         yyin = fopen(argv[1], "r");
//     }
//     else
//     {
//         yyin=stdin;
//     }

//     yylex();
// }