#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mutators.h"
#include "utils.h"

// Table of mutators
char* (*mutatorTable[])(char* , size_t) = {
	mutate_bits,
	mutate_bytes,
	mutate_magic
};

#define NUM_MUTATORS sizeof(mutatorTable)/sizeof(void *)

// Table of magic substitutions
magic magicSubstitutions[] = {
	{1, 0xff},
	{1, 0x7f},
	{1, 0},
	{2, 0xffff},
	{2, 0},
	{4, 0xffffffff},
	{4, 0},
	{4, 0x80000000},
	{4, 0x80000000},
	{4, 0x7fffffff}
};

#define NUM_MAGIC sizeof(magicSubstitutions)/sizeof(void *)


char* mutate_bits(char *buf, size_t sz)
{
	size_t numBits = sz * 8;
	size_t count = (size_t)((double)numBits * 0.05);
	if (count == 0)
	{
		count = 1;
	}
	for (size_t i = 0; i < count; i++)
	{
		size_t bit = rand() % numBits;
		size_t bytePos = bit / 8;
		unsigned int bitPos = bit % 8;
		buf[bytePos] ^= (1 << bitPos);
	}
	return buf;
}


char* mutate_bytes(char *buf, size_t sz)
{
	size_t count = (size_t)((double)sz * 0.05);
	if (count == 0)
	{
		count = 1;
	}
	for (size_t i = 0; i < count; i++)
	{
		buf[rand() % sz] = (char)(rand() & 0xff);
	}
	return buf;
}


char* mutate_magic(char *buf, size_t sz)
{
	size_t count = (size_t)((double)sz * 0.05);
	if (count == 0)
	{
		count = 1;
	}
	for (size_t i = 0; i < count;)
	{
		unsigned int choice = rand() % NUM_MAGIC;
		unsigned int numBytes = magicSubstitutions[choice].numBytes;
		if (sz < numBytes)
		{
			continue;
		}
		size_t val = magicSubstitutions[choice].val;
		switch(numBytes)
		{
			case 1:
				p8(buf, rand() % sz, val);
				break;
			case 2:
				p16(buf, rand() % (sz - 1), val);
				break;
			case 4:
				p32(buf, rand() % (sz - 3), val);
				break;
		}
		i += numBytes;
	}
	return buf;
}


char* mutate(char *buf, size_t sz)
{
	char *mutated_buf = (char *)calloc(sz, 1);
	memcpy(mutated_buf, buf, sz);
	unsigned int choice = rand() % NUM_MUTATORS;
	// printf("Choosing mutator %u\n", choice);
	mutatorTable[choice](mutated_buf, sz);
	return mutated_buf;
}
