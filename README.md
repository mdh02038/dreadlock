# Dreadlock

---
Deadlock analysis tool

Converts a system description into an [alloy](http://alloy.mit.edu/alloy/) file to test if it is deadlock free.

## To build:
---
./configure

make

make check

make install

## BNF
[BNF can be found here](bnf)

## Example
[Here is an example source file](tests/test1.dlk)

Running these lines in the test directory produces this:
```
../src/dreadlock -o regression/case1.als -t case1 test1.dlk
../src/dreadlock -o regression/case2.als -t case2 test1.dlk
../src/dreadlock -o regression/case3.als -t case3 test1.dlk
../src/dreadlock -o regression/case4.als -t case4 test1.dlk
../src/dreadlock -o regression/case5.als -t case5 test1.dlk
../src/dreadlock -o regression/case6.als -t case6 test1.dlk
```

#### [case1.als](tests/golden/case1.als)
#### [case2.als](tests/golden/case2.als)
#### [case3.als](tests/golden/case3.als)
#### [case4.als](tests/golden/case4.als)
#### [case5.als](tests/golden/case5.als)
#### [case6.als](tests/golden/case6.als)
