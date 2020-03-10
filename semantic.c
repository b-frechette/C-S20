#include <stdio.h>
#include <stdlib.h>
#include "treeNodes.h"
#include "symbolTable.h"
#include "semantic.h"

SymbolTable st;

void semantic(TreeNode *syntaxTree)
{
    insertNode(syntaxTree);

    //st.print(pointerPrintStr);

    if(st.lookupGlobal("main") == NULL)
    {
        printf("ERROR(LINKER): Procedure main is not declared.\n");
        numErrors++;
    }
}

void insertNode(TreeNode *t)
{
    int i;
    ExpType c1, c2;
    bool scoped = false;
    TreeNode *temp;

    if(t->nodekind == DeclK)
    {
        switch(t->kind.decl)
        {
            case VarK:
                if(!st.insert(t->attr.name, t))         //Already declared
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
                if(!st.insert(t->attr.name, t))         //Already declared
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, st.lookupNode(t->attr.name)->lineno);
                    numErrors++;
                }

                if(t->child[1] != NULL)                     //Apparently Null checking makes this work if it is not a compound????
                {
                    if(t->child[1]->kind.stmt == CompoundK) //Set the enteredScope bool to true for the following compound statement
                    {
                        t->child[1]->enteredScope = true;
                    }
                }
                

                st.enter(t->attr.name);                 //Enter a new scope
                scoped = true;                          //Entered scope bool set

                break;

            case ParamK:
                if(!st.insert(t->attr.name, t))         //Already declared
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
                c1 = typeCheck(t->child[0]);
                c2 = typeCheck(t->child[1]);
                // printf("OP %s child 1 type is %d\n", t->attr.name, c1);
                // printf("OP %s child 2 type is %d\n", t->attr.name, c2);

                //CHECKING FOR ASSIGNMENT GOES HERE (?)

                t->expType = c1;                            //Assign it to the first child -- errors not handled here
                //return t->expType;                        Causes major errors at the moment?
                break;
                break;
            case ConstantK:
                //return t->expType;
                break;
            case IdK:
                temp = st.lookupNode(t->attr.name);         //Assign return of lookupNode to temporary TreeNode
                if(temp == NULL)                            //Not declared
                {
                    t->expType = UndefinedType;             //Set to undefined type
                    printf("ERROR(%d): Symbol '%s' is not declared.\n", t->lineno, t->attr.name);
                    numErrors++;
                    //return t->expType;
                }
                else                                        //Is declared
                {
                    if(temp->kind.decl == FuncK)            //Error in calling a function as a variable
                    {
                        printf("ERROR(%d): Cannot use function '%s' as a variable.\n", t->lineno, t->attr.name);
                        numErrors++;
                        //return UndefinedType; 
                    }
                    else                                    //Assign the ID with a type   
                    {
                        t->expType = temp->expType;

                        if(temp->isInit == false)
                        {
                            printf("WARNING(%d): Variable %s may be uninitialized when used here.\n", t->lineno, t->attr.name);
                            numWarnings++;
                        }
                    }
                    
                }
                break;
            case AssignK:   //check the children recurisively first
                c1 = typeCheck(t->child[0]);
                c2 = typeCheck(t->child[1]);

                temp = st.lookupNode(t->child[0]->attr.name);
                if(temp != NULL)
                {
                    temp->isInit = true;
                    temp->isUsed = true;
                }

                //CHECKING FOR ASSIGNMENT GOES HERE (?)

                t->expType = c1;                            //Assign it to the first child -- errors not handled here
                break;
            case CallK:
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
                    else
                    {
                        t->expType = temp->expType;
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
                break;
            case IfK:
                break;
            case WhileK:
                break;
            case LoopK:
                // Create a new scope
                if(t->child[2] != NULL)                     //Apparently Null checking makes this work if it is not a compound????
                {
                    if(t->child[2]->kind.stmt == CompoundK) //Set the enteredScope bool to true for the following compound statement
                    {
                        t->child[2]->enteredScope = true;
                    }
                }
                st.enter("Loop");
                scoped = true;
                break;
            case LoopForeverK:
                break;
            case CompoundK:
                if(!t->enteredScope)                    //Check that it is not a function scope before
                {
                    st.enter("Compound Scope"); 
                    scoped = true;
                }
                break;

            case RangeK:
                break;
            case ReturnK:
                break;
            case BreakK:
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

    if(scoped)                          //Leaves the scope as recursive function backtracks
    {
        st.applyToAll(checkUse);
        st.leave();
    }

    if(t->sibling != NULL)
    {
        insertNode(t->sibling); 
    }
}

void checkUse(std::string sym, void* t)
{
    //Check for initialization and use before leaving scope
    //TO DO:
    // -> don't worry about many warnings - it will clear up once I get operators in
    // -> use still needs to be implemented
    TreeNode *temp;
    temp = st.lookupNode(sym.c_str());

    if(temp != NULL)
    {
        if(temp->isUsed == false)
        {
            printf("WARNING(%d): The variable %s seems not to be used.\n", temp->lineno, temp->attr.name);
            numWarnings++;
        }
    }
}

ExpType typeCheck(TreeNode *t)
{
    int i;
    ExpType c1, c2;
    TreeNode *temp;
    
    if(t == NULL)
    {
        return Error;
    }

    if(t->nodekind == ExpK)
    {
        switch(t->kind.exp)
        {
            case OpK:
                c1 = typeCheck(t->child[0]);
                c2 = typeCheck(t->child[1]);

                //CHECKING FOR ASSIGNMENT GOES HERE (?)

                t->expType = c1;                            //Assign it to the first child -- errors not handled here
                return t->expType;                          //Causes major errors at the moment?
                break;
            case ConstantK:
                return t->expType;
                break;
            case IdK:
                temp = st.lookupNode(t->attr.name);         //Assign return of lookupNode to temporary TreeNode
                if(temp == NULL)                            //Not declared
                {
                    t->expType = UndefinedType;             //Set to undefined type
                    return t->expType;
                }
                else                                        //Is declared
                {
                    if(temp->kind.decl == FuncK)            //Error in calling a function as a variable
                    {
                        return UndefinedType; 
                    }
                    else                                    //Assign the ID with a type   
                    {
                        t->expType = temp->expType;
                        return t->expType;
                    }
                }
                break;
            case AssignK:   //check the children recurisively first
                c1 = typeCheck(t->child[0]);
                c2 = typeCheck(t->child[1]);

                //CHECKING FOR ASSIGNMENT GOES HERE (?)

                t->expType = c1;                            //Assign it to the first child -- errors not handled here
                return t->expType;                          //Causes major errors at the moment?
                break;
            case CallK:
                temp = st.lookupNode(t->attr.name);         //Assign return of lookupNode to temporary TreeNode

                if(temp == NULL)                            //Not declared
                {
                    return UndefinedType;
                }
                else
                {
                    if(temp->kind.decl != FuncK)            //Error in calling a function as a variable
                    {
                        return UndefinedType;
                    }
                    else
                    {
                        t->expType = temp->expType;
                        return t->expType;
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
                 break;
         }
    }
    else
    {
        printf("ERROR: Unkown Node\n");
    }

    for(i = 0; i < MAXCHILDREN; i++)
    {
        if(t->child[i] != NULL)
        {
            insertNode(t->child[i]);
        }
    }

    if(t->sibling != NULL)
    {
        insertNode(t->sibling); 
    }
}