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

test:
	./c- tests/arrays.c-
	./c- tests/basicAll4.c-
	./c- tests/call.c-
	./c- tests/callerr.c-
	./c- tests/defused.c-
	./c- tests/elsif.c-
	./c- tests/everythingS20.c-
	./c- tests/exp.c-
	./c- tests/exp2.c-
	./c- tests/exp3.c-
	./c- tests/init.c-
	./c- tests/loops.c-
	./c- tests/loopscope.c-
	./c- tests/minus.c-
	./c- tests/nano.c-
	./c- tests/notused.c-
	./c- tests/op.c-
	./c- tests/redefined.c-
	./c- tests/scope.c-
	./c- tests/small.c-
	./c- tests/strange.c-
	./c- tests/tiny.c-
	./c- tests/tinybad.c-
	./c- tests/tinygood.c-
	./c- tests/tinyscope.c-
	./c- tests/tinyType.c-
	./c- tests/types.c-
	./c- tests/undefined.c-
	./c- tests/uninitialized.c-
	./c- tests/whileif.c-

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c symbolTable.h symbolTable.cpp semantic.h semantic.cpp printtree.h printtree.c makefile 
	ls -l frechette.tar