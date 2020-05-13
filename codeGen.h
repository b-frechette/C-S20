#ifndef _CODE_GEN_H_
#define _CODE_GEN_H_

void codeGen(TreeNode *);

void ioSetup(TreeNode * s);

void traverse(TreeNode * s, int offset);

void getParams(TreeNode * s, int offset);

void globalInit(std::string sym, void* s);

void similarEmit(TreeNode *s, int offset);

#endif