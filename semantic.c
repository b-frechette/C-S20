#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "treeNodes.h"
#include "symbolTable.h"
#include "semantic.h"

SymbolTable st;
TreeNode *currFunc;
bool returnFlg = false;

void semantic(TreeNode *syntaxTree)
{
    insertNode(syntaxTree);

    st.applyToAll(checkUse);

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
    bool arr1F = false, arr2F = false, n1 = true, n2 = true;
    const char* types[] = {"type void", "type int", "type bool", "type char", "type char", "equal", "undefined type", "error"};
    const char* stmt[] = { "null", "elsif", "if", "while" };
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
                    t->child[0]->isChecked = true;

                    c1 = insertNode(t->child[0]);

                    if(t->child[0]->nodekind == ExpK)
                    {
                        if(t->child[0]->kind.exp == ConstantK)
                        {   
                            if(t->child[0]->expType != t->expType)
                            {
                                printf("ERROR(%d): Variable '%s' is of %s but is being initialized with an expression of %s.\n", t->lineno, t->attr.name, types[t->expType], types[t->child[0]->expType]);
                                numErrors++;
                            }  
                        }
                        else if(t->child[0]->kind.exp == OpK)
                        {
                            if(c1 != t->expType)
                            {
                                printf("ERROR(%d): Variable '%s' is of %s but is being initialized with an expression of %s.\n", t->lineno, t->attr.name, types[t->expType], types[temp->expType]);
                                numErrors++;
                            }
                        }
                        else
                        {
                            if(t->child[0]->kind.exp == IdK)
                            {
                                temp2 = st.lookupNode(t->child[0]->attr.name);

                                if(temp2 != NULL)
                                { temp2->isUsed = true; }
                            }

                            printf("ERROR(%d): Initializer for variable '%s' is not a constant expression.\n", t->lineno, t->attr.name);
                            numErrors++;
                        }
                    }
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
                currFunc = t;
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

                    if(t->child[0]->kind.exp == IdK)
                    {
                        //t->child[0]->isChecked = true;  //CAUTION
                        temp = st.lookupNode(t->child[0]->attr.name);

                        if(temp != NULL)  //&& temp->kind.exp == IdK
                        {
                            n1 = false;
                            if(temp->isArray == true)
                            { arr1F = true; }
                        }
                    }
                }

                if(t->child[1] != NULL)
                {
                    t->child[1]->isChecked = true;  //CAUTION

                    if(t->child[1]->kind.exp == IdK)
                    {
                        temp2 = st.lookupNode(t->child[1]->attr.name);

                        if(temp2 != NULL)  //&& temp2->kind.exp == IdK
                        {
                            n2 = false;
                            if(temp2->isArray == true)
                            { arr2F = true; }
                        }
                    }
                }

                switch(t->op)
                {
                    case 1:     //OR
                    case 2:     //AND
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

                        //Check if array
                        if(arr1F || arr2F)
                        {
                            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                            numErrors++;  
                        }

                        t->expType = Boolean;
                        break;

                    case 3:     //NOT
                        c1 = insertNode(t->child[0]);

                        if(c1 == UndefinedType)
                        {/*Do nothing*/}
                        else if(c1 != Boolean)
                        {
                            printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[2], types[c1]);
                            numErrors++;
                        }

                        //Check if array
                        if(arr1F == true)
                        {
                            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
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

                        if(arr1F == true || arr2F == true)
                        {
                            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                            numErrors++;
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
                            if(c1 != Integer)
                            {
                                printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                                numErrors++;
                            }

                            if(arr1F == true)
                            {
                                printf("ERROR(%d): The operation '%s' does not work with arrays.\n", t->lineno, t->attr.name);
                                numErrors++;
                            }
                        }
                        t->expType = Integer;
                        break;

                    case 8:     // [
                        if(t->isInit)
                        {
                            if(!n1)
                            { temp->isInit = true; }
                            else if(strncmp(t->child[0]->attr.name, "[", 1)== 0)
                            {
                                t->child[0]->isInit = true;
                            }

                        }

                        c1 = insertNode(t->child[0]);
                        c2 = insertNode(t->child[1]);

                        //Check that child[0] is an array
                        if(strncmp(t->child[0]->attr.name, "[", 1)== 0)
                        {
                            printf("ERROR(%d): Cannot index nonarray.\n",t->lineno);
                            numErrors++;
                        }
                        else if((!n1 && !arr1F) || c1 == UndefinedType) //Careful
                        {
                            printf("ERROR(%d): Cannot index nonarray '%s'.\n",t->lineno, t->child[0]->attr.name);
                            numErrors++;
                        }

                        if(!n2)
                        {
                            //Ensure array is being indexed by an Integer
                            if(c2 == UndefinedType)
                            {/*Do Nothing */}
                            else if(c2 != Integer)
                            {
                                printf("ERROR(%d): Array '%s' should be indexed by type int but got %s.\n", t->lineno, t->child[0]->attr.name, types[c2]);
                                numErrors++;
                            }  

                            //Check if array is being used for index, it is indexed
                            if(arr2F && !temp2->isIndexed)
                            {

                                printf("ERROR(%d): Array index is the unindexed array '%s'.\n", t->lineno, temp2->attr.name);
                                numErrors++;
                            }
                        }
                        else
                        {
                            if(t->child[1]->expType == UndefinedType)
                            { /* Do nothing */ }
                            else if(t->child[1]->expType != Integer)
                            {
                                printf("ERROR(%d): Array '%s' should be indexed by type int but got %s.\n", t->lineno, t->child[0]->attr.name, types[t->child[1]->expType]);
                                numErrors++;
                            }  
                        }
    
                        t->child[0]->isIndexed = true;
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
                    temp->isUsed = true;
                    if(temp->kind.decl == FuncK)            //Error in calling a function as a variable
                    {
                        temp->isFlagged = true;
                        t->expType = UndefinedType;
                        printf("ERROR(%d): Cannot use function '%s' as a variable.\n", t->lineno, t->attr.name);
                        numErrors++;
                    }
                    else                                    //Assign the ID with a type   
                    {
                        t->expType = temp->expType;
                    }  

                    if(!temp->isInit && !temp->isFlagged)
                    {
                        temp->isFlagged = true;
                        printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", t->lineno, temp->attr.name);
                        numWarnings++;
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

                    if(t->child[0]->kind.exp == IdK)
                    {
                        temp = st.lookupNode(t->child[0]->attr.name);

                        if(temp != NULL)  //&& temp->kind.exp == IdK
                        {
                            n1 = false;
                            if(temp->isArray == true)
                            { arr1F = true; }
                        }
                    }
                }

                if(t->child[1] != NULL)
                {
                    t->child[1]->isChecked = true;  //CAUTION

                    if(t->child[1]->kind.exp == IdK)
                    {
                        temp2 = st.lookupNode(t->child[1]->attr.name);

                        if(temp2 != NULL)  //&& temp2->kind.exp == IdK
                        {
                            n2 = false;
                            if(temp2->isArray == true)
                            { arr2F = true; }
                        }
                        else
                        {
                            temp2 = t->child[0];     //Catch '[' case
                        }
                    }
                }

                switch(t->op)
                {
                    case 1:     // =
                        //Child 1 Initialized
                        if(!n1)
                        { temp->isInit = true; }
                        else
                        { t->child[0]->isInit = true; }

                        c1 = insertNode(t->child[0]);
                        c2 = insertNode(t->child[1]);

                        if(c1 == UndefinedType || c2 == UndefinedType)
                        { /* Do nothing? */ }
                        else if(c1 != c2) 
                        {
                            printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", t->lineno, t->attr.name, types[c1], types[c2]);
                            numErrors++;
                        }

                        t->expType = c1;
                        break;

                    case 2:     // +=
                    case 3:     // -=
                    case 4:     // *=
                    case 5:     // /=
                        c1 = insertNode(t->child[0]);
                        c2 = insertNode(t->child[1]);  
                        
                        if(c1 == UndefinedType)
                        {/* Do Nothing*/ }
                        else if(c1 != Integer)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", t->lineno, t->attr.name, types[1], types[c1]);
                            numErrors++;
                        }

                        if(c2 == UndefinedType)
                        {/* Do Nothing*/ }
                        else if(c2 != Integer)
                        {
                            printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", t->lineno, t->attr.name, types[1], types[c2]);
                            numErrors++;
                        }

                        t->expType = Integer;
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
            case IfK:
            case WhileK:
                c1 = insertNode(t->child[0]);

                if(t->child[0] != NULL)
                { t->child[0]->isChecked = true; }

                if(c1 == UndefinedType)
                { /*Do Nothing*/ }
                else if(c1 != Boolean)
                {
                    printf("ERROR(%d): Expecting Boolean test condition in %s statement but got %s.\n", t->lineno, stmt[t->kind.stmt], types[c1]);
                    numErrors++;
                }

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
                c1 = insertNode(t->child[0]);

                if(t->child[0] != NULL)
                {
                    t->child[0]->isChecked = true;

                    temp = st.lookupNode(t->child[0]->attr.name);

                    if(temp != NULL)
                    {
                        temp->isInit = true;
                    }
                }

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
                    t->child[0]->isChecked = true;
                    if(t->child[0]->kind.exp == IdK)
                    {
                        temp = st.lookupNode(t->child[0]->attr.name);

                        if(temp != NULL && temp->isArray == true)
                        {
                            printf("ERROR(%d): Cannot return an array.\n",t->lineno);
                            numErrors++;
                        }
                    }
                }

                c1 = insertNode(t->child[0]);
 
                if(currFunc != NULL)
                {
                    if(t->child[0] != NULL)
                    {
                        if(c1 == UndefinedType)
                        { /* Do Nothing */}
                        else if(currFunc->expType == Void)
                        {
                            printf("ERROR(%d): Function '%s' at line %d is expecting no return value, but return has return value.\n", t->lineno, currFunc->attr.name, currFunc->lineno);
                            numErrors++;
                        }
                        else if(c1 != currFunc->expType)
                        {
                            printf("ERROR(%d): Function '%s' at line %d is expecting to return %s but got %s.\n", t->lineno, currFunc->attr.name, currFunc->lineno, types[currFunc->expType], types[c1]);
                            numErrors++;
                        }
                    }
                    else
                    {
                        if(currFunc->expType != Void)
                        {
                            printf("ERROR(%d): Function '%s' at line %d is expecting to return %s but return has no return value.\n", t->lineno, currFunc->attr.name, currFunc->lineno, types[currFunc->expType]);
                            numErrors++;  
                        }
                    }
                }

                returns = Void;
                returnFlg = true;
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
        if(strncmp(currFunc->attr.name, st.scope().c_str(), 10) == 0)
        {
            if(!returnFlg && currFunc->expType != Void)
            {
                printf("WARNING(%d): Expecting to return %s but function '%s' has no return statement.\n", t->lineno, types[currFunc->expType], currFunc->attr.name);
                numWarnings++;
            }
            else
            { returnFlg = false; }
        }
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
    TreeNode *temp;
    temp = st.lookupNode(sym.c_str());

    if(temp != NULL)
    {
        if(temp->isUsed == false)
        {
            if(temp->nodekind == DeclK && temp->kind.decl == FuncK)
            {
                //To do
            }
            else if(temp->nodekind == DeclK && temp->kind.decl == ParamK)
            {
                printf("WARNING(%d): The parameter '%s' seems not to be used.\n", temp->lineno, temp->attr.name);
                numWarnings++;
            }
            else
            {
                printf("WARNING(%d): The variable '%s' seems not to be used.\n", temp->lineno, temp->attr.name);
                numWarnings++;  
            }
        }
    }
}
