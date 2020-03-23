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

test:
	./c- -P tests/array.c-
	./c- -P tests/basicAll4.c-
	./c- -P tests/callerr.c-
	./c- -P tests/decllists.c-
	./c- -P tests/elsif.c-
	./c- -P tests/everythingS20.c-
	./c- -P tests/exp.c-
	./c- -P tests/exp2.c-
	./c- -P tests/exp3.c-
	./c- -P tests/if.c-
	./c- -P tests/ifevil.c-
	./c- -P tests/init.c-
	./c- -P tests/loops.c-
	./c- -P tests/loopscope.c-
	./c- -P tests/matchunmatch.c-
	./c- -P tests/minus.c-
	./c- -P tests/nano.c-
	./c- -P tests/oparray.c-
	./c- -P tests/optype.c-
	./c- -P tests/precassoc.c-
	./c- -P tests/redefined.c-
	./c- -P tests/simple.c-
	./c- -P tests/small.c-
	./c- -P tests/strange.c-
	./c- -P tests/testExample.c-
	./c- -P tests/tiny.c-
	./c- -P tests/tinybad.c-
	./c- -P tests/tinygood.c-
	./c- -P tests/tinyscope.c-
	./c- -P tests/types.c-
	./c- -P tests/undefined.c-
	./c- -P tests/uninitialized.c-
	./c- -P tests/whileif.c-

clean:
	rm -f lex.yy.o parser.tab.o c- lex.yy.c parser.tab.h parser.tab.c parser.tar parser.output treeUtils.o ourGetopt.o semantic.o printtree.o symbolTable.o frechette.tar *~

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c symbolTable.h symbolTable.cpp semantic.h semantic.c printtree.h printtree.c makefile 
	ls -l frechette.tar