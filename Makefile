fuzzer: fuzzer.c utils.c mutators.c utils.h mutators.h
	gcc -o bin/fuzzer fuzzer.c utils.c mutators.c 

test: testcase.c
	gcc -o bin/testcase testcase.c
