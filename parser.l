%{
    /*
    *   parser.l
    */

    #include <stdlib.h>
    #include <stdio.h> 
    #include <ctype.h> 
    #include "scanType.h"
    #include "parser.tab.h"

    static int line = 1;

    void toUpper(char *s)
    {
        int i = 0;
        while(s[i]) 
        {
            s[i] =  (toupper(s[i]));
            i++;
        }
    }

    static int setValue(int lineNum, int tokClass, char *value)
    {
        // create and handle default data storage
        yylval.tokenData = new TokenData;
        yylval.tokenData->tokenclass = tokClass;
        yylval.tokenData->linenum = lineNum;
        yylval.tokenData->tokenstr = strdup(value); 

        // handle special conversions
        switch (tokClass) {
        case ID:
            yylval.tokenData->tokenname = "ID";
            break;
        case NUMCONST:
            yylval.tokenData->numValue = atof(value); 
            yylval.tokenData->tokenname = "NUMCONST";
            break;
        case CHARCONST:
            // if a char literal 
            if(value[1] == '\\')
            { 
                //If \n or \0 then grab the char 
                if(value[2] == 'n')
                { yylval.tokenData->charValue = '\n'; }
                else if(value[2] == '0')
                { yylval.tokenData->charValue = '\0'; }
                else // otherwise switch into those escape values
                { yylval.tokenData->charValue = value[2]; }
            }
            else //otherwise strip character outright
            { yylval.tokenData->charValue = value[1]; }
            yylval.tokenData->tokenname = "CHARCONST";
            break;
        case STRINGCONST:
            //strncpy(yylval.tokenData->stringValue, value + 1, strlen(value) - 2);
            break;
        case BOOLCONST:
            if(strcmp(value, "true") == 0)
            { yylval.tokenData->numValue = 1; }
            else
            { yylval.tokenData->numValue = 0; }
            yylval.tokenData->tokenname = "BOOLCONST";
            break;
        case KEYWORD:
            toUpper(value);
            yylval.tokenData->tokenname = strdup(value);
            break;
        case OPERATOR:
            if(value[1] != '\0')
            {
                if(strcmp(value, "..") == 0)
                { yylval.tokenData->tokenname = "RANGE"; }
                else if(strcmp(value, "+=") == 0)
                { yylval.tokenData->tokenname = "ADDASS"; }
                else if(strcmp(value, "*=") == 0)
                { yylval.tokenData->tokenname = "MULASS"; }
                else if(strcmp(value, "/=") == 0)
                { yylval.tokenData->tokenname = "DIVASS"; }
                else if(strcmp(value, "++") == 0)
                { yylval.tokenData->tokenname = "INC"; }
                else if(strcmp(value, "--") == 0)
                { yylval.tokenData->tokenname = "DEC"; }
                else if(strcmp(value, "<=") == 0)
                { yylval.tokenData->tokenname = "LESSEQ"; }
                else if(strcmp(value, ">=") == 0)
                { yylval.tokenData->tokenname = "GRTEQ"; }
                else if(strcmp(value, "==") == 0)
                { yylval.tokenData->tokenname = "EQ"; }
                else if (strcmp(value, "!=") == 0)
                { yylval.tokenData->tokenname = "NOTEQ"; }
            }
            else
            { yylval.tokenData->tokenname = strdup(value); }
            break;
    }

    return tokClass;
    }
%}

%option noyywrap

ID          [a-z|A-Z|_][a-z|A-Z|_|0-9]*
NUMCONST    [0-9]+
CHARCONST   \'[\\]?[^\n]\' 
STRINGCONST \"[^\n\0]*\"   

%%

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
\.\. | //RANGE
\+\= | //ADDASS
\-\= | //SUBASS
\*\= | //MULASS
\/\= | //DIVASS
\+\+ | //INC
\-\- | //DEC
\<\= | //LESSEQ
\>\= | //GRTEQ
\=\= | //EQ
\!\=            { return setValue(line, OPERATOR, yytext); } //TESTING PHASE - Confirm solution or possibilities with Heckendorn

"static"|"int"|"bool"|"char"|"elsif"|"then"|"if"|"else"|"while"|"do"|"forever"|"loop"|"return"|"break"|"or"|"and"|"not"         { return setValue(line, KEYWORD, yytext); } //TESTING PHASE - NOT PRETTY

"true"|"false"  { return setValue(line, BOOLCONST, yytext); } //TESTING PHASE

{ID}            { return setValue(line, ID, yytext); } //TESTING PHASE

{NUMCONST}      { return setValue(line, NUMCONST, yytext); } //TESTING PHASE

{CHARCONST}     { return setValue(line, CHARCONST, yytext); } //TESTING PHASE

{STRINGCONST}   { return setValue(line, STRINGCONST, yytext); } //Need to fix - maybe parse out the string?

[ \t]           ; //WHITESPACE -> Discard

"//".*\n        { line++; } //COMMENTS -> grab newline for counter and discard the comment

\n              { line++ ; return setValue(line, yytext[0], yytext); } //NEWLINE -> grab character for grammar

.               ;{printf("line:%d ERROR \n",line);} //Error -> Return an error message

%%