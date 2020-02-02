%{
    /* 
    *  parser.l
    *
    *  Flex file used to match with the stream of characters
    *  in a given file or stdin.
    */

    #include <stdlib.h>
    #include <stdio.h> 
    #include <ctype.h> 
    #include <string.h>
    #include "scanType.h"
    #include "parser.tab.h"

    static int line = 1;

    /* descr: Convert the given char * to uppercase. Used
    *  for KEYWORDs for the tokenname.
    *
    * param: char *s -> yytext
    * return: void -> alters the char *s as a return
    */
    void toUpper(char *s)
    {
        int i = 0;
        while(s[i]) 
        {
            s[i] =  (toupper(s[i]));
            i++;
        }
    }

    /* descr: Parse the STRINGCONST value. Removing the
    *  quotes and converting the meta-characters.
    *
    * param: char *s -> yytext
    * return: void -> alters the char *s as a return
    */
    void parseString(char *s)
    {
        int i = 0, curr = 0, len = strlen(s);
        char *temp = strdup(s);
        memset(s, '\0', len);

        while(temp[i])
        {
            if(temp[i] == '\"')
            { i++; } //Do nothing
            else if(temp[i] == '\\' && temp[i+1] == 'n')
            {
                s[curr] = '\n';
                curr++;
                i += 2;
            }
            else if(temp[i] == '\\' && temp[i+1] == '0')
            {
                s[curr] = '\0';
                curr++;
                i += 2;
            }
            else if(temp[i] == '\\')
            {
                s[curr] = temp[i+1];
                curr++;
                i += 2;
            }
            else
            {
                s[curr] = temp[i];
                curr++;
                i++;
            }
        }
    }

    /* descr: Parse an invalid CHARCONST value. Removing the
    *  quotes, counting the amount of characters, and checking
    *  for/handling meta-charcters.
    *
    * param: char *s -> yytext
    * return: int -> returns the number of characters in the CHARCONST
    */
    int parseChar(char *s)
    {
        int i = 0, curr = 0, errlen = 0, len = strlen(s);
        char *temp = strdup(s);
        memset(s, '\0', len);

        while(temp[i])
        {
            if(temp[i] == '\'')
            { i++; } //Do nothing
            else if(temp[i] == '\\' && temp[i+1] == 'n')
            {
                s[curr] = '\n';
                curr++;
                i += 2;
                errlen += 2;
            }
            else if(temp[i] == '\\' && temp[i+1] == '0')
            {
                s[curr] = '\0';
                curr++;
                i += 2;
                errlen += 2;
            }
            else if(temp[i] == '\\')
            {
                s[curr] = temp[i+1];
                curr++;
                i += 2;
                errlen += 2;
            }
            else
            {
                s[curr] = temp[i];
                curr++;
                i++;
                errlen++;
            }
        }

        return errlen;
    }

    /* descr: Parse the STRINGCONST value. Removing the
    *  quotes and converting the meta-characters.
    *
    * param: int lineNum -> line number of the token
    * param: int tokClass -> given bison number of the token value
    * param: char *value -> yytext the input of the given token
    * return: int - returns the token value for bison
    */
    static int setValue(int lineNum, int tokClass, char *value)
    {
        // Create and handle default data storage
        yylval.tokenData = new TokenData;
        yylval.tokenData->tokenclass = tokClass;
        yylval.tokenData->linenum = lineNum;
        yylval.tokenData->tokenstr = strdup(value); 

        // Handle special conversions
        switch (tokClass) {
        case ID:
            yylval.tokenData->tokenname = "ID";
            break;
        case NUMCONST:
            yylval.tokenData->numValue = atof(value); 
            yylval.tokenData->tokenname = "NUMCONST";
            break;
        case CHARCONST:
            // Catch meta-character charconst
            if(value[1] == '\\')
            { 
                //If \n or \0 then grab the char 
                if(value[2] == 'n')
                { yylval.tokenData->charValue = '\n'; }
                else if(value[2] == '0')
                { yylval.tokenData->charValue = '\0'; }
                else 
                { yylval.tokenData->charValue = value[2]; }
            }
            else //otherwise strip character outright
            { yylval.tokenData->charValue = value[1]; }
            yylval.tokenData->tokenname = "CHARCONST";
            break;
        case STRINGCONST:
            parseString(value);
            yylval.tokenData->stringValue = strdup(value);
            yylval.tokenData->tokenname = "STRINGCONST";
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
                else if(strcmp(value, "-=") == 0)
                { yylval.tokenData->tokenname = "SUBASS"; }
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
        case INVALIDCHAR:
            printf("ERROR(%d): character is %d characters and not a single character: \'%s\'. F\n", lineNum, parseChar(value), yylval.tokenData->tokenstr);
            yylval.tokenData->charValue = value[0];
            yylval.tokenData->tokenname = "CHARCONST";
            break;
    }

    return tokClass;
    }
%}

%option noyywrap

ID          [a-z|A-Z|_][a-z|A-Z|_|0-9]*
NUMCONST    [0-9]+
CHARCONST   \'(\\.|[^'\n])\'
INVALIDCHAR \'[^'\n]*\' 
STRINGCONST \"(\\.|[^"\n])*\"  

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
\!\=            { return setValue(line, OPERATOR, yytext); } //OPERATOR -> Catches all operators (will probably need to change)

"static"|"int"|"bool"|"char"|"elsif"|"then"|"if"|"else"|"while"|"do"|"forever"|"loop"|"return"|"break"|"or"|"and"|"not"         { return setValue(line, KEYWORD, yytext); } //KEYWORD -> Catches all keywords

"true"|"false"  { return setValue(line, BOOLCONST, yytext); } //BOOLCONST -> Catches the keyword of true or false, but recognized as BOOLCONST

{ID}            { return setValue(line, ID, yytext); } //ID -> Catches all IDs

{NUMCONST}      { return setValue(line, NUMCONST, yytext); } //NUMCONST -> Catches all valid numbers

{CHARCONST}     { return setValue(line, CHARCONST, yytext); } //CHARCONST -> Catches all CHARCONST (Watch regular expression and parsing for charValue)

{INVALIDCHAR}   { return setValue(line, INVALIDCHAR, yytext); } //INVALIDCHAR -> Catches all INVALID CHAR 

{STRINGCONST}   { return setValue(line, STRINGCONST, yytext); } //STRINGCONST -> Catches string (Watch the regular expression and parsing for stringValue)

[ \t]           ; //WHITESPACE -> Discard

"//".*\n        { line++; } //COMMENTS -> grab newline for counter and discard the comment

\n              { line++ ; return setValue(line, yytext[0], yytext); } //NEWLINE -> grab character for grammar

.               { printf("ERROR(%d): Invalid or misplaced input character: \"%s\"\n", line, yytext); } //ERROR -> Throw line number are erroneous charcter

%%