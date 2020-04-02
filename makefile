BIN  = parser
CC   = g++
CFLAGS = -DCPLUSPLUS -g  
TAR = frechette.tar

SRCS = parser.y parser.l
HDRS = scanType.h treeNode.h
OBJS = lex.yy.o parser.tab.o
LIBS = -lm 

parser: lex.yy.o parser.tab.o treeUtils.o printtree.o ourGetopt.o symbolTable.o semantic.o yyerror.o
	g++ -DCPLUSPLUS -g lex.yy.o parser.tab.o treeUtils.o printtree.o ourGetopt.o symbolTable.o semantic.o yyerror.o -lm -o c-

parser.tab.h parser.tab.c: parser.y treeUtils.h ourGetopt.h semantic.h yyerror.h
	bison -v -t -d parser.y  

lex.yy.c: parser.l parser.tab.h
	flex parser.l

test:
	./c-  tests/array.c-
	./c-  tests/assignSmall.c-
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
	./c-  tests/gcd.c-
	./c-  tests/gcd2.c-
	./c-  tests/if.c-
	./c-  tests/ifevil.c-
	./c-  tests/init.c-
	./c-  tests/initbad.c-
	./c-  tests/io.c-
	./c-  tests/loops.c-
	./c-  tests/loopscope.c-
	./c-  tests/loopscope2.c-
	./c-  tests/matchunmatch.c-
	./c-  tests/minus.c-
	./c-  tests/nano.c-
	./c-  tests/nested.c-
	./c-  tests/notused.c-
	./c-  tests/oparray.c-
	./c-  tests/optype.c-
	./c-  tests/param.c-
	./c-  tests/paramerr.c-
	./c-  tests/paramerr2.c-
	./c-  tests/precassoc.c-
	./c-  tests/redefined.c-
	./c-  tests/scope.c-
	./c-  tests/simple.c-
	./c-  tests/small.c-
	./c-  tests/strange.c-
	./c-  tests/syntaxerr-assign.c-
	./c-  tests/syntaxerr-badchar.c-
	./c-  tests/syntaxerr-call.c-
	./c-  tests/syntaxerr-constants.c-
	./c-  tests/syntaxerr-empty.c-
	./c-  tests/syntaxerr-hamlet.c-
	./c-  tests/syntaxerr-if.c-
	./c-  tests/syntaxerr-ifminus.c-
	./c-  tests/syntaxerr-logic.c-
	./c-  tests/syntaxerr-parens.c-
	./c-  tests/syntaxerr-parms.c-
	./c-  tests/syntaxerr-small.c-
	./c-  tests/syntaxerr-summul.c-
	./c-  tests/syntaxerr-type.c-
	./c-  tests/syntaxerr-typearray.c-
	./c-  tests/syntaxerr-typefun.c-
	./c-  tests/syntaxerr-unary.c-
	./c-  tests/syntaxerr-while.c-
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
	rm -f lex.yy.o parser.tab.o c- lex.yy.c parser.tab.h parser.tab.c parser.tar parser.output treeUtils.o ourGetopt.o semantic.o printtree.o symbolTable.o yyerror.o frechette.tar *~

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c symbolTable.h symbolTable.cpp semantic.h semantic.cpp printtree.h printtree.cpp yyerror.h yyerror.cpp makefile 
	ls -l frechette.tar