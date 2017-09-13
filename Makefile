all: dreadlock

parse.tab.cc parse.tab.hh:	parse.yy
	bison -d parse.yy

lex.yy.cc: lex.ll parse.tab.hh
	flex -o lex.yy.cc lex.ll

dreadlock: lex.yy.cc parse.tab.cc parse.tab.hh main.cc
	gcc -o dreadlock main.cc parse.tab.cc lex.yy.cc -lstdc++

clean:
	@rm -f dreadlock parse.tab.cc lex.yy.cc parse.tab.hh

