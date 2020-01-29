#ifndef _SCANTYPE_H_
#define _SCANTYPE_H_
// 
//  SCANNER TOKENDATA
// 
struct TokenData {
    int tokenclass;     // token class
    int linenum;        // line number where token found
    char *tokenname;
    char *tokenstr;     // literal string for token for use
    int numValue;       // value of number if number
    char *stringValue;
    char charValue;     //NEED TO FIGURE OUT
};
#endif