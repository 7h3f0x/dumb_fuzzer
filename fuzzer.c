#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
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

	int nullfd = open("/dev/null", O_WRONLY);

	// write(STDOUT_FILENO, inpBuf, inpSize);
	unsigned long long int cnt = 0;
	while (true)
	{
		cnt += 1;
		char* mutatedInput = mutate(inpBuf, inpSize);
		writeFile("test.sample", mutatedInput, inpSize);

		int fdout[2];
		pipe(fdout);

		pid_t child_pid = fork();

		if (child_pid == -1)
		{
			fputs("Failed to create child process, exiting...", stderr);
			exit(1);
		}
		if (child_pid == 0)
		{
			// child process
			dup2(fdout[1], STDOUT_FILENO);
			dup2(nullfd, STDERR_FILENO);
			close(fdout[0]);
			char *args[] = {"gdb", "-q", "--batch", "-x", "fuzzer.gdb", fuzzfname, NULL};
			execvp(args[0], args); // execvp searches PATH if filename has  no '/'
		}
		else
		{
			//parent process
			close(fdout[1]);
			wait(NULL);
			
			size_t output_cap = 256 ;
			char *outbuf = outbuf = (char *)calloc(output_cap, 1);
			size_t output_sz = 0, bytes_read = 0; 
			do {
				output_sz += bytes_read;
				if ((output_sz + 256) > output_cap)
				{
					output_cap += 256;
					outbuf = (char *)realloc(outbuf, output_cap);
				}
				bytes_read = read(fdout[0], (outbuf + output_sz), 256);
			}while(bytes_read != 0);

			if (strstr(outbuf, "Program received signal SIG"))
			{
				puts("Program Crashed");
				char crash_name[0x100];
				sprintf(crash_name, "crash.sample.%llu", cnt);
				writeFile(crash_name, mutatedInput, inpSize);
				sprintf(crash_name, "crash.sample.%llu.txt", cnt);
				writeFile(crash_name, outbuf, output_sz);
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
