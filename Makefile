all: dreadlock

CFILES = \
	main.cc \
	cobstack.cc \
        message.cc \
	csymbol.cc \
	cdecl.cc \
	cvc.cc \
	cbus.cc \
	cbustype.cc \
	cmodule.cc \
	cinstance.cc \

parse.tab.cc parse.tab.hh:	parse.yy
	bison -v -d parse.yy

lex.yy.cc: lex.ll parse.tab.hh
	flex -o lex.yy.cc lex.ll

dreadlock: lex.yy.cc parse.tab.cc parse.tab.hh $(CFILES)
	gcc -o dreadlock $(CFILES) parse.tab.cc lex.yy.cc -lstdc++

clean:
	@rm -f dreadlock parse.tab.cc lex.yy.cc parse.tab.hh parse.output

