#include <stdio.h>
#include <stdlib.h>
#include "treeNodes.h"
#include "symbolTable.h"
#include "semantic.h"

void semantic(TreeNode *syntaxTree)
{
    //printf("Checkpoint\n");
    traverse(syntaxTree, insertNode, nullProcedure);
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
                switch(t->expType)
                {
                    case Integer:
                        if(t->isArray)
                        {
                            printf("Var %s is array of type int [line: %d]\n", t->attr.name, t->lineno);
                        }
                        else
                        {
                            printf("Var %s of type int [line: %d]\n", t->attr.name, t->lineno);
                        }
                        break;
                    case Boolean:
                        if(t->isArray)
                        {
                            printf("Var %s is array of type bool [line: %d]\n", t->attr.name, t->lineno);
                        }
                        else
                        {
                            printf("Var %s of type bool [line: %d]\n", t->attr.name, t->lineno);
                        }
                        break;
                    case Char:
                        if(t->isArray)
                        {
                            printf("Var %s is array of type char [line: %d]\n", t->attr.name, t->lineno);
                        }
                        else
                        {
                            printf("Var %s of type char [line: %d]\n", t->attr.name, t->lineno);
                        }
                        break;
                    case UndefinedType:
                        printf("ERROR VAR: Undefined Type [line: %d]\n",t->lineno);
                        break;
                    default:
                        printf("ERROR VAR: Default [line: %d]\n",t->lineno);
                        break;
                }
                break;
            case FuncK:
                switch(t->expType)
                {
                    case Void:
                        printf("Func %s returns type void [line: %d]\n", t->attr.name, t->lineno);
                        break;
                    case Integer:
                        printf("Func %s returns type int [line: %d]\n", t->attr.name, t->lineno);
                        break;
                    case Boolean:
                        printf("Func %s returns type bool [line: %d]\n", t->attr.name, t->lineno);
                        break;
                    case Char:
                        printf("Func %s returns type char [line: %d]\n", t->attr.name, t->lineno);
                        break;
                    case UndefinedType:
                        printf("ERROR FUNC: Undefined Type [line: %d]\n",t->lineno);
                        break;
                    default:
                        printf("ERROR FUNC: Undefined Type [line: %d]\n",t->lineno);
                        break;
                }
                break;
            case ParamK:
                switch(t->expType)
                {
                    case Integer:
                        if(t->isArray)
                        {
                            printf("Param %s is array of type int [line: %d]\n", t->attr.name, t->lineno);
                        }
                        else
                        {
                            printf("Param %s of type int [line: %d]\n", t->attr.name, t->lineno);
                        }
                        break;
                    case Boolean:
                        if(t->isArray)
                        {
                            printf("Param %s is array of type bool [line: %d]\n", t->attr.name, t->lineno);
                        }
                        else
                        {
                            printf("Param %s of type bool [line: %d]\n", t->attr.name, t->lineno);
                        }
                        break;
                    case Char:
                        if(t->isArray)
                        {
                            printf("Param %s is array of type char [line: %d]\n", t->attr.name, t->lineno);
                        }
                        else
                        {
                            printf("Param %s of type char [line: %d]\n", t->attr.name, t->lineno);
                        }
                        break;
                    case UndefinedType:
                        printf("ERROR PARAM: Undefined Type [line: %d]\n",t->lineno);
                        break;
                    default:
                        printf("ERROR EXP: Default [line: %d]\n",t->lineno);
                        break;
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
                printf("Op: %s [line: %d]\n", t->attr.name, t->lineno);
                break;
            case ConstantK:
                switch (t->expType)
                {
                    case Char:
                        printf("Const: \"%s\" [line: %d]\n", t->attr.string, t->lineno);
                        break;
                    case CharInt:
                        printf("Const: '%c' [line: %d]\n", t->attr.cvalue, t->lineno);
                        break;
                    default:
                        printf("Const: %s [line: %d]\n", t->attr.name, t->lineno);
                        break;
                }
                break;
            case IdK:
                printf("Id: %s [line: %d]\n", t->attr.name, t->lineno);
                break;
            case AssignK:
                printf("Assign: %s [line: %d]\n", t->attr.name, t->lineno);
                break;
            case CallK:
                printf("Call: %s [line: %d]\n", t->attr.name, t->lineno);
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
                printf("Elsif [line: %d]\n", t->lineno);
                break;
            case IfK:
                printf("If [line: %d]\n", t->lineno);
                break;
            case WhileK:
                printf("While [line: %d]\n", t->lineno);
                break;
            case LoopK:
                printf("Loop [line: %d]\n", t->lineno);
                break;
            case LoopForeverK:
                printf("LoopForever [line: %d]\n", t->lineno);
                break;
            case CompoundK:
                printf("Compound [line: %d]\n", t->lineno);
                break;
            case RangeK:
                printf("Range [line: %d]\n", t->lineno);
                break;
            case ReturnK:
                printf("Return [line: %d]\n", t->lineno);
                break;
            case BreakK:
                printf("Break [line: %d]\n", t->lineno);
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