#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <cstring>
#include "emitcode.h"
#include "treeNodes.h"
#include "symbolTable.h"
#include "codeGen.h"

extern SymbolTable st;
extern int Goffset;
//int tmpOffset = 0; Deprecated in favor of handling recursively RIP simplicity

void ioSetup()
{
    //input
    emitComment((char *)"FUNCTION", (char *)"input");
    ((TreeNode *)(st.lookup("input")))->offset = emitSkip(0);
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab int input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"input");

    //output
    emitComment((char *)"FUNCTION", (char *)"output");
    ((TreeNode *)(st.lookup("output")))->offset = emitSkip(0);
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
    ((TreeNode *)(st.lookup("inputb")))->offset = emitSkip(0);
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab bool input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"inputb");

    //outputb
    emitComment((char *)"FUNCTION", (char *)"outpub");
    ((TreeNode *)(st.lookup("outputb")))->offset = emitSkip(0);
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
    ((TreeNode *)(st.lookup("inputc")))->offset = emitSkip(0);
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab char input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"inputc");

    //outputc
    emitComment((char *)"FUNCTION", (char *)"outputc");
    ((TreeNode *)(st.lookup("outputc")))->offset = emitSkip(0);
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
    ((TreeNode *)(st.lookup("outnl")))->offset = emitSkip(0);
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"OUTNL", 3, 3, 3, (char *)"Output a newline");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outnl");
    emitComment((char *)"END OF I/O");
    emitComment((char *)"\n*\n*\n*");
}

void getParams(TreeNode *s, int offset)
{
    if(s != NULL)
    {
        emitComment((char *)"PARAM");
        traverse(s, offset);
        emitRM((char *)"ST", 3, offset, 1, (char *)"Store Parameter");
        getParams(s->sibling, offset - 1);
    }
}

