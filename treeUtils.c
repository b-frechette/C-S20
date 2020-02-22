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

static int indentno = 0;
bool first = true;

#define INDENT indentno += 1
#define UNINDENT indentno -= 1

static void printSpaces(void)
{
    int i;
    for(i = 0; i < indentno; i++)
    {
        printf("!   ");
    }
}

void printTree(TreeNode *tree)
{
    int i, sib;
    INDENT;
    
    while(tree != NULL)
    {
        //printSpaces();

        if(tree->nodekind == DeclK)
        {
            switch(tree->kind.decl)
            {
                case VarK:
                    //NEEDS ARRAY CHECK
                    printf("Var %s of type TO DO[line: %d]\n", tree->attr.name, tree->lineno);
                    break;
                case FuncK:
                    printf("Func %s returns type TO DO[line: %d]\n", tree->attr.name, tree->lineno);
                    break;
                //NEEDS ARRAY CHECK
                case ParamK:
                    printf("Param %s of type TO DO line: %d]\n", tree->attr.name, tree->lineno);
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
                    printf("Constant: %s [line: %d]\n", tree->attr.name, tree->lineno);
                    break;
                case IdK:
                    printf("Id: %s [line: %d]\n", tree->attr.name, tree->lineno);
                    break;
                case AssignK:
                    printf("Assign: %s [line: %d]\n", tree->attr.name, tree->lineno);
                    break;
                case InitK:
                    printf("I don't have any nodes for this\n");
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
                case NullK:
                    printf("Null unsure if I am going to use\n");
                    break;
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
                printSpaces();
                printf("Child[%d]  ", i);
            }
            printTree(tree->child[i]);
        }
        tree = tree->sibling;
    }
    UNINDENT;
}