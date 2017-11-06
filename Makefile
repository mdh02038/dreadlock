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
        cmodel.cc \
	cportconn.cc

parse.tab.cc parse.tab.hh:	parse.yy
	bison -v -d parse.yy

lex.yy.cc: lex.ll parse.tab.hh
	flex -o lex.yy.cc lex.ll

dreadlock: lex.yy.cc parse.tab.cc parse.tab.hh $(CFILES)
	gcc -o dreadlock $(CFILES) parse.tab.cc lex.yy.cc -lstdc++

cases:
	./dreadlock -o case1.als -t case1 testcases.dlk
	./dreadlock -o case2.als -t case2 testcases.dlk
	./dreadlock -o case3.als -t case3 testcases.dlk
	./dreadlock -o case4.als -t case4 testcases.dlk
	./dreadlock -o case5.als -t case5 testcases.dlk
	./dreadlock -o case6.als -t case6 testcases.dlk

clean:
	@rm -f dreadlock parse.tab.cc lex.yy.cc parse.tab.hh parse.output

