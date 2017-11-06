check-local:
	mkdir -p regression
	../src/dreadlock -o regression/case1.als -t case1 test1.dlk
	../src/dreadlock -o regression/case2.als -t case2 test1.dlk
	../src/dreadlock -o regression/case3.als -t case3 test1.dlk
	../src/dreadlock -o regression/case4.als -t case4 test1.dlk
	../src/dreadlock -o regression/case5.als -t case5 test1.dlk
	../src/dreadlock -o regression/case6.als -t case6 test1.dlk
	diff golden/ regression/

