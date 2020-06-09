#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"


void parse_arguements(int argc, char **argv, char **inputfname, char **fuzzfname)
{
	int option;
	option = getopt(argc, argv, ":i:f:");
	while(option != -1)
	{
		switch(option)
		{
			case 'i':
				*inputfname = strdup(optarg);
				break;
			case 'f':
				*fuzzfname = strdup(optarg);
				break;
			/*
			default:
				fprintf(stderr, "Unknown option %c\n", optopt);
				exit(-1);
			*/
		}
		option = getopt(argc, argv, ":i:f:");
	}
	if (*inputfname == NULL)
	{
		fputs("Please specify an inputfile", stderr);
		exit(-1);
	}
	if (*fuzzfname == NULL)
	{
		fputs("Please specify a file to fuzz", stderr);
		exit(-1);
	}
}

size_t readFile(char *fname, char **inpBuf)
{
	size_t size;
	FILE* inputfile = fopen(fname, "r");
	fseek(inputfile, 0, SEEK_END);
	size = ftell(inputfile);
	rewind(inputfile);
	*inpBuf = (char *)calloc(size, 1);
	fread(*inpBuf, 1, size, inputfile);
	fclose(inputfile);
	return size;
}

size_t writeFile(char *fname, char *buf, size_t size)
{
	FILE* outFile = fopen(fname, "w");
	size_t sz = fwrite(buf, 1, size, outFile);
	fclose(outFile);
	return sz;
}


void p8(char *buf, size_t idx, size_t val)
{
	*(unsigned char *)(buf + idx)= (unsigned char)(val & 0xff);
}

void p16(char *buf, size_t idx, size_t val)
{
	*(unsigned short *)(buf + idx) = (unsigned short)(val & 0xffff);
}

void p32(char *buf, size_t idx, size_t val)
{
	*(unsigned int *)(buf + idx) = (unsigned int)(val & 0xffffffff);
}

