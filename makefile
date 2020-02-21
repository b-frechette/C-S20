BIN  = parser
CC   = g++
CFLAGS = -DCPLUSPLUS -g     # for use with C++ if file ext is .c
TAR = frechette.tar

SRCS = parser.y parser.l
HDRS = scanType.h treeNode.h
OBJS = lex.yy.o parser.tab.o
LIBS = -lm 

parser: lex.yy.o parser.tab.o treeUtils.o ourGetopt.o
	g++ -DCPLUSPLUS -g lex.yy.o parser.tab.o treeUtils.o ourGetopt.o -lm -o c-

parser.tab.h parser.tab.c: parser.y treeUtils.h ourGetopt.h
	bison -v -t -d parser.y  

lex.yy.c: parser.l parser.tab.h
	flex parser.l

clean:
	rm -f lex.yy.o parser.tab.o c- lex.yy.c parser.tab.h parser.tab.c parser.tar parser.output treeUtils.o ourGetopt.o frechette.tar *~

test:
	./c- tests/array.c-
	./c- tests/call.c-
	./c- tests/elsif.c-
	./c- tests/everythingS20.c-
	./c- tests/exp.c-
	./c- tests/if.c-
	./c- tests/init.c-
	./c- tests/loops.c-
	./c- tests/matchunmatch.c-
	./c- tests/precassoc.c-
	./c- tests/scope.c-
	./c- tests/simple.c-
	./c- tests/small.c-
	./c- tests/testExample.c-
	./c- tests/tiny.c-
	./c- tests/whileif.c-

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c makefile 
	ls -l frechette.tar