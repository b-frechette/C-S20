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
	./c- -t tests/array.c-
	./c- -t tests/call.c-
	./c- -t tests/elsif.c-
	./c- -t tests/everythingS20.c-
	./c- -t tests/exp.c-
	./c- -t tests/if.c-
	./c- -t tests/init.c-
	./c- -t tests/loops.c-
	./c- -t tests/matchunmatch.c-
	./c- -t tests/precassoc.c-
	./c- -t tests/scope.c-
	./c- -t tests/simple.c-
	./c- -t tests/small.c-
	./c- -t tests/testExample.c-
	./c- -t tests/tiny.c-
	./c- -t tests/whileif.c-

print:
	./c- -p tests/array.c-

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNodes.h treeUtils.h treeUtils.c ourGetopt.h ourGetopt.c makefile 
	ls -l frechette.tar