void traverse(TreeNode *s, int offset)
{
    TreeNode *temp;
    int savedLoc, savedLoc2, savedLoc3;

    if (s != NULL)
    {
        if (s->nodekind == DeclK)
        {
            switch (s->kind.decl)
            {
                case VarK:
                    if(s->var == Local)
                    {
                        if(s->isArray)
                        {
                            emitRM((char *)"LDC", 3, s->size - 1, 6, (char *)"Load size of array");
                            emitRM((char *)"ST", 3, s->offset + 1, 1, (char *)"Save size of array");
                        }
                    }
                    break;

                case FuncK:
                    emitComment((char *)"FUNCTION", (char *)s->attr.name);
                    s->offset = emitSkip(0);
                    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
                    offset += s->size;
                    traverse(s->child[1], offset);
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
                            similarEmit(s, offset);
                            emitRO((char *)"OR", 3, 4, 3, (char *)"Op OR");
                            break;
                        case 2: //AND
                            similarEmit(s, offset);
                            emitRO((char *)"AND", 3, 4, 3, (char *)"Op AND");
                            break;
                        case 3: //NOT
                            traverse(s->child[0], offset);
                            emitRM((char *)"LDC", 4, 1, 6, (char *)"Load 1");
                            emitRO((char *)"XOR", 3, 3, 4, (char *)"Op NOT");
                            break;

                        case 4: //relop
                            if(strncmp(s->attr.name, "==", 2)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"TEQ", 3, 4, 3, (char *)"Op ==");
                            }
                            else if(strncmp(s->attr.name, ">", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"TGT", 3, 4, 3, (char *)"Op >");
                            }
                            else if(strncmp(s->attr.name, "<", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"TLT", 3, 4, 3, (char *)"Op <");
                            }
                            break;

                        case 5: //sumop
                            if(strncmp(s->attr.name, "+", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"ADD", 3, 4, 3, (char *)"Op +");
                            }
                            else if(strncmp(s->attr.name, "-", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"SUB", 3, 4, 3, (char *)"Op -");
                            }
                            break;
                        case 6: //mulop
                            if(strncmp(s->attr.name, "*", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"MUL", 3, 4, 3, (char *)"Op *");
                            }
                            else if(strncmp(s->attr.name, "%", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"DIV", 5, 4, 3, (char *)"Op %");
                                emitRO((char *)"MUL", 5, 5, 3, (char *)"");
                                emitRO((char *)"SUB", 3, 4, 5, (char *)"");
                            }
                            else if(strncmp(s->attr.name, "/", 1)== 0)
                            {
                                similarEmit(s, offset);
                                emitRO((char *)"DIV", 3, 4, 3, (char *)"Op /");
                            }
                            break;

                        case 7: //unaryop
                            if(strncmp(s->attr.name, "?", 1)== 0)
                            {
                                traverse(s->child[0], offset);
                                emitRO((char *)"RND", 3, 3, 6, (char *)"Op ?");
                            }
                            else if(strncmp(s->attr.name, "-", 1)== 0)
                            {
                                traverse(s->child[0], offset);
                                emitRM((char *)"LDC", 4, 0, 6, (char *)"Load 0");
                                emitRO((char *)"SUB", 3, 4, 3, (char *)"Op unary -");
                            }
                            break;
                        case 8: // [
                            similarEmit(s, offset);
                            emitRO((char *)"SUB", 3, 4, 3, (char *)"Compute location from frame index");
                            emitRM((char *)"LD", 3, 0, 3, (char *)"Load array element");
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
                            emitRM((char *)"LDC", 3, atoi(s->attr.name), 6, (char *)"Load integer constant");
                            break;
                        case 2: //bool
                            emitRM((char *)"LDC", 3, s->attr.value, 6, (char *)"Load Boolean constant");
                            break;
                        case 3: //char
                            emitRM((char *)"LDC", 3, s->attr.cvalue, 6, (char *)"Load char constant");
                            break;
                        default:
                            printf("Default hit for constantk with %d\n", s->expType);
                            break;
                    }
                    break;

                case IdK:
                    if(s->isArray)
                    {   
                        emitRM((char *)"LDA", 3, s->offset, s->var != Global, (char *)"Load address of base of array");
                    }
                    else
                        emitRM((char *)"LD", 3, s->offset, s->var != Global, (char *)"Load variable");
                    break;

                case AssignK:

                    switch (s->op)
                    {
                        case 1: // =
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"ST", 3, offset, 1, (char *)"Save index"); 
                                traverse(s->child[1], offset - 1); //Check
                                emitRM((char *)"LD", 4, offset, 1, (char *)"Restore index"); 
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                                emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable");
                            }
                            else
                            {
                                traverse(s->child[1], offset);
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Store variable");
                            }
                            break;
                        case 2: // +=
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"ST", 3, offset, 1, (char *)"Save index"); 
                                traverse(s->child[1], offset - 1); //Check
                                emitRM((char *)"LD", 4, offset, 1, (char *)"Restore index"); 
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                                emitRM((char *)"LD", 4, s->child[0]->offset, 5, (char *)"Load lhs variable");
                                emitRO((char *)"ADD", 3, 4, 3, (char *)"Op +=");
                                emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable");
                            }
                            else
                            {
                                traverse(s->child[1], offset);
                                emitRM((char *)"LD", 4, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                                emitRO((char *)"ADD", 3, 4, 3, (char *)"Op +=");
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Store variable");
                            }
                            break;
                        case 3: // -=
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"ST", 3, offset, 1, (char *)"Save index"); 
                                traverse(s->child[1], offset - 1); //Check
                                emitRM((char *)"LD", 4, offset, 1, (char *)"Restore index"); 
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                                emitRM((char *)"LD", 4, s->child[0]->offset, 5, (char *)"Load lhs variable");
                                emitRO((char *)"SUB", 3, 4, 3, (char *)"Op -=");
                                emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable");
                            }
                            else
                            {
                                traverse(s->child[1], offset);
                                emitRM((char *)"LD", 4, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                                emitRO((char *)"SUB", 3, 4, 3, (char *)"Op -=");
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Store variable");
                            }
                            break;
                        case 4: // *=
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"ST", 3, offset, 1, (char *)"Save index"); 
                                traverse(s->child[1], offset - 1); //Check
                                emitRM((char *)"LD", 4, offset, 1, (char *)"Restore index"); 
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                                emitRM((char *)"LD", 4, s->child[0]->offset, 5, (char *)"Load lhs variable");
                                emitRO((char *)"MUL", 3, 4, 3, (char *)"Op *=");
                                emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable");
                            }
                            else
                            {
                                traverse(s->child[1], offset);
                                emitRM((char *)"LD", 4, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                                emitRO((char *)"MUL", 3, 4, 3, (char *)"Op *=");
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Store variable");
                            }
                            break;
                        case 5: // /=
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"ST", 3, offset, 1, (char *)"Save index"); 
                                traverse(s->child[1], offset - 1); //Check
                                emitRM((char *)"LD", 4, offset, 1, (char *)"Restore index"); 
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 4, (char *)"Compute offset of value");
                                emitRM((char *)"LD", 4, s->child[0]->offset, 5, (char *)"Load lhs variable");
                                emitRO((char *)"DIV", 3, 4, 3, (char *)"Op /=");
                                emitRM((char *)"ST", 3, 0, 5, (char *)"Store variable");
                            }
                            else
                            {
                                traverse(s->child[1], offset);
                                emitRM((char *)"LD", 4, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                                emitRO((char *)"DIV", 3, 4, 3, (char *)"Op /=");
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Store variable");
                            }
                            break;
                        case 6: // ++
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 3, (char *)"Compute offset of value");
                                emitRM((char *)"LD", 3, s->child[0]->offset, 5, (char *)"Load lhs variable");
                                emitRO((char *)"LDA", 3, 1, 3, (char *)"Increment value");
                                emitRM((char *)"ST", 3, s->child[0]->offset, 5, (char *)"Store variable");
                            }
                            else
                            {
                                emitRM((char *)"LD", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                                emitRO((char *)"LDA", 3, 1, 3, (char *)"Increment value");
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Store variable");
                            }
                            break;
                        case 7: // --
                            if(s->child[0]->op == 8)
                            {
                                traverse(s->child[0]->child[1], offset);
                                emitRM((char *)"LDA", 5, s->child[0]->child[0]->offset, s->child[0]->child[0]->var != Global, (char *)"Load address of base of array");
                                emitRO((char *)"SUB", 5, 5, 3, (char *)"Compute offset of value");
                                emitRM((char *)"LD", 3, s->child[0]->offset, 5, (char *)"Load lhs variable");
                                emitRO((char *)"LDA", 3, -1, 3, (char *)"Decrement value");
                                emitRM((char *)"ST", 3, s->child[0]->offset, 5, (char *)"Store variable");
                            }
                            else
                            {
                                emitRM((char *)"LD", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                                emitRO((char *)"LDA", 3, -1, 3, (char *)"Decrement value");
                                emitRM((char *)"ST", 3, s->child[0]->offset, s->child[0]->var != Global, (char *)"Load lhs variable");
                            }
                            break;
                    }
                    break;

                case CallK:
                    temp = st.lookupNode(s->attr.name);
                    emitComment((char *)"CALL", (char *)s->attr.name);
                    emitRM((char *)"ST", 1, offset, 1, (char *)"Store fp in ghost frame for output");
                    getParams(s->child[0], offset - 2);
                    emitComment((char *)"END PARAM", (char *)s->attr.name);
                    emitRM((char *)"LDA", 1, offset, 1, (char *)"Load address of new frame");
                    emitRM((char *)"LDA", 3, 1, 7, (char *)"Return address in ac");
                    emitGotoAbs(temp->offset, (char *)"CALL");
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
                    emitComment((char *)"IF");
                    traverse(s->child[0], offset);
                    savedLoc = emitSkip(1);

                    emitComment((char *)"THEN");
                    traverse(s->child[1], offset);
                    if(s->child[2] != NULL)
                        savedLoc2 = emitSkip(1);
                    backPatchAJumpToHere((char *)"JZR", 3, savedLoc, (char *)"Jump around the THEN if false [backpatch]");

                    if(s->child[2] != NULL)
                    {
                        emitComment((char *)"ELSE");
                        traverse(s->child[2], offset);
                        backPatchAJumpToHere(savedLoc2, (char *)"Jump around the ELSE [backpatch]");
                    }

                    emitComment((char *)"END IF");
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
                    traverse(s->child[0], offset);
                    traverse(s->child[1], offset);
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
                    traverse(s->child[0], offset);
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
            traverse(s->sibling, offset); 
        }
    }
}

