#ifndef _UTIL_H_
#define _UTIL_H_

#include "treeNodes.h"
#include "scanType.h"

TreeNode * newExpNode(ExpKind);

TreeNode * newStmtNode(StmtKind);

TreeNode * newDeclNode(DeclKind);

// static void printSpaces(void);

void printTree(TreeNode *, int, int);

#endif