#ifndef _TREENODE_H_
#define _TREENODE_H_


// Kinds of Operators
// these are the token numbers for the operators same as in flex
typedef int OpKind;  

// Kinds of Statements
typedef enum {DeclK, StmtK, ExpK} NodeKind;

// Subkinds of Declarations
typedef enum {VarK, FuncK, ParamK} DeclKind;

// Subkinds of Statements
typedef enum {NullK, ElsifK, IfK, WhileK, LoopK, LoopForeverK, CompoundK, RangeK, ReturnK, BreakK} StmtKind;

// Subkinds of Expressions
typedef enum {OpK, ConstantK, IdK, AssignK, InitK, CallK} ExpKind;

// ExpType is used for type checking (Void means no type or value, UndefinedType means undefined)
typedef enum {Void, Integer, Boolean, Char, CharInt, Equal, UndefinedType} ExpType;

// What kind of scoping is used?  (decided during typing)
typedef enum {None, Local, Global, Parameter, LocalStatic} VarKind;

#define MAXCHILDREN 3 

typedef struct treeNode
{
    // connectivity in the tree
    struct treeNode *child[MAXCHILDREN];   // children of the node
    struct treeNode *sibling;              // siblings for the node

    // what kind of node
    int lineno;                            // linenum relevant to this node
    NodeKind nodekind;                     // type of node
    union                                  // subtype of type
    {
	    DeclKind decl;                     // used when DeclK
	    StmtKind stmt;                     // used when StmtK
	    ExpKind exp;                       // used when ExpK
    } kind;
    
    // extra properties about the node depending on type of the node
    union                                  // relevant data to type -> attr
    {
        OpKind op;                         // type of token (same as in bison)
	    int value;                         // used when an integer constant or boolean
        unsigned char cvalue;               // used when a character
	    char *string;                      // used when a string constant
	    const char *name;                        // used when IdK
    } attr;                                 
    ExpType expType;		           // used when ExpK for type checking
    bool isArray;                          // is this an array
    bool isStatic;                         // is staticly allocated?

    // even more semantic stuff will go here in later assignments.
} TreeNode;

#endif