#include "emitcode.h"
#include "treeNodes.h"
#include "symbolTable.h"
#include "codeGen.h"

extern SymbolTable st;
extern int Goffset;

void ioSetup()
{
    //input
    emitComment((char * )"FUNCTION", (char *)"input");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab int input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"input");

    //output
    emitComment((char * )"FUNCTION", (char *)"output");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
    emitRO((char*)"OUT", 3, 3, 3, (char*)"Output integer");
    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"output");

    //inputb
    emitComment((char * )"FUNCTION", (char *)"inputb");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab bool input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"inputb");

    //outputb
    emitComment((char * )"FUNCTION", (char *)"outpub");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
    emitRO((char*)"OUT", 3, 3, 3, (char*)"Output bool");
    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outputb");  

    //inputc
    emitComment((char * )"FUNCTION", (char *)"inputc");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"IN", 2, 2, 2, (char *)"Grab char input");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"inputc");

    //outputc
    emitComment((char * )"FUNCTION", (char *)"outpuc");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
    emitRO((char*)"OUT", 3, 3, 3, (char*)"Output char");
    emitRM((char *)"LDC", 2, 0, 6, (char *)"Set return to 0");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outputc");  

    //outnl
    emitComment((char * )"FUNCTION", (char *)"outnl");
    emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
    emitRO((char *)"OUTNL", 3, 3, 3, (char *)"Output a newline");
    emitRM((char *)"LD", 3, -1, 1, (char *)"Load return address");
    emitRM((char *)"LD", 1, 0, 1, (char *)"Adjust fp");
    emitRM((char *)"LDA", 7, 0, 3, (char *)"Return");
    emitComment((char *)"END FUNCITON", (char *)"outnl");  
}

void traverse(TreeNode *s)
{
    if(s != NULL)
    {
        if(s->nodekind == DeclK)
        {
            switch(s->kind.decl)
            {
                case FuncK:
                    emitComment((char * )"FUNCTION", (char *)s->attr.name);
                    s->offset = emitSkip(0);
                    
                    emitComment((char *)"END FUNCITON", (char *)s->attr.name);
                    break;
                default:
                    break;
            }
        }
    }
}

void codeGen(TreeNode * s)
{
    emitComment((char *)"TM Code");
    emitSkip(1);
    ioSetup();
    traverse(s);
}