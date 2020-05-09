%{

    #include <stdlib.h>
    #include <stdio.h> 
    #include <ctype.h> 
    #include <string.h>
    #include "treeNodes.h"
    #include "scanType.h"
    #include "parser.tab.h"
    #include "yyerror.h"

    int line = 1;
    extern int numWarnings;

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
            sprintf(yylval.tokenData->tokenstr, "%d", yylval.tokenData->numValue);
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
        case INVALIDCHAR:
            printf("ERROR(%d): character is %d characters and not a single character: %s. First char used.\n", lineNum, parseChar(value), yylval.tokenData->tokenstr);
            yylval.tokenData->charValue = value[0];
            yylval.tokenData->tokenname = "CHARCONST";
            break;
    }

    return tokClass;
    }

// "true"                              { return setValue(line, TRUE, yytext); }
// "false"                             { return setValue(line, FALSE, yytext); }
%}

%option noyywrap

ID          [a-z|A-Z|_][a-z|A-Z|_|0-9]*
NUMCONST    [0-9]+
CHARCONST   \'(\\.|[^'\n])\'
INVALIDCHAR \'[^'\n]*\' 
STRINGCONST \"(\\.|[^"\n])*\"

%%
\;                                  { return setValue(line, *yytext, yytext); } 
\,                                  { return setValue(line, *yytext, yytext); } 
\[                                  { return setValue(line, *yytext, yytext); }
\]                                  { return setValue(line, *yytext, yytext); }
\:                                  { return setValue(line, *yytext, yytext); }
\(                                  { return setValue(line, *yytext, yytext); }
\)                                  { return setValue(line, *yytext, yytext); }
\{                                  { return setValue(line, *yytext, yytext); }
\}                                  { return setValue(line, *yytext, yytext); }
\=                                  { return setValue(line, *yytext, yytext); }
\>                                  { return setValue(line, *yytext, yytext); }
\<                                  { return setValue(line, *yytext, yytext); }
\+                                  { return setValue(line, *yytext, yytext); }
\-                                  { return setValue(line, *yytext, yytext); }
\*                                  { return setValue(line, *yytext, yytext); }
\/                                  { return setValue(line, *yytext, yytext); }
\%                                  { return setValue(line, *yytext, yytext); }
\?                                  { return setValue(line, *yytext, yytext); }
\.\.                                { return setValue(line, RANGE, yytext); }
\+\=                                { return setValue(line, ADDASS, yytext); }
\-\=                                { return setValue(line, SUBASS, yytext); }
\*\=                                { return setValue(line, MULASS, yytext); }
\/\=                                { return setValue(line, DIVASS, yytext); }
\+\+                                { return setValue(line, INC, yytext); }
\-\-                                { return setValue(line, DEC, yytext); }
\<\=                                { return setValue(line, LESSEQ, yytext); }
\>\=                                { return setValue(line, GRTEQ, yytext); }
\=\=                                { return setValue(line, EQ, yytext); }
\!\=                                { return setValue(line, NOTEQ, yytext); }

"static"                            { return setValue(line, STATIC, yytext); }
"int"                               { return setValue(line, INT, yytext); } 
"bool"                              { return setValue(line, BOOL, yytext); }
"char"                              { return setValue(line, CHAR, yytext); }
"elsif"                             { return setValue(line, ELSIF, yytext); }
"then"                              { return setValue(line, THEN, yytext); }
"if"                                { return setValue(line, IF, yytext); }
"else"                              { return setValue(line, ELSE, yytext); }
"while"                             { return setValue(line, WHILE, yytext); }
"do"                                { return setValue(line, DO, yytext); }
"forever"                           { return setValue(line, FOREVER, yytext); }
"loop"                              { return setValue(line, LOOP, yytext); }
"return"                            { return setValue(line, RETURN, yytext); }
"break"                             { return setValue(line, BREAK, yytext); }
"or"                                { return setValue(line, OR, yytext); }
"and"                               { return setValue(line, AND, yytext); }
"not"                               { return setValue(line, NOT, yytext); }

"true"|"false"                      { return setValue(line, BOOLCONST, yytext); }

{ID}                                { return setValue(line, ID, yytext); }
{STRINGCONST}                       { return setValue(line, STRINGCONST, yytext); }
{CHARCONST}                         { return setValue(line, CHARCONST, yytext); }
{INVALIDCHAR}                       { return setValue(line, INVALIDCHAR, yytext); }
{NUMCONST}                          { return setValue(line, NUMCONST, yytext); }

"//".*\n                            { line++; }
[ \t]                               ;  // white space
\n                                  { line++; }
.                                   { printf("WARNING(%d): Invalid or misplaced input character: \'%s\'. Character Ignored.\n", line, yytext); numWarnings++; }
%%