void codeGen(TreeNode *s)
{
    TreeNode *temp = st.lookupNode("main");

    emitComment((char *)"TM Code");
    emitSkip(1);

    ioSetup();

    traverse(s, 0);

    backPatchAJumpToHere(0, (char *)"Jump to init [backpatch]");

    emitComment((char *)"INIT");
    emitRM((char *)"LD", 0, 0, 0, (char *)"Set the global pointer");
    emitRM((char *)"LDA", 1, Goffset, 0, (char *)"Set first frame at end of globals");
    emitRM((char *)"ST", 1, 0, 1, (char *)"store old fp (point to self)");
    
    emitComment((char *)"INIT GLOBALS AND STATICS");
    st.applyToAllGlobal(globalInit);
    emitComment((char *)"END INIT GLOBALS AND STATICS");

    emitRM((char *)"LDA", 3, 1, 7, (char *)"Return address in ac");
    //Sketch or logically sound fix?
    emitRM((char *)"LDA", 7, temp->offset - (emitSkip(0)+1), 7, (char *)"Jump to main"); //FIX THIS
    emitRO((char *)"HALT", 0, 0, 0, (char *)"DONE!");
    emitComment((char *)"END INIT");
}

void similarEmit(TreeNode *s, int offset)
{
    traverse(s->child[0], offset);
    emitRM((char *)"ST", 3, offset, 1, (char *)"Save left side");
    traverse(s->child[1], offset - 1);
    emitRM((char *)"LD", 4, offset, 1, (char *)"Load left into ac1");
}

void globalInit(std::string sym, void* s)
{
    TreeNode *temp;
    temp = st.lookupNode(sym.c_str());
    
    if(temp->nodekind == DeclK && temp->kind.decl == VarK)
    {
        if(temp->isArray)
        {
            emitRM((char *)"LDC", 3, temp->size - 1, 6, (char *)"load size of array");
            emitRM((char *)"ST", 3, temp->offset + 1, 0, (char *)"save size of array");
        }
    }
}