#ifndef _TREENODE_H_
#define _TREENODE_H_

typedef int OpKind;  

typedef enum {DeclK, StmtK, ExpK} NodeKind;

typedef enum {VarK, FuncK, ParamK} DeclKind;

typedef enum {NullK, ElsifK, IfK, WhileK, LoopK, LoopForeverK, CompoundK, RangeK, ReturnK, BreakK} StmtKind;

typedef enum {OpK, ConstantK, IdK, AssignK, InitK, CallK} ExpKind;

typedef enum {Void, Integer, Boolean, Char, CharInt, Equal, UndefinedType, Error} ExpType;

typedef enum {None, Local, Global, Parameter, LocalStatic} VarKind;

#define MAXCHILDREN 3 

/* 
* descr: Struct to store data for the treeNode
*/
typedef struct treeNode
{
    struct treeNode *child[MAXCHILDREN];   
    struct treeNode *sibling;            

    int lineno;                          
    NodeKind nodekind; 
    ExpType expType;  
    VarKind var;
    OpKind op;                
    union                               
    {
	    DeclKind decl;                    
	    StmtKind stmt;                    
	    ExpKind exp;                      
    } kind;
    
    union                                 
    {                         
	    int value;                        
        unsigned char cvalue;           
	    char *string;                     
	    const char *name;                        
    } attr;     
		      
    bool isArray;           //Array flag                        
    bool isStatic;          //Static flag
    bool enteredScope;      //Scope flag for compound scope
    bool isInit;            //Initialization flag 
    bool isUsed;            //Use flag
    bool isChecked;         //Check flag for children
    bool isIndexed;         //Index flag for arrays
    bool isFlagged;         //Warning flag for checking if warning has already been flagged          

} TreeNode;

/* 
* descr: Struct to store data for var or param types
*/
typedef struct varData 
{
    int linenum;
    ExpType expType;
    bool isStatic; 
} VarData;

#endif