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

int execCommands(char **);
int exec_io_redirection(char **);
void SIGCHLD_handler(int);

char **splitInputCommands(char *my_string)
{
	int buffsize = 50,i = 0;
	char *command;
	char **commands = malloc(buffsize * sizeof(char*));

	if (!commands) {
		fprintf(stderr, "Memory allocation failed, relaunch the program\n");
		exit(0);
	}

	command = strtok(my_string, " \a\r\n\t");
	while (command != NULL) {
		commands[i] = command;
		i++;

		if (i >= buffsize) {
			buffsize = buffsize + 50;
			commands = realloc(commands, buffsize * sizeof(char*));
			if (!commands) {
				fprintf(stderr, "Memory allocation failed, relaunch the program\n");
				exit(0);
			}
		}

		command = strtok(NULL, " \a\r\n\t");
	}
	commands[i] = NULL;
	return commands;
}

void chld_SIGQUIT_handler(int num)
{
	exit(0);
}
int exec_io_redirection(char **args) {

	int command_count = 0;
	int output_loc = -1;
	int status = 0;
	pid_t wpid;
	char **res;
	char *choice;

	for (char **temparg = args; *temparg; ++temparg) {
		if(strstr(*temparg, "=>") != NULL) {
			output_loc = command_count;
		}
		command_count++;
	}

	if (command_count >= 3) {
		pid_t pidp = getpid(),pid2;
		pid1 = fork();
		if (pid1 == -1) {
			perror("Failed during fork");
			status = 1;
		} else if (pid1 == 0) {


			res = malloc((output_loc) * sizeof(char*));
			for(int i = 0; i < output_loc;i++) {
				res[i] = args[i];
			}
			char *output = args[output_loc + 1];

			int fd1 = creat(output , 0644) ;
			dup2(fd1, STDOUT_FILENO);
			close(fd1);

			if (execvp(res[0], res)==-1){
				perror("Failed during execution of commands");
			}
			exit(0);

		} else {
			pid2 = fork();
			if (pid2 == 0) {
				if (signal(SIGQUIT, chld_SIGQUIT_handler) == SIG_ERR) {
					printf("SIGINT install error\n");
					exit(1);
				}
				while(1){
					sleep(5);
					printf("please enter [y]es, [n]o to terminate the program:");
					scanf("%c", &choice);
					//printf("out:%c",choice);
					if (choice == 'y') {
						puts("inside");
						kill(pidp, SIGCHLD);
						exit(0);
					}
				}
			}
			waitpid(pid1,0,0);
		}

		kill(pid2, SIGQUIT);
		waitpid(pid2,0,0);
		free(res);
		return 1;
	} else {
		printf("Error in number of inputs");
	}
	return status;
}

int parseArguments(char **args, int is_io)
{

	int status=0;

	if (strcmp(args[0],"exit")==0)
	{
		puts("shell is gone exit");
		exit(0);
	}
	else if (is_io == 1) {
		return exec_io_redirection(args);
	}
	else
	{
		return execCommands(args);
	}

	return status;

}

int execCommands(char **args)
{

	int status;
	clock_t start, end, total;
	char choice;
	pid_t pidp, pid2, wpid;
	pidp = getpid();
	if (signal(SIGCHLD, SIGCHLD_handler) == SIG_ERR) {
		printf("SIGINT install error\n");
		exit(1);
	}
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
				printf("please enter [y]es, [n]o to terminate the program:");
				choice=getchar();
				printf("yo",choice);

				//putchar(choice);
				if (choice == 'y') {
					puts("inside");
					kill(pidp, SIGCHLD);
					exit(0);
				}
			}
		}
		waitpid(pid1, 0,0);

	}
	kill(pid2, SIGQUIT);
	waitpid(pid2, 0, 0);
	return 1;

}
void SIGCHLD_handler(int num)
{
	kill(pid1, SIGQUIT);
}

int main( void ) {
	// number of bytes to read
	int nbytes = 100,bytes_read;
	// input command will be stored in my_string
	char *my_string;
	// after parsing of commands, o/p is stored in args
	char **arguments;
	int contains_io =0;

	if (signal(SIGCHLD, SIGCHLD_handler) == SIG_ERR) {
		printf("SIGINT install error\n");
		exit(1);
	}
	// allocates memory for the command input
	my_string = (char *) malloc (nbytes + 1);
	while(1)
	{
		//input prompt
		printf("\n->");
		bytes_read = getline (&my_string, &nbytes, stdin);

		if (bytes_read == -1)
		{
			puts ("Error while reading command line");
		}
		else
		{
			puts ("Entered Commands...");
			puts (my_string);
		}
		contains_io=0;
		if (strstr(my_string, "=>") != NULL) {
			contains_io=1;
		}

		arguments = splitInputCommands(my_string);

		parseArguments(arguments,contains_io);
	}
	//free the allocated memory
	free(my_string);
	free(arguments);
	return 0;
}
