#ifndef _UTIL_H_
#define _UTIL_H_

#include "treeNodes.h"
#include "scanType.h"

/* descr: Create a TreeNode for a Expression type node to store
* relevant tokenData.
* param: ExpKind -> Contains the nodekind for the tree node
* return: TreeNode -> Stores tokenData and matches $$
*/
TreeNode * newExpNode(ExpKind);

/* descr: Create a TreeNode for a Statement type node to store
* relevant tokenData.
* param: StmtKind -> Contains the nodekind for the tree node
* return: TreeNode -> Stores tokenData and matches $$
*/
TreeNode * newStmtNode(StmtKind);

/* descr: Create a TreeNode for a Declaration type node to store
* relevant tokenData.
* param: DeclKind -> Contains the nodekind for the tree node
* return: TreeNode -> Stores tokenData and matches $$
*/
TreeNode * newDeclNode(DeclKind);

#endif