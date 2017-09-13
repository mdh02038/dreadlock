all: dreadlock

parse.tab.c parse.tab.h:	parse.y
	bison -d parse.y

lex.yy.c: lex.l parse.tab.h
	flex lex.l

dreadlock: lex.yy.c parse.tab.c parse.tab.h main.cc
	gcc -o dreadlock main.cc parse.tab.c lex.yy.c -lstdc++

clean:
	rm dreadlock parse.tab.c lex.yy.c parse.tab.h

