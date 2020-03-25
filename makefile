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

printTest:
	./c- -P tests/array.c-
	./c- -P tests/badcontrol.c-
	./c- -P tests/basicAll5.c-
	./c- -P tests/call.c-
	./c- -P tests/callerr.c-
	./c- -P tests/control.c-
	./c- -P tests/control2.c-
	./c- -P tests/decllists.c-
	./c- -P tests/defused.c-
	./c- -P tests/elsif.c-
	./c- -P tests/everythingS20.c-
	./c- -P tests/exp.c-
	./c- -P tests/exp2.c-
	./c- -P tests/exp3.c-
	./c- -P tests/if.c-
	./c- -P tests/ifevil.c-
	./c- -P tests/init.c-
	./c- -P tests/initbad.c-
	./c- -P tests/io.c-
	./c- -P tests/loops.c-
	./c- -P tests/loopscope.c-
	./c- -P tests/matchunmatch.c-
	./c- -P tests/minus.c-
	./c- -P tests/nano.c-
	./c- -P tests/notused.c-
	./c- -P tests/oparray.c-
	./c- -P tests/optype.c-
	./c- -P tests/precassoc.c-
	./c- -P tests/redefined.c-
	./c- -P tests/scope.c-
	./c- -P tests/simple.c-
	./c- -P tests/small.c-
	./c- -P tests/strange.c-
	./c- -P tests/testExample.c-
	./c- -P tests/tiny.c-
	./c- -P tests/tinybad.c-
	./c- -P tests/tinygood.c-
	./c- -P tests/tinyscope.c-
	./c- -P tests/tinyType.c-
	./c- -P tests/types.c-
	./c- -P tests/undefined.c-
	./c- -P tests/uninitialized.c-
	./c- -P tests/whileif.c-

test:
	./c-  tests/array.c-
	./c-  tests/badcontrol.c-
	./c-  tests/basicAll5.c-
	./c-  tests/call.c-
	./c-  tests/callerr.c-
	./c-  tests/control.c-
	./c-  tests/control2.c-
	./c-  tests/decllists.c-
	./c-  tests/defused.c-
	./c-  tests/elsif.c-
	./c-  tests/everythingS20.c-
	./c-  tests/exp.c-
	./c-  tests/exp2.c-
	./c-  tests/exp3.c-
	./c-  tests/if.c-
	./c-  tests/ifevil.c-
	./c-  tests/init.c-
	./c-  tests/initbad.c-
	./c-  tests/io.c-
	./c-  tests/loops.c-
	./c-  tests/loopscope.c-
	./c-  tests/matchunmatch.c-
	./c-  tests/minus.c-
	./c-  tests/nano.c-
	./c-  tests/notused.c-
	./c-  tests/oparray.c-
	./c-  tests/optype.c-
	./c-  tests/precassoc.c-
	./c-  tests/redefined.c-
	./c-  tests/scope.c-
	./c-  tests/simple.c-
	./c-  tests/small.c-
	./c-  tests/strange.c-
	./c-  tests/testExample.c-
	./c-  tests/tiny.c-
	./c-  tests/tinybad.c-
	./c-  tests/tinygood.c-
	./c-  tests/tinyscope.c-
	./c-  tests/tinyType.c-
	./c-  tests/types.c-
	./c-  tests/undefined.c-
	./c-  tests/uninitialized.c-
	./c-  tests/whileif.c-

clean:
	rm -f lex.yy.o parser.tab.o c- lex.yy.c parser.tab.h parser.tab.c parser.tar parser.output treeUtils.o ourGetopt.o semantic.o printtree.o symbolTable.o frechette.tar *~

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c symbolTable.h symbolTable.cpp semantic.h semantic.c printtree.h printtree.c makefile 
	ls -l frechette.tar