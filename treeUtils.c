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

#define INDENT indentno += 2
#define UNINDENT indentno -= 2

static void printSpaces(void)
{
    int i;
    for(i = 0; i < indentno; i++)
    {
        printf(" ");
    }
}

void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    
    while(tree != NULL)
    {
        printSpaces();

        if(tree->nodekind == DeclK)
        {
            switch(tree->kind.decl)
            {
                case VarK:
                    printf("Var %s\n", tree->attr.name);
                    break;
                case FuncK:
                    printf("Func %s\n", tree->attr.name);
                    break;
                case ParamK:
                    printf("Param %s\n", tree->attr.name);
                    break;
                default:
                    break;
            }
        }
        for(i = 0; i < MAXCHILDREN; i++)
        {
            printTree(tree->child[i]);
        }
        tree = tree->sibling;
    }
    UNINDENT;
}