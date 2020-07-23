#include <stdio.h>

#define NUM_MAGIC 10
#define numMutators 3

typedef struct magic_struct
{
	unsigned int numBytes;
	size_t val;
} magic;


// Mutator which flips random bits
char* mutate_bits(char *buf, size_t sz);

// Mutator which changes random bytes to random values
char* mutate_bytes(char *buf, size_t sz);

// Mutator which does "magic" substitutions
char* mutate_magic(char *buf, size_t sz);

// Mutate the input to output of same size
char* mutate(char *buf, size_t sz);
