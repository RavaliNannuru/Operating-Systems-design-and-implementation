#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <errno.h>

pid_t pid1;

int run_exec(char **);

char **split_cmd_line(char *line)
{
	int buffsize = 50, pos = 0;
	char *token;
	char **tokenpos = malloc(buffsize * sizeof(char*));

	if (!tokenpos) {
		fprintf(stderr, "sh: no memory to allocate, re-run ur shell\n");
		exit(0);
	}

	token = strtok(line, " \a\r\n\t");
	while (token != NULL) {
		tokenpos[pos] = token;
		pos++;

		if (pos >= buffsize) {
			buffsize = buffsize + 50;
			tokenpos = realloc(tokenpos, buffsize * sizeof(char*));
			if (!tokenpos) {
				fprintf(stderr, "sh: no memory to allocate, re-run ur shell\n");
				exit(0);
			}
		}

		token = strtok(NULL, " \a\r\n\t");
	}
	tokenpos[pos] = NULL;
	return tokenpos;
}

void chld_SIGQUIT_handler(int num)
{
	exit(0);
}

int shell_exit()
{
	exit(0);
}
int parse_args(char **args)
{

	int status=0;

	if (strcmp(args[0],"exit")==0)
	{
		puts("shell is gone exit");
		status = shell_exit();
	}
	else
	{
		return run_exec(args);
	}

	return status;

}

int run_exec(char **args)
{

	int status;
	clock_t start, end, total;
	char c;
	pid_t pidp, pid2, wpid;
	pidp = getpid();
	pid1 = fork();
	if (pid1 == 0) {
		if (execvp(args[0], args) == -1) {
			perror("sh: couldnot exec, try re-run");
		}
		exit(0);
	} else if (pid1 < 0) {

		perror("sh: couldnot fork, try re-run");
	} else {
		pid2 = fork();
		if (pid2 == 0) {
			if (signal(SIGQUIT, chld_SIGQUIT_handler) == SIG_ERR) {
				printf("SIGINT install error\n");
				exit(1);
			}

			while(1){
				sleep(5);
				printf("Do you want to terminate the current command[Y/N]:");
				c=getchar();
				getchar();
				fflush(stdin);
				if (c == 'Y' || c == 'y') {
					kill(pidp, SIGCHLD);
					exit(0);
				}
			}
		}
		do{
			wpid = waitpid(pid1, &status, WUNTRACED);

		}while(!WIFEXITED(status) && !WIFSIGNALED(status));

	}
	kill(pid2, SIGQUIT);
	do{
		wpid = waitpid(pid2, &status, WUNTRACED);

	}while(!WIFEXITED(status) && !WIFSIGNALED(status));

	return 1;

}
void SIGCHLD_handler(int num)
{
	kill(pid1, SIGQUIT);
}

int main( void ) {
	int nbytes = 100,bytes_read;
	char *my_string;
	char **args;
	int status;

	if (signal(SIGCHLD, SIGCHLD_handler) == SIG_ERR) {
		printf("SIGINT install error\n");
		exit(1);
	}
	my_string = (char *) malloc (nbytes + 1);
	while(1)
	{
		printf("--");
		bytes_read = getline (&my_string, &nbytes, stdin);

		if (bytes_read == -1)
		{
			puts ("Error while reading command line");
		}
		else
		{
			puts ("Your commands...");
			puts (my_string);
		}
		args = split_cmd_line(my_string);
		status = parse_args(args);
	}
	free(my_string);
	return 0;
}
