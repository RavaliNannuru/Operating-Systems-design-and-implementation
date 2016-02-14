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
int timer = 2;
char * path,*home;
int execCommands(char **);
int exec_io_redirection(char **);
void SIGCHLD_handler(int);

char* get_var(char *file, char *key) {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char * token;
	char *search = "=";

	fp = fopen(file, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		if(strstr(line, key) != NULL){
			//token = strtok(line, search);
			//token = strtok(NULL, search);
			token = line;
			break;
		}
	}

	fclose(fp);
	if (line)
		free(line);
	char *pos;
	if ((pos=strchr(token, '\n')) != NULL)
		*pos = '\0';
	return token;
}

char **splitInputCommands(char *my_string)
{
	int buffsize = 50,i = 0;
	char *command;
	char **commands = malloc(buffsize * sizeof(char*));

	if (!commands) {
		fprintf(stderr, "Memory allocation failed, relaunch the program");
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
				fprintf(stderr, "Memory allocation failed, relaunch the program");
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
					printf("Signal received for internal error...");
					exit(1);
				}
				while(1){
					char buff[60];
					sleep(timer);
					printf("Enter ctrl+d to terminate, else program will continue:");
					if (fgets(buff,60,stdin) != NULL) {
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
	pid_t pidp, pid2;
	pidp = getpid();
	pid1 = fork();
	if (pid1 == 0) {
		if (execvp(args[0], args) == -1) {
			perror("command execution failed...");
		}
		exit(0);
	} else if (pid1 < 0) {

		perror("command execution failed...");

	} else {
		pid2 = fork();
		if (pid2 == 0) {
			pid2 =getpid();
			char ch;

			if (signal(SIGQUIT, chld_SIGQUIT_handler) == SIG_ERR) {
				printf("Signal received for internal error...");
				exit(1);
			}

			while(1){
				char buff[60];
				sleep(timer);
				printf("Enter ctrl+d to terminate, else program will continue:");
				if (fgets(buff,60,stdin) != NULL) {
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
	char ch;
	int contains_io =0;

	printf("Reading profile file...\n");
	putenv(get_var(".profile","TIMER"));
	timer=atoi(getenv("TIMER"));
	printf("Timer set to:%d\n",timer);
	path=get_var(".profile","PATH");
	putenv(path);
	printf("PATH env variable set to:%s\n",path);
	home=get_var(".profile","HOME");
	putenv(home);
	printf("HOME env variable set to:%s\n",home);

	if (signal(SIGCHLD, SIGCHLD_handler) == SIG_ERR) {
		printf("Signal received for internal error...");
		exit(1);
	}
	// allocates memory for the command input
	my_string = (char *) malloc (nbytes + 1);
	while(1)
	{
		//input prompt
		printf("->");
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
