#include <stdlib.h>
#include <stdio.h> 
#include "printtree.h"

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
                break;
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
                        printf("Const: \"%s\" [line: %d]\n", tree->attr.string, tree->lineno);
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
        printf("ERROR: Unkown Node\n");
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
    
    if(tree->sibling != NULL)
    {
        printSpaces(indent-1);
        printf("Sibling: %d ", sibling);
        printTree(tree->sibling, indent, sibling+1); 
    }
}

void semanticPrintTree(TreeNode *tree, int indent, int sibling)
{
    int i;

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
                break;
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
                        printf("Const: \"%s\" [type: %d] [line: %d]\n", tree->attr.string, tree->expType, tree->lineno);
                        break;
                    case CharInt:
                        printf("Const: '%c' [type: %d] [line: %d]\n", tree->attr.cvalue, tree->expType, tree->lineno);
                        break;
                    default:
                        printf("Const: %s [type: %d] [line: %d]\n", tree->attr.name, tree->expType, tree->lineno);
                        break;
                }
                break;
            case IdK:
                printf("Id: %s [type: %d] [line: %d]\n", tree->attr.name, tree->expType, tree->lineno);
                break;
            case AssignK:
                printf("Assign: %s [type: %d] [line: %d]\n", tree->attr.name, tree->expType, tree->lineno);
                break;
            case CallK:
                printf("Call: %s [type: %d] [line: %d]\n", tree->attr.name, tree->expType, tree->lineno);
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
        printf("ERROR: Unkown Node\n");
    }

    for(i = 0; i < MAXCHILDREN; i++)
    {
        if(tree->child[i] != NULL)
        {
            printSpaces(indent);
            printf("Child: %d ", i);
            semanticPrintTree(tree->child[i], indent+1, 0);
        }
    }
    
    if(tree->sibling != NULL)
    {
        printSpaces(indent-1);
        printf("Sibling: %d ", sibling);
        semanticPrintTree(tree->sibling, indent, sibling+1); 
    }
}