#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include "utils.h"


int main(int argc, char *argv[])
{
	char *inputfname = NULL, *fuzzfname = NULL;
	parse_arguements(argc, argv, &inputfname, &fuzzfname);
	printf("Inputfile: %s\n", inputfname);
	printf("File to fuzz: %s\n", fuzzfname);

	char *inpBuf = NULL;
	size_t inpSize = 0;
	// read the input file
	inpSize = readFile(inputfname, &inpBuf);

	// write(STDOUT_FILENO, inpBuf, inpSize);

	int fdin[2];
	int fdout[2];

	pipe(fdin);
	pipe(fdout);

	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		fputs("Failed to create child process, exiting...", stderr);
		exit(-1);
	}
	if (child_pid == 0)
	{
		// child process
		dup2(fdin[0], STDIN_FILENO);
		dup2(fdout[1], STDOUT_FILENO);
		close(fdin[1]);
		close(fdout[0]);
		char *args[] = {fuzzfname, NULL};
		execv(args[0], args);
	}
	else
	{
		//parent process
		close(fdin[0]);
		close(fdout[1]);
		write(fdin[1], inpBuf, inpSize);
		close(fdin[1]);
		wait(NULL);
		int output_cap = 256 ;
		char *outbuf = outbuf = (char *)calloc(output_cap, 1);
		int output_sz = 0, bytes_read = 0; 
		do {
			output_sz += bytes_read;
			if ((output_sz + 256) > output_cap)
			{
				output_cap += 256;
				outbuf = (char *)realloc(outbuf, output_cap);
			}
			bytes_read = read(fdout[0], (outbuf + output_sz), 256);
		}while(bytes_read != 0);

		// Write the output read to screen
		write(STDOUT_FILENO, outbuf, output_sz);
	}
	return 0;
}
