BIN  = parser
CC   = g++
CFLAGS = -DCPLUSPLUS -g  
TAR = frechette.tar

SRCS = parser.y parser.l
HDRS = scanType.h treeNode.h
OBJS = lex.yy.o parser.tab.o
LIBS = -lm 

parser: lex.yy.o parser.tab.o treeUtils.o printtree.o ourGetopt.o symbolTable.o semantic.o
	g++ -DCPLUSPLUS -g lex.yy.o parser.tab.o treeUtils.o printtree.o ourGetopt.o symbolTable.o semantic.o -lm -o c-

parser.tab.h parser.tab.c: parser.y treeUtils.h ourGetopt.h semantic.h
	bison -v -t -d parser.y  

lex.yy.c: parser.l parser.tab.h
	flex parser.l

clean:
	rm -f lex.yy.o parser.tab.o c- lex.yy.c parser.tab.h parser.tab.c parser.tar parser.output treeUtils.o ourGetopt.o semantic.o printtree.o symbolTable.o frechette.tar *~

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c symbolTable.h symbolTable.cpp semantic.h semantic.c printtree.h printtree.c makefile 
	ls -l frechette.tar