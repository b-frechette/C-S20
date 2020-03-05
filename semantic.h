#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

void semantic(TreeNode *syntaxTree);

static void traverse(TreeNode *t, void (*preProcedure) (TreeNode *), void (*postProcedure) (TreeNode *));

static void nullProcedure(TreeNode *t);

static void insertNode(TreeNode *t);

#endif