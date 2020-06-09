#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include "utils.h"
#include "mutators.h"


int main(int argc, char *argv[])
{
	char *inputfname = NULL, *fuzzfname = NULL;
	parse_arguements(argc, argv, &inputfname, &fuzzfname);
	printf("Inputfile: %s\n", inputfname);
	printf("File to fuzz: %s\n", fuzzfname);

	srand(time(NULL));

	char *inpBuf = NULL;
	size_t inpSize = 0;
	// read the input file
	inpSize = readFile(inputfname, &inpBuf);

	// write(STDOUT_FILENO, inpBuf, inpSize);
	unsigned long long int cnt = 0;
	while (true)
	{
		cnt += 1;
		char* mutatedInput = mutate(inpBuf, inpSize);
		writeFile("test.sample", mutatedInput, inpSize);

		pid_t child_pid = fork();

		if (child_pid == -1)
		{
			fputs("Failed to create child process, exiting...", stderr);
			exit(-1);
		}
		if (child_pid == 0)
		{
			// child process
			char *args[] = {fuzzfname, "test.sample", NULL};
			execv(args[0], args);
		}
		else
		{
			//parent process
			int status = 0;
			wait(&status);
			if (!WIFEXITED(status))
			{
				puts("Program Crashed");
				char crash_name[0x100];
				sprintf(crash_name, "crash.sample.%llu", cnt);
				writeFile(crash_name, mutatedInput, inpSize);
			}
			else
			{
				puts("no crash");
			}
			free(mutatedInput);
		}
	}
	return 0;
}
