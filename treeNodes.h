#ifndef _TREENODE_H_
#define _TREENODE_H_

typedef int OpKind;  

typedef enum {DeclK, StmtK, ExpK} NodeKind;

typedef enum {VarK, FuncK, ParamK} DeclKind;

typedef enum {NullK, ElsifK, IfK, WhileK, LoopK, LoopForeverK, CompoundK, RangeK, ReturnK, BreakK} StmtKind;

typedef enum {OpK, ConstantK, IdK, AssignK, InitK, CallK} ExpKind;

typedef enum {Void, Integer, Boolean, Char, CharInt, Equal, UndefinedType} ExpType;

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
    union                               
    {
	    DeclKind decl;                    
	    StmtKind stmt;                    
	    ExpKind exp;                      
    } kind;
    
    union                                 
    {
        OpKind op;                         
	    int value;                        
        unsigned char cvalue;           
	    char *string;                     
	    const char *name;                        
    } attr;                                 
    ExpType expType;		      
    bool isArray;                          
    bool isStatic;  
    bool isDeclared;                      

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