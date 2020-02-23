/* 
*  scanType.h
*
*  Header file to define the struct that stores token data
*/
#ifndef _SCANTYPE_H_
#define _SCANTYPE_H_

/* 
*  descr: Struct for holding the token data that is assigned
*  in the flex and unionized to a %token in bison
*/
struct TokenData 
{
    int tokenclass;             // Token class
    int linenum;                // Line number where token found
    const char *tokenname;      // Token name for output (i.e. ID, STRINGCONST, etc.)
    char *tokenstr;             // Stores yytext value
    int numValue;               // Stores value of a NUMCONST or BOOLCONST
    char *stringValue;          // Stores value of a STRINGCONST
    char charValue;             // Stores value of a CHARCONST
};

#endif