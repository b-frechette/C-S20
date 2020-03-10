#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern int numErrors, numWarnings;

void semantic(TreeNode *syntaxTree);

static void traverse(TreeNode *t, void (*preProcedure) (TreeNode *), void (*postProcedure) (TreeNode *));

static void nullProcedure(TreeNode *t);

ExpType insertNode(TreeNode *t);

void checkUse(std::string, void*);

//SymbolTable st;

#endif