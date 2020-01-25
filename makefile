build: 
		flex parser.l
		g++ lex.yy.c -o c-

clean:
		rm -f c-
		rm -f lex.yy.c