BIN  = parser
CC   = g++
CFLAGS = -DCPLUSPLUS -g     # for use with C++ if file ext is .c
TAR = frechette.tar

SRCS = $(BIN).y $(BIN).l
HDRS = scanType.h
OBJS = lex.yy.o $(BIN).tab.o
LIBS = -lm 

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o c-

$(BIN).tab.h $(BIN).tab.c: $(BIN).y
	bison -v -t -d $(BIN).y  

lex.yy.c: $(BIN).l $(BIN).tab.h
	flex $(BIN).l

all:    
	touch $(SRCS)
	make

clean:
	rm -f $(OBJS) c- lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output $(TAR) *~

tar:
	tar -cvf $(TAR) $(SRCS) $(HDRS) makefile 
	ls -l $(TAR)