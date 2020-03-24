#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "treeNodes.h"
#include "symbolTable.h"
#include "semantic.h"

SymbolTable st;

void semantic(TreeNode *syntaxTree)
{
    insertNode(syntaxTree);

    if(st.lookupGlobal("main") == NULL)
    {
        printf("ERROR(LINKER): Procedure main is not declared.\n");
        numErrors++;
    }
}

ExpType insertNode(TreeNode *t)
{
    int i, errType;
    ExpType c1, c2, returns;
    bool scoped = false;    //Boolean to help recursively leave scopes
    bool arr1F = false, arr2F = false, id1 = false, id2 = false;
    bool arrayF = false;
    const char* types[] = {"type void", "type int", "type bool", "type char", "type char", "equal", "undefined type", "error"};
    TreeNode *temp, *temp2;

    //Check if Null -- return Error if true
    if(t == NULL)
    {return Error;}

    //VALID
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

                returns = t->expType;
                break;

            case FuncK:                                  
                if(!st.insert(t->attr.name, t))         //Already declared
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, st.lookupNode(t->attr.name)->lineno);
                    numErrors++;
                }

                if(t->child[1] != NULL && t->child[1]->kind.stmt == CompoundK) //Set the enteredScope bool to true for the following compound statement
                {
                    t->child[1]->enteredScope = true;
                }
                

                st.enter(t->attr.name);                 //Enter a new scope
                scoped = true;                          //Entered scope bool set
                returns = t->expType;
                break;

            case ParamK:
                if(!st.insert(t->attr.name, t))         //Already declared
                {
                    printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", t->lineno, t->attr.name, st.lookupNode(t->attr.name)->lineno);
                    numErrors++;
                }
                t->isInit = true; //Technically initialized since being passed in
                returns = t->expType;
                break;

            default:
                returns = Error;
                break;
        }
    }
    //INVALID
    else if(t->nodekind == ExpK)
    {
        switch(t->kind.exp)
        {
            case OpK:

                if(t->child[0] != NULL)
                {
                    t->child[0]->isChecked = true;  //CAUTION

                    temp = st.lookupNode(t->child[0]->attr.name);

                    if(temp != NULL)  //&& temp->kind.exp == IdK
                    {
                        if(temp->isArray == true)
                        { arr1F = true; }
                    }
                }

                if(t->child[1] != NULL)
                {
                    t->child[1]->isChecked = true;  //CAUTION

                    temp2 = st.lookupNode(t->child[1]->attr.name);

                    if(temp2 != NULL)  //&& temp2->kind.exp == IdK
                    {
                        if(temp2->isArray == true)
                        { arr2F = true; }
                    }
                }

                switch(t->op)
                {
                    case 1:     //OR
                    case 2:     //AND
                        //Check if array
                        if(arr1F || arr2F)
                        {
                            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                            numErrors++;  
                        }

                        c1 = insertNode(t->child[0]);
                        c2 = insertNode(t->child[1]);

                        if(c1 == UndefinedType)
                        { /*Do Nothing*/ }
                        else if(c1 != Boolean)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", t->lineno, t->attr.name, types[2], types[c1]);
                            numErrors++;
                        }

                        if(c2 == UndefinedType)
                        { /*Do Nothing*/ }
                        else if(c2 != Boolean)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", t->lineno, t->attr.name, types[2], types[c2]);
                            numErrors++;
                        }

                        t->expType = Boolean;
                        break;

                    case 3:     //NOT
                        //Check if array
                        if(arr1F == true)
                        {
                            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                            numErrors++;
                        }

                        c1 = insertNode(t->child[0]);

                        if(c1 == UndefinedType)
                        {/*Do nothing*/}
                        else if(c1 != Boolean)
                        {
                            printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[2], types[c1]);
                            numErrors++;
                        }
                        t->expType = Boolean;
                        break;

                    case 4:     //relop
                        c1 = insertNode(t->child[0]);
                        c2 = insertNode(t->child[1]);

                        if(strncmp(t->attr.name, "==", 2)== 0 || strncmp(t->attr.name, "!=", 2)== 0)
                        {
                            if(c1 == Void || c2 == Void)
                            {
                                if(c1 == Void)
                                {
                                    printf("ERROR(%d): '%s' requires operands of type bool, char, or int but lhs is of %s.\n", t->lineno, t->attr.name, types[c1]);
                                    numErrors++;
                                }

                                if(c2 == Void)
                                {
                                    printf("ERROR(%d): '%s' requires operands of type bool, char, or int but rhs is of %s.\n", t->lineno, t->attr.name, types[c2]);
                                    numErrors++;
                                }
                            }
                            else
                            {
                                if(c1 == UndefinedType || c2 == UndefinedType)
                                { /* Do nothing? */ }
                                else if(c1 != c2)                       //if they are not equal
                                { 
                                    printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", t->lineno, t->attr.name, types[c1], types[c2]);
                                    numErrors++;
                                } 
                            }
                            t->expType = Boolean;
                        }
                        else    // <=, <, >=, >
                        {
                            if(arr1F == true || arr2F == true)
                            {
                                printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                                numErrors++;
                            }
                            else
                            {
                                if(c1 == Void || c2 == Void || c1 == Boolean || c2 == Boolean)
                                {
                                    if(c1 == Void || c1 == Boolean)
                                    {
                                        printf("ERROR(%d): '%s' requires operands of type char or type int but lhs is of %s.\n", t->lineno, t->attr.name, types[c1]);
                                        numErrors++;
                                    }

                                    if(c2 == Void || c2 == Boolean)
                                    {
                                        printf("ERROR(%d): '%s' requires operands of type char or type int but rhs is of %s.\n", t->lineno, t->attr.name, types[c2]);
                                        numErrors++;
                                    }
                                }
                                else
                                {
                                    if(c1 == UndefinedType || c2 == UndefinedType)
                                    { /* Do nothing? */ }
                                    else if(c1 != c2)
                                    {
                                        printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", t->lineno, t->attr.name, types[c1], types[c2]);
                                        numErrors++;
                                    }
                                }
                            }
                            t->expType = Boolean;
                        }
                        break;

                    case 5:     //sumop
                    case 6:     //mulop
                        c1 = insertNode(t->child[0]);
                        c2 = insertNode(t->child[1]);

                        if(arr1F == true || arr2F == true)
                        {
                            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                            numErrors++;
                        }
                        else
                        {
                            if(c1 == UndefinedType)
                            {/*Do nothing*/}
                            else if(c1 != Integer)
                            {
                                printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                                numErrors++;
                            }
                            if(c2 == UndefinedType)
                            {/*Do nothing*/}
                            else if(c2 != Integer)
                            {
                                printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", t->lineno, t->attr.name, types[1], types[c2]);
                                numErrors++;
                            }
                        }
                        t->expType = Integer;
                        break;

                    case 7:     //unaryop
                        c1 = insertNode(t->child[0]);

                        if(c1 == UndefinedType)
                        {/*Do nothing*/}
                        else if(strncmp(t->attr.name, "*", 1)== 0)
                        {
                            if(arr1F == false)
                            {
                                printf("ERROR(%d): The operation '%s' only works with arrays.\n", t->lineno, t->attr.name);
                                numErrors++;
                            }
                        }
                        else    // - & ?
                        {
                            if(arr1F == true)
                            {
                                printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                                numErrors++;
                            }
                            else if(c1 != Integer)
                            {
                                printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                                numErrors++;
                            }
                        }
                        t->expType = Integer;
                        break;

                    case 8:     // [
                    //     temp = st.lookupNode(t->child[0]->attr.name);

                    //     if((temp != NULL && temp->isArray == false) || c1 == UndefinedType)
                    //     {
                    //         printf("ERROR(%d): Cannot index nonarray '%s'.\n",t->lineno, t->child[0]->attr.name);
                    //         numErrors++;
                    //     }
                    //     else if(strncmp(t->child[0]->attr.name, "[", 1)== 0)
                    //     {
                    //         printf("ERROR(%d): Cannot index nonarray.\n",t->lineno);
                    //         numErrors++;
                    //     }
                    //         if(t->child[1]->kind.exp == IdK)
                    //         {
                    //             temp = st.lookupNode(t->child[1]->attr.name);
        
                    //             if(temp != NULL && temp->expType != Integer)
                    //             {
                    //                 printf("ERROR(%d): Array '%s' should be indexed by type int but got %s.\n", t->lineno, t->child[0]->attr.name, types[t->child[1]->expType]);
                    //                 numErrors++;
                    //             }

                    //             if(temp != NULL && temp->isArray)
                    //             {
                    //                 if(temp->isIndexed == false)
                    //                 {
                    //                     printf("ERROR(%d): Array index is the unindexed array '%s'.\n", t->lineno, temp->attr.name);
                    //                     numErrors++;
                    //                 }
                    //             }
                    //         }
                    //         else
                    //         {
                    //             if(t->child[1]->expType == UndefinedType)
                    //             { /* Do nothing */ }
                    //             else if(t->child[1]->expType != Integer)
                    //             {
                    //                 printf("ERROR(%d): Array '%s' should be indexed by type int but got %s.\n", t->lineno, t->child[0]->attr.name, types[t->child[1]->expType]);
                    //                 numErrors++;
                    //             }
                    //         }
                    //         t->child[0]->isIndexed = true;
                    //         t->isIndexed = true;
                    //         c1F = false;
                    t->expType = t->child[0]->expType; //lhs
                    break;

                    default:
                        break;
                }

                returns = t->expType;
                break;

            case ConstantK:
                returns = t->expType;
                break;

            case IdK:
                temp = st.lookupNode(t->attr.name);         //Assign return of lookupNode to temporary TreeNode
                if(temp == NULL)                            //Not declared
                {
                    t->expType = UndefinedType;             //Set to undefined type
                    printf("ERROR(%d): Symbol '%s' is not declared.\n", t->lineno, t->attr.name);
                    numErrors++;
                }
                else                                        //Is declared
                {
                    if(temp->kind.decl == FuncK)            //Error in calling a function as a variable
                    {
                        t->expType = UndefinedType;
                        printf("ERROR(%d): Cannot use function '%s' as a variable.\n", t->lineno, t->attr.name);
                        numErrors++;
                    }
                    else                                    //Assign the ID with a type   
                    {
                        t->expType = temp->expType;
                    }   
                }
                if(t != NULL && t->isExp == true)
                {
                    if(temp != NULL)
                    {
                        temp->isUsed = true;
                    }
                }
                returns = t->expType;
                break;

            case AssignK:  
                if(t->child[0] != NULL)
                {
                    t->child[0]->isChecked = true;  //CAUTION

                    temp = st.lookupNode(t->child[0]->attr.name);

                    if(temp != NULL) // && temp->kind.exp == IdK
                    {
                        if(temp->kind.exp == IdK)
                        {
                            if(temp->isArray == true)
                            { arr1F = true; }

                            id1 = true;
                        }
                    }
                    else
                    {
                        temp = t->child[0];     //Catch '[' case
                    }
                    
                }

                if(t->child[1] != NULL)
                {
                    t->child[1]->isChecked = true;  //CAUTION

                    temp2 = st.lookupNode(t->child[1]->attr.name);

                    if(temp2 != NULL)  //&& temp2->kind.exp == IdK
                    {
                        if(temp2->kind.exp == IdK)
                        {
                            if(temp2->isArray == true)
                            { arr2F = true; }
                        }
                    }
                    else
                    {
                        temp2 = t->child[0];     //Catch '[' case
                    }
                }

                switch(t->op)
                {
                    case 1:     // =
                        //Child 1 Initialized
                        //c1 = insertNode(t->child[0]);
                        //t->expType = c1;
                        break;
                    case 2:     // +=
                        break;
                    case 3:     // -=
                        break;
                    case 4:     // *=
                        break;
                    case 5:     // /=
                        break;
                    case 6:     // ++
                    case 7:     // --
                        c1 = insertNode(t->child[0]);

                        if(c1 != Integer)
                        {
                            printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                            numErrors++;
                        }
                        t->expType = Integer;
                        break;
                }
                returns = t->expType;
                break;

            case CallK:
                temp = st.lookupNode(t->attr.name);         //Assign return of lookupNode to temporary TreeNode

                if(temp == NULL)                            //Not declared
                {   
                    t->expType = UndefinedType;
                    printf("ERROR(%d): Symbol '%s' is not declared.\n", t->lineno, t->attr.name);
                    numErrors++;
                }
                else
                {
                    if(temp->kind.decl != FuncK)            //Error in calling a function as a variable
                    {
                        t->expType = temp->expType;
                        printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", t->lineno, t->attr.name);
                        numErrors++;
                    }
                    else
                    {
                        t->expType = temp->expType;
                    }
                    temp->isUsed = true;
                    
                }
                returns = t->expType;
                break;

            default:
                returns = Error;
                break;
        }
    }
    //INVALID
    else if(t->nodekind == StmtK)
    {
        switch(t->kind.stmt)
        {
            case ElsifK:
                returns = Void;
                break;

            case IfK:
                temp = st.lookupNode(t->child[0]->attr.name);
                    if(temp != NULL)
                    {
                            if(temp->isInit == false && temp->isFlagged == false)
                            {
                                temp->isFlagged = true;
                                printf("WARNING(%d): Variable %s may be uninitialized when used here.\n", t->lineno, temp->attr.name);
                                numWarnings++;
                            }
                    }
                returns = Void;
                break;

            case WhileK:
                returns = Void;
                break;

            case LoopK:
                if(t->child[2] != NULL)                     //Apparently Null checking makes this work if it is not a compound????
                {
                    if(t->child[2]->kind.stmt == CompoundK) //Set the enteredScope bool to true for the following compound statement
                    {
                        t->child[2]->enteredScope = true;
                    }
                }
                st.enter("Loop");
                scoped = true;
                returns = Void;
                break;

            case LoopForeverK:
                returns = Void;
                break;

            case CompoundK:
                if(!t->enteredScope)                    //Check that it is not a function scope before
                {
                    st.enter("Compound Scope"); 
                    scoped = true;
                }
                returns = Void;
                break;

            case RangeK:
                returns = Void;
                break;

            case ReturnK:
                if(t->child[0] != NULL)
                {
                    if(t->child[0]->kind.exp == IdK)
                    {
                        temp = st.lookupNode(t->child[0]->attr.name);
                        temp->isUsed = true;

                        if(temp != NULL && temp->isArray == true)
                        {
                            printf("ERROR(%d): Cannot return an array.\n",t->lineno);
                            numErrors++;
                        }

                        if(temp != NULL && temp->isInit == false)
                        {
                            printf("WARNING(%d): Variable %s may be uninitialized when used here.\n", t->child[0]->lineno, temp->attr.name);
                            numWarnings++;
                        }
                    }
                }
                returns = Void;
                break;

            case BreakK:
                returns = Void;
                break;

            default:
                returns = Error;
                break;
         }
    }

    //VALID
    for(i = 0; i < MAXCHILDREN; i++)
    {
        if(t->child[i] != NULL && t->child[i]->isChecked == false)
        {
            insertNode(t->child[i]);
        }
    }

    //VALID
    if(scoped)                          //Leaves the scope as recursive function backtracks
    {
        st.applyToAll(checkUse);
        st.leave();
    }

    //VALID
    if(t->sibling != NULL)
    {
        insertNode(t->sibling); 
    }

    return returns;
}

//INVALID?
void checkUse(std::string sym, void* t)
{
    // TreeNode *temp;
    // temp = st.lookupNode(sym.c_str());

    // if(temp != NULL)
    // {
    //     if(temp->isUsed == false)
    //     {
    //         printf("WARNING(%d): The variable %s seems not to be used.\n", temp->lineno, temp->attr.name);
    //         numWarnings++;
    //     }
    // }
}
