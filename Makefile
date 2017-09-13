all: calc

parse.tab.c parse.tab.h:	parse.y
	bison -d parse.y

lex.yy.c: lex.l parse.tab.h
	flex lex.l

calc: lex.yy.c parse.tab.c parse.tab.h main.cc
	gcc -o calc main.cc parse.tab.c lex.yy.c

clean:
	rm calc parse.tab.c lex.yy.c parse.tab.h

