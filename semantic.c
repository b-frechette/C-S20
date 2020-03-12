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

    //st.print(pointerPrintStr);

    if(st.lookupGlobal("main") == NULL)
    {
        printf("ERROR(LINKER): Procedure main is not declared.\n");
        numErrors++;
    }
}

ExpType insertNode(TreeNode *t)
{
    int i;
    ExpType c1, c2, returns;
    bool scoped = false;
    const char* types[] = {"type void", "type int", "type bool", "type char", "type char", "equal", "undefined type", "error"};
    TreeNode *temp;

    if(t == NULL)
    {return Error;}

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

                if(t->child[1] != NULL)                     //Apparently Null checking makes this work if it is not a compound????
                {
                    if(t->child[1]->kind.stmt == CompoundK) //Set the enteredScope bool to true for the following compound statement
                    {
                        t->child[1]->enteredScope = true;
                    }
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
                returns = t->expType;
                break;

            default:
                returns = Error;
                break;
        }
    }
    else if(t->nodekind == ExpK)
    {
        switch(t->kind.exp)
        {
            case OpK:
                c1 = insertNode(t->child[0]);       //Get the types of the children
                c2 = insertNode(t->child[1]);

                t->child[0]->isChecked = true;      //Mark them as already checked
                if(t->child[1] != NULL)
                {
                    t->child[1]->isChecked = true;
                }

                switch(t->op)
                {
                    case 1:         //OR
                        if(c1 == UndefinedType)
                        {/*Do nothing*/}
                        else if(c1 != Boolean)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", t->lineno, t->attr.name, types[2], types[c1]);
                            numErrors++;
                        }
                        if(c2 == UndefinedType)
                        {/*Do nothing*/}
                        else if(c2 != Boolean)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", t->lineno, t->attr.name, types[2], types[c2]);
                             numErrors++;
                        }
                        t->expType = Boolean;
                        break;
                    
                    case 2:     //AND
                        if(c1 == UndefinedType)
                        {/*Do nothing*/}
                        else if(c1 != Boolean)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", t->lineno, t->attr.name, types[2], types[c1]);
                            numErrors++;
                        }
                        if(c2 == UndefinedType)
                        {/*Do nothing*/}
                        else if(c2 != Boolean)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", t->lineno, t->attr.name, types[2], types[c2]);
                            numErrors++;
                        }
                        t->expType = Boolean;
                        break;

                    case 3:     //NOT
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
                        if(strncmp(t->attr.name, "==", 2)== 0 || strncmp(t->attr.name, "!=", 2)== 0)
                        {
                            if(c1 == UndefinedType || c2 == UndefinedType)
                            {/*Do nothing*/}
                            else if(c1 != c2)
                            {
                                printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", t->lineno, t->attr.name, types[c1], types[c2]);
                                numErrors++;
                            }
                            t->expType = Boolean;
                        }
                        else    // <=, <, >=, >
                        {
                            //To do check the args
                            t->expType = Boolean;
                        }
                        break;

                    case 5:     //sumop
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
                        t->expType = Integer;
                        break;

                    case 6:     //mulop
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
                        t->expType = Integer;
                        break;

                    case 7:     //unaryop
                        if(c1 == UndefinedType)
                        {/*Do nothing*/}
                        else if(strncmp(t->attr.name, "*", 1)== 0)
                        {
                            //TO DO
                        }
                        else    // - & ?
                        {
                            if(c1 != Integer)
                            {
                                printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                                numErrors++;
                            }
                        }
                        t->expType = Integer;
                        break;

                    case 8:     // [
                        // printf("case %s\n", t->attr.name);
                        //To do type check the two arguments
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

                        if(temp->isInit == false)
                        {
                            // printf("WARNING(%d): Variable %s may be uninitialized when used here.\n", t->lineno, t->attr.name);
                            // numWarnings++;
                        }
                    }   
                }

                returns = t->expType;
                break;

            case AssignK:   //check the children recurisively first
                c1 = insertNode(t->child[0]);       //Get the types of the children
                c2 = insertNode(t->child[1]);

                t->child[0]->isChecked = true;      //Mark them as already checked
                if(t->child[1] != NULL)
                {
                    t->child[1]->isChecked = true;
                }

                if(c1 == Error || c2 == Error || c1 == UndefinedType || c2 == UndefinedType)          //If a function was called break to let error be called with the id
                {
                    t->expType = UndefinedType;
                }
                
                if(strncmp(t->attr.name, "--", 2) == 0)
                {
                    // printf("<\n");
                    t->expType = Integer;

                    if(c1 != Integer)
                    {
                        printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                        numErrors++;
                    }
                }
                else if(strncmp(t->attr.name, "++", 2) == 0)
                {
                    // printf("<\n");
                    t->expType = Integer;
                }
                else if(strncmp(t->attr.name, "=", 1) == 0)
                {
                    // printf("<\n");
                    if(c1 == UndefinedType || c2 == UndefinedType)
                    {
                        //Do nothing?
                    }
                    else if(c1 != c2)                       //if they are not equal
                    { 
                        printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", t->lineno, t->attr.name, types[c1], types[c2]);
                        numErrors++;
                    } 
                    t->expType = c1;
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

                    t->expType = Integer;
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
                    
                }
                returns = t->expType;
                break;

            default:
                returns = Error;
                break;
        }
    }
    else if(t->nodekind == StmtK)
    {
        switch(t->kind.stmt)
        {
            case ElsifK:
                returns = Void;
                break;

            case IfK:
                returns = Void;
                break;

            case WhileK:
                returns = Void;
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

    for(i = 0; i < MAXCHILDREN; i++)
    {
        if(t->child[i] != NULL && t->child[i]->isChecked == false)
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

    return returns;
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
            //printf("WARNING(%d): The variable %s seems not to be used.\n", temp->lineno, temp->attr.name);
            //numWarnings++;
        }
    }
}
