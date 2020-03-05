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

    if(st.depth() > 1)
    {
        //do check for usage and other warnings here
        st.leave();
    }

    st.print(pointerPrintStr);

    if(st.lookupGlobal("main") == NULL)
    {
        printf("ERROR(LINKER): Procedure main is not declared.\n");
    }
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
                if(st.lookup(t->attr.name) == NULL)
                { st.insert(t->attr.name, t); }
                else
                {
                    TreeNode *temp = st.lookupNode(t->attr.name);
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, temp->lineno);
                }
                break;
            case FuncK:
                //Leave the scope of the last function
                if(st.depth() > 1)
                { st.leave(); }

                if(st.lookup(t->attr.name) == NULL)
                { st.insert(t->attr.name, t); }
                else
                {
                    TreeNode *temp = st.lookupNode(t->attr.name);
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, temp->lineno);
                }
                st.enter(t->attr.name);
                break;
            case ParamK:
                if(st.lookup(t->attr.name) == NULL)
                { st.insert(t->attr.name, t); }
                else
                {
                    TreeNode *temp = st.lookupNode(t->attr.name);
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, temp->lineno);
                }
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
                //st.lookup(t->attr.name);
                if(st.lookup(t->attr.name) == NULL)
                { printf("ERROR(%d): Symbol '%s' is not declared.\n", t->lineno, t->attr.name); }
                else
                { /* TO DO code here */ }
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