#include <stdlib.h>
#include <stdio.h> 
#include "treeUtils.h"

TreeNode * newExpNode(ExpKind kind)
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    
    if(t == NULL)
    {
        { printf("Out of memory error at line\n"); }
    }
    else
    {
        for(i = 0; i < MAXCHILDREN; i++)
        { 
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
    }

    return t;
}

TreeNode * newStmtNode(StmtKind kind)
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    
    if(t == NULL)
    {
        { printf("Out of memory error at line\n"); }
    }
    else
    {
        for(i = 0; i < MAXCHILDREN; i++)
        { 
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
    }

    return t;
}

TreeNode * newDeclNode(DeclKind kind)
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    
    if(t == NULL)
    {
        { printf("Out of memory error at line\n"); }
    }
    else
    {
        for(i = 0; i < MAXCHILDREN; i++)
        { 
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->nodekind = DeclK;
        t->kind.decl = kind;
    }

    return t;
}

static int indentno = -1;
bool first = true;

#define INDENT indentno += 1
#define UNINDENT indentno -= 1

static void printSpaces(int spaces)
{
    int i;
    for(i = 0; i < spaces; i++)
    {
        printf("!   ");
    }
}

void printTree(TreeNode *tree, int indent, int sibling)
{
    int i;
    //INDENT;
    
    //printSpaces();

    if(tree->nodekind == DeclK)
    {
        switch(tree->kind.decl)
        {
            case VarK:
                switch(tree->expType)
                {
                    case Integer:
                        if(tree->isArray)
                        {
                            printf("Var %s is array of type int [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        else
                        {
                            printf("Var %s of type int [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        break;
                    case Boolean:
                        if(tree->isArray)
                        {
                            printf("Var %s is array of type bool [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        else
                        {
                            printf("Var %s of type bool [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        break;
                    case Char:
                        if(tree->isArray)
                        {
                            printf("Var %s is array of type char [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        else
                        {
                            printf("Var %s of type char [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        break;
                    case UndefinedType:
                        printf("ERROR VAR: Undefined Type [line: %d]\n",tree->lineno);
                        break;
                    default:
                        printf("ERROR VAR: Default [line: %d]\n",tree->lineno);
                        break;
                }
                break;
            case FuncK:
                switch(tree->expType)
                {
                    case Void:
                        printf("Func %s returns type void [line: %d]\n", tree->attr.name, tree->lineno);
                        break;
                    case Integer:
                        printf("Func %s returns type int [line: %d]\n", tree->attr.name, tree->lineno);
                        break;
                    case Boolean:
                        printf("Func %s returns type bool [line: %d]\n", tree->attr.name, tree->lineno);
                        break;
                    case Char:
                        printf("Func %s returns type char [line: %d]\n", tree->attr.name, tree->lineno);
                        break;
                    case UndefinedType:
                        printf("ERROR FUNC: Undefined Type [line: %d]\n",tree->lineno);
                        break;
                    default:
                        printf("ERROR FUNC: Undefined Type [line: %d]\n",tree->lineno);
                        break;
                }
                //printf("Func %s returns type TO DO[line: %d]\n", tree->attr.name, tree->lineno);
                break;
            //NEEDS ARRAY CHECK
            case ParamK:
                switch(tree->expType)
                {
                    case Integer:
                        if(tree->isArray)
                        {
                            printf("Param %s is array of type int [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        else
                        {
                            printf("Param %s of type int [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        break;
                    case Boolean:
                        if(tree->isArray)
                        {
                            printf("Param %s is array of type bool [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        else
                        {
                            printf("Param %s of type bool [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        break;
                    case Char:
                        if(tree->isArray)
                        {
                            printf("Param %s is array of type char [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        else
                        {
                            printf("Param %s of type char [line: %d]\n", tree->attr.name, tree->lineno);
                        }
                        break;
                    case UndefinedType:
                        printf("ERROR PARAM: Undefined Type [line: %d]\n",tree->lineno);
                        break;
                    default:
                        printf("ERROR EXP: Default [line: %d]\n",tree->lineno);
                        break;
                }
                //printf("Param %s of type TO DO line: %d]\n", tree->attr.name, tree->lineno);
                break;
            default:
                printf("error\n");
                break;
        }
    }
    else if(tree->nodekind == ExpK)
    {
        switch(tree->kind.exp)
        {
            case OpK:
                printf("Op: %s [line: %d]\n", tree->attr.name, tree->lineno);
                break;
            case ConstantK:
                switch (tree->expType)
                {
                    case Char:
                        printf("Const: \"%s\" [line: %d]\n", tree->attr.name, tree->lineno);
                        break;
                    case CharInt:
                        printf("Const: '%c' [line: %d]\n", tree->attr.cvalue, tree->lineno);
                        break;
                    default:
                        printf("Const: %s [line: %d]\n", tree->attr.name, tree->lineno);
                        break;
                }
                break;
            case IdK:
                printf("Id: %s [line: %d]\n", tree->attr.name, tree->lineno);
                break;
            case AssignK:
                printf("Assign: %s [line: %d]\n", tree->attr.name, tree->lineno);
                break;
            case CallK:
                printf("Call: %s [line: %d]\n", tree->attr.name, tree->lineno);
                break;
            default:
                printf("error\n");
                break;
        }
    }
    else if(tree->nodekind == StmtK)
    {
        switch(tree->kind.stmt)
        {
            case ElsifK:
                printf("Elsif [line: %d]\n", tree->lineno);
                break;
            case IfK:
                printf("If [line: %d]\n", tree->lineno);
                break;
            case WhileK:
                printf("While [line: %d]\n", tree->lineno);
                break;
            case LoopK:
                printf("Loop [line: %d]\n", tree->lineno);
                break;
            case LoopForeverK:
                printf("LoopForever [line: %d]\n", tree->lineno);
                break;
            case CompoundK:
                printf("Compound [line: %d]\n", tree->lineno);
                break;
            case RangeK:
                printf("Range [line: %d]\n", tree->lineno);
                break;
            case ReturnK:
                printf("Return [line: %d]\n", tree->lineno);
                break;
            case BreakK:
                printf("Break [line: %d]\n", tree->lineno);
                break;
            default:
                printf("error\n");
                 break;
         }
    }
    else
    {
        printf("Unkown node\n");
    }

    for(i = 0; i < MAXCHILDREN; i++)
    {
        if(tree->child[i] != NULL)
        {
            printSpaces(indent);
            printf("Child: %d ", i);
            printTree(tree->child[i], indent+1, 0);
        }
    }
    
    //tree = tree->sibling;
    if(tree->sibling != NULL)
    {
        printSpaces(indent-1);
        printf("Sibling: %d ", sibling);
        printTree(tree->sibling, indent, sibling+1); 
    }
    //printTree(tree->sibling);
}