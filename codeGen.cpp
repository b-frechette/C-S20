#include "emitcode.h"
#include "treeNodes.h"
#include "symbolTable.h"
#include "codeGen.h"

extern SymbolTable st;
extern int Goffset;

void ioSetup()
{
    //input
    emitComment((char *)"FUNCTION", (char *)"input");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab int input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"input");

    //output
    emitComment((char *)"FUNCTION", (char *)"output");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
    emitRO((char *)"OUT", 3, 3, 3, (char *)"Output integer");
    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"output");

    //inputb
    emitComment((char *)"FUNCTION", (char *)"inputb");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab bool input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"inputb");

    //outputb
    emitComment((char *)"FUNCTION", (char *)"outpub");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
    emitRO((char *)"OUT", 3, 3, 3, (char *)"Output bool");
    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outputb");

    //inputc
    emitComment((char *)"FUNCTION", (char *)"inputc");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab char input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"inputc");

    //outputc
    emitComment((char *)"FUNCTION", (char *)"outpuc");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
    emitRO((char *)"OUT", 3, 3, 3, (char *)"Output char");
    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outputc");

    //outnl
    emitComment((char *)"FUNCTION", (char *)"outnl");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"OUTNL", 3, 3, 3, (char *)"Output a newline");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outnl");
    emitComment((char *)"END OF I/O");
    emitComment((char *)"\n*\n*\n*");
}

void getParams(TreeNode *s)
{
    if(s != NULL)
    {
        emitComment((char *)"PARAM");
        traverse(s);
        emitRM((char *)"ST", 3, -4, 1, (char *)"Store Parameter");
        getParams(s->sibling);
    }
}

void traverse(TreeNode *s)
{
    TreeNode *temp;
    int savedLoc;

    if (s != NULL)
    {
        if (s->nodekind == DeclK)
        {
            switch (s->kind.decl)
            {
                case VarK:
                    break;

                case FuncK:
                    emitComment((char *)"FUNCTION", (char *)s->attr.name);
                    s->offset = emitSkip(0);
                    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
                    traverse(s->child[1]);
                    emitComment((char *)"Add standard closing in case there is no return statement");
                    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
                    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
                    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
                    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
                    emitComment((char *)"END FUNCITON", (char *)s->attr.name);
                    break;

                case ParamK:
                    break;

                default:
                    break;
            }
        }
        else if (s->nodekind == ExpK)
        {
            switch (s->kind.exp)
            {
                case OpK:
                    switch (s->op)
                    {
                        case 1: //OR
                            break;
                        case 2: //AND
                            break;
                        case 3: //NOT
                            break;

                        case 4: //relop
                            break;

                        case 5: //sumop
                            break;
                        case 6: //mulop
                            break;

                        case 7: //unaryop
                            break;
                        case 8: // [
                            break;
                        default:
                            break;
                    }
                    break;

                case ConstantK:
                    switch(s->expType)
                    {
                        // case 0: //void
                        //     break;
                        case 1: //int
                            emitRM((char *)"LDC", 3, s->attr.value, 6, (char *)"Load integer constant");
                            break;
                        case 2: //bool
                            break;
                        case 3: //char
                            break;
                        default:
                            printf("Default hit for constantk with %d\n", s->expType);
                            break;
                    }
                    break;

                case IdK:
                    break;

                case AssignK:

                    switch (s->op)
                    {
                        case 1: // =
                            break;
                        case 2: // +=
                            break;
                        case 3: // -=
                            break;
                        case 4: // *=
                            break;
                        case 5: // /=
                            break;
                        case 6: // ++
                            break;
                        case 7: // --
                            break;
                    }
                    break;

                case CallK:
                    savedLoc = emitSkip(0);
                    emitComment((char *)"CALL", (char *)s->attr.name);
                    emitRM((char *)"ST", 1, -2,1, (char *)"Store fp in ghost frame for output");
                    getParams(s->child[0]);
                    emitComment((char *)"END PARAM", (char *)s->attr.name);
                    emitRM((char *)"LDA", 1, -2, 1, (char *)"Load address of new frame");
                    emitRM((char *)"LDA", 3, 1, 7, (char *)"Return address in ac");
                    emitRM((char *)"LDA", 7, -savedLoc, 7, (char *)"CALL");
                    emitRM((char *)"LDA", 3, 0, 2, (char *)"Save the result in ac");
                    emitComment((char *)"END CALL", (char *)s->attr.name);
                    break;

                default:
                    break;
            }
        }
        else if (s->nodekind == StmtK)
        {
            switch (s->kind.stmt)
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
                    emitComment((char *)"COMPOUND");
                    emitComment((char *)"Compound Body");
                    traverse(s->child[0]);
                    traverse(s->child[1]);
                    emitComment((char *)"END COMPOUND");
                    break;

                case RangeK:
                    switch (s->op)
                    {
                        //case 1:     ASSIGN simpleExpression RANGE simpleExpression
                        case 2: //ASSIGN simpleExpression RANGE simpleExpression COLON simpleExpression
                            break;

                        default:
                            break;
                    }
                    break;

                case ReturnK:
                    emitComment((char *)"RETURN");
                    traverse(s->child[0]);
                    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
                    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
                    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
                    break;

                case BreakK:
                    break;

                default:
                    break;
            }
        }
        else
        {
            printf("This shouldn't be happening\n");
        }

        if(s->sibling != NULL)
        {
            traverse(s->sibling); 
        }
    }
}

void codeGen(TreeNode *s)
{
    TreeNode *temp = st.lookupNode("main");

    emitComment((char *)"TM Code");
    emitSkip(1);

    ioSetup();

    traverse(s);

    backPatchAJumpToHere(0, (char *)"Jump to init [backpatch]");

    emitComment((char *)"INIT");
    emitRM((char *)"LD", 0, 0, 0, (char *)"Set the global pointer");
    emitRM((char *)"LDA", 1, Goffset, 0, (char *)"Set first frame at end of globals");
    emitRM((char *)"ST", 1, 0, 1, (char *)"Store old fp (point to self)");
    
    emitComment((char *)"INIT GLOBALS AND STATICS");
    emitComment((char *)"END INIT GLOBALS AND STATICS");

    emitRM((char *)"LDA", 3, 1, 7, (char *)"Return address in ac");
    //Sketch or logically sound fix?
    emitRM((char *)"LDA", 7, temp->offset - (emitSkip(0)+1), 7, (char *)"Jump to main");
    emitRO((char *)"HALT", 0, 0, 0, (char *)"DONE!");
    emitComment((char *)"END INIT");
}