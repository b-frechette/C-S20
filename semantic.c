#include <stdio.h>
#include <stdlib.h>
#include "treeNodes.h"
#include "symbolTable.h"
#include "semantic.h"

SymbolTable st;

void semantic(TreeNode *syntaxTree)
{
    insertNode(syntaxTree);

    st.print(pointerPrintStr);

    if(st.lookupGlobal("main") == NULL)
    {
        printf("ERROR(LINKER): Procedure main is not declared.\n");
        numErrors++;
    }
}

ExpType TypeCheck(TreeNode *t)
{
    TreeNode *temp = t;

    if(t->kind.exp == IdK)
    {
        temp = st.lookupNode(t->attr.name); 

        if(temp == NULL)                            //Not declared
        {
            return UndefinedType;
        }
        else                                        //Is declared
        {
            if(temp->kind.decl == FuncK)            //Error in calling a function as a variable
            {
                return Error;
            }
            return temp->expType;
        }
    }
    else
    {
        return t->expType;
    }
}

char insertNode(TreeNode *t)
{
    int i;
    bool scoped = false;
    TreeNode *temp;

    if(t->nodekind == DeclK)
    {
        switch(t->kind.decl)
        {
            case VarK:
                //printf("VarK\n");
                if(st.lookup(t->attr.name) == NULL)     //Insert into table
                {
                    st.insert(t->attr.name, (TreeNode *)t);
                }
                else                                    //Already declared
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, st.lookupNode(t->attr.name)->lineno);
                    numErrors++;
                }

                if(t->child[0] != NULL)                 //If Initializing
                {
                    t->isInit = true;
                }
                break;

            case FuncK:
                //printf("FuncK\n");
                while(st.depth() > 1)                   //Leave out to Global Scope
                {
                    //Maybe do a check for leaving the scopes
                    st.leave();
                }                                    

                if(st.lookup(t->attr.name) == NULL)     //Insert into table
                {
                    st.insert(t->attr.name, (TreeNode *)t);
                }
                else                                    //Already declared
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, st.lookupNode(t->attr.name)->lineno);
                    numErrors++;
                }

                if(t->child[1]->kind.stmt == CompoundK) //Set the enteredScope bool to true for the following compound statement
                {
                    t->child[1]->enteredScope = true;
                }

                st.enter(t->attr.name);                 //Enter a new scope
                scoped = true;
                break;

            case ParamK:
                //printf("ParamK\n");
                if(st.lookup(t->attr.name) == NULL)     //Insert into table
                {
                    st.insert(t->attr.name, (TreeNode *)t);
                }
                else                                    //Already declared
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, st.lookupNode(t->attr.name)->lineno);
                    numErrors++;
                }
                break;

            default:
                break;
        }
    }
    else if(t->nodekind == ExpK)
    {
        switch(t->kind.exp)
        {
            case OpK:
                //printf("OpK\n");
                break;
            case ConstantK:
                //printf("ConstantK\n");
                break;
            case IdK:
                //printf("IdK\n");
                temp = st.lookupNode(t->attr.name);        //Assign return of lookupNode to temporary TreeNode

                if(temp == NULL)                            //Not declared
                {
                    printf("ERROR(%d): Symbol '%s' is not declared.\n", t->lineno, t->attr.name);
                    numErrors++;
                }
                else                                        //Is declared
                {
                    if(temp->kind.decl == FuncK)            //Error in calling a function as a variable
                    {
                        printf("ERROR(%d): Cannot use function '%s' as a variable.\n", t->lineno, t->attr.name);
                        numErrors++;
                    }
                    else                                    //Assign the ID with a type   
                    {
                        t->expType = temp->expType;
                    }
                    
                }
                break;
            case AssignK:
                break;
            case CallK:
                //printf("CallK\n");
                temp = st.lookupNode(t->attr.name);         //Assign return of lookupNode to temporary TreeNode

                if(temp == NULL)                            //Not declared
                {
                    printf("ERROR(%d): Symbol '%s' is not declared.\n", t->lineno, t->attr.name);
                    numErrors++;
                }
                else
                {
                    if(temp->kind.decl != FuncK)            //Error in calling a function as a variable
                    {
                        printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", t->lineno, t->attr.name);
                        numErrors++;
                    }
                }
                break;

            default:
                break;
        }
    }
    else if(t->nodekind == StmtK)
    {
        switch(t->kind.stmt)
        {
            case ElsifK:
                //printf("ElsifK\n");
                break;
            case IfK:
                //printf("IfK\n");
                break;
            case WhileK:
                //printf("WhileK\n");
                break;
            case LoopK:
                //printf("LoopK\n");
                break;
            case LoopForeverK:
                //printf("LoopForeverK\n");
                break;
            case CompoundK:
                //printf("CompoundK\n");
                if(!t->enteredScope)
                {
                    st.enter("Compound Scope"); 
                    scoped = true;
                }
                break;

            case RangeK:
                //printf("RangeK\n");
                break;
            case ReturnK:
                //printf("ReturnK\n");
                break;
            case BreakK:
                //printf("BreakK\n");
                break;
            default:
                 break;
         }
    }

    for(i = 0; i < MAXCHILDREN; i++)
    {
        if(t->child[i] != NULL)
        {
            insertNode(t->child[i]);
        }
    }

    if(scoped)
    {
        st.leave();
    }

    if(t->sibling != NULL)
    {
        insertNode(t->sibling); 
    }
}