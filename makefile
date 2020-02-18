BIN  = parser
CC   = g++
CFLAGS = -DCPLUSPLUS -g     # for use with C++ if file ext is .c
TAR = frechette.tar

SRCS = parser.y parser.l
HDRS = scanType.h treeNode.h
OBJS = lex.yy.o parser.tab.o
LIBS = -lm 

parser: lex.yy.o parser.tab.o treeUtils.o
	g++ -DCPLUSPLUS -g lex.yy.o parser.tab.o treeUtils.o -lm -o c-

parser.tab.o: parser.tab.c treeUtils.h

parser.tab.h parser.tab.c: parser.y
	bison -v -t -d parser.y  

lex.yy.c: parser.l parser.tab.h
	flex parser.l

clean:
	rm -f lex.yy.o parser.tab.o c- lex.yy.c parser.tab.h parser.tab.c parser.tar parser.output treeUtils.o frechette.tar *~

tar:
	tar -cvf frechette.tar parser.y parser.l scanType.h treeNode.h makefile 
	ls -l frechette.tar