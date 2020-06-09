#include <stdio.h>


// To parse command line arguements
void parse_arguements(int argc, char **argv, char **inputfname, char **fuzzfname);

// To read from a file into *inpBuf
size_t readFile(char *fname, char **inpBuf);

// To write to a file from buf
size_t writeFile(char *fname, char *buf, size_t size);

// Pack val to 8 bit char at idx in buf
void p8(char *buf, size_t idx, size_t val);

// Pack val to 2 8-bit chars, considering system endianness, at idx in buf
void p16(char *buf, size_t idx, size_t val);

// Pack val to 4 8-bit chars, considering system endianness, at idx in buf
void p32(char *buf, size_t idx, size_t val);
