CC=gcc


fuzzer: fuzzer.c utils.c mutators.c utils.h mutators.h
	$(CC) -o bin/fuzzer fuzzer.c utils.c mutators.c 

test: testcase.c
	$(CC) -o bin/testcase testcase.c -ggdb

clean:
	rm bin/fuzzer bin/testcase
