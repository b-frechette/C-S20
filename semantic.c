#include <stdio.h>
#include <stdlib.h>
#include "treeNodes.h"
#include "symbolTable.h"
#include "semantic.h"

SymbolTable st;

void semantic(TreeNode *syntaxTree)
{
    //printf("Checkpoint\n");
    traverse(syntaxTree, insertNode, nullProcedure);

    st.print(pointerPrintStr);
}

static void traverse(TreeNode *t, void (* preProcedure) (TreeNode *), void (* postProcedure) (TreeNode *))
{
    if(t != NULL)
    {
        preProcedure(t);
        {
           int i;
           for(i = 0 ; i < MAXCHILDREN; i++)
           {
               traverse(t->child[i], preProcedure, postProcedure);
           } 
        }

        postProcedure(t);
        if(st.depth() > 1)
        {
            st.leave();
        }
        traverse(t->sibling, preProcedure, postProcedure);
    }
}

static void nullProcedure(TreeNode *t)
{
    if(t == NULL)
        { return; }   
    else
        { return; }
}

static void insertNode(TreeNode *t)
{
    int i;

    if(t->nodekind == DeclK)
    {
        switch(t->kind.decl)
        {
            case VarK:
                st.insert(t->attr.name, t);
                break;
            case FuncK:
                st.insert(t->attr.name, t);
                st.enter(t->attr.name);
                break;
            case ParamK:
                break;
            default:
                printf("error\n");
                break;
        }
    }
    else if(t->nodekind == ExpK)
    {
        switch(t->kind.exp)
        {
            case OpK:
                break;
            case ConstantK:
                break;
            case IdK:
                break;
            case AssignK:
                break;
            case CallK:
                break;
            default:
                printf("error\n");
                break;
        }
    }
    else if(t->nodekind == StmtK)
    {
        switch(t->kind.stmt)
        {
            case ElsifK:
                break;
            case IfK:
                break;
            case WhileK:
                break;
            case LoopK:
                break;
            case LoopForeverK:
                break;
            case CompoundK:
                break;
            case RangeK:
                break;
            case ReturnK:
                break;
            case BreakK:

                break;
            default:
                printf("error\n");
                 break;
         }
    }
    else
    {
        printf("ERROR: Unkown Node\n");
    }
}