#include <stdlib.h>
#include <stdio.h> 
#include "treeUtils.h"

TreeNode * newExpNode(ExpKind kind)
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    
    if(t == NULL)
    { printf("Out of memory error at line\n"); }
    else
    {
        for(i = 0; i < MAXCHILDREN; i++)
        { t->child[i] = NULL; }
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
    }

    return t;
}