#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern int numErrors, numWarnings;

/* descr: Semantic analysis of AST
*  param: TreeNode -> Pointer to the head treeNode of the AST
*  return: void
*/
void semantic(TreeNode *);

/* descr: Semantically analyze node from AST tree
*  param: TreeNode -> Pointer to the treeNode
*  return: ExpType
*/
ExpType insertNode(TreeNode *);

/* descr: Function to check if a variable has been use
*  param: std::string -> Name of the node being checked
*  param: void* 
*  return: void
*/
void checkUse(std::string, void*);

/* descr: Check parameters of a call against the function paramaters
*  param: TreeNode -> Original function node
*  param: TreeNode -> Call function node
*  param: TreeNode -> Function param node
*  param: TreeNode -> Call param node
*  param: int -> Parameter number
*  return: void
*/
void checkParams(TreeNode *, TreeNode *, TreeNode *, TreeNode *, int);

/* descr: Insert the IO functions into the symbol table
*  param: char -> Function name
*  param: ExpType -> Function return type
*  param: ExpType -> Parameter type 
*  return: void
*/
void ioSetup(const char*, ExpType, ExpType);

#endif