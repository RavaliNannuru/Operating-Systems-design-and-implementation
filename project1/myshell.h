#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <curses.h>

/*
Definition of the standard output and input
for the main routine.
*/
#define STDIN	0
#define STDOUT	1
#define PROMPT	"$ "
#define PATH
#define HOME
#define SIGN

/*
Max number of entries per line, probably a little
too much, since now I found out some will be pointers
to pointers (argv), but still the maximum length of
a command line input is unknown.
Once everything is done I'll try reducing this by 
factors of 2 till the minimum.
*/
#define MAX_LEN		256
#define MAX_FILE 	1048576

/*
Definition of the data structures to be used
by the main routine.
Needs a entry structure in which to store the
users input characters.
*/
struct args{
	char*	args;
	char*	next;
	};
	
struct env_var{
	char*	name;
	char*	value;
	};
	
int imp = 0;
int timer = 2;
char * path,*home;
pid_t pid1,childpid,pid2;
pid_t pid1_io,childpid_io,pid2_io;

/*
Headers for functions used in main.c for the
shell program
*/
void read_cmdline(char* cmdline);
void parse_cmdline(char* cmdline, char* cmd, char* args);
void parse_cmdline2(char* cmdline, char** argv);
void suggest(char* line, int* last);
void clear_line(char* line);	
char* get_var(char *file, char *key);
int parseArguments(char **args, int is_io);
int execCommands(char **);
int exec_io_redirection(char **);
void sigchild_handler(int);
void chlid_sigquit_handler(int num);








	
