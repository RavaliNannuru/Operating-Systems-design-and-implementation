#include "myshell.h"

/*
assuming that every command will be separated by A SINGLE SPACE
otherwise it will be an invalid thing.
it would make too hard and additional logic (which I find useless
and not the goal of this project) and consequently cause to waste
a lot of time.
 */
int main(void){

	char cmdline[MAX_LEN];
	char cmd[MAX_LEN], args[MAX_LEN];
	char* argv[MAX_LEN];

	//change input mode to raw
	//so that no \n is waited for.
	struct termios attr;
	tcgetattr(0, &attr);
	attr.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &attr);

	int i = 0, is_io=0;

	printf("Reading profile file...\n");
	putenv(get_var(".profile","TIMER"));
	timer=atoi(getenv("TIMER"));
	printf("Timer set to:%d\n\n",timer);
	path=get_var(".profile","PATH");
	//putenv(path);
	printf("PATH env variable set to:%s\n\n",path);
	home=get_var(".profile","HOME");
	//putenv(home);
	printf("HOME env variable set to:%s\n",home);

	while(1){
		//clear contents of the arrays for next input.
		memset(cmd,'\0',strlen(cmd));
		memset(args,'\0',strlen(args));
		memset(cmdline,'\0',strlen(cmdline));

		printf("%s",PROMPT);
		read_cmdline(cmdline);
		is_io=0;
		if (strstr(cmdline, "=>") != NULL) {
			is_io=1;
		}
		parse_cmdline2(cmdline,argv);
		parseArguments(argv,is_io);

		//check for a empty, if yes do nothing.
		if(argv[0]==NULL)
			;
		else{
			if(strcmp(argv[0],"exit") == 0)
				break;
		}
	}
	printf("Logout\n");
	return 0;
}

void child_sigquit_handler(int num)
{
	exit(0);
}
int exec_io_redirection(char **args) {

	int command_count = 0;
	int output_loc = -1;
	int status = 0;
	char *choice;

	for (char **temparg = args; *temparg; ++temparg) {
		printf("arg:%s\n",*temparg);
		if(strstr(*temparg, "=>") != NULL) {
			output_loc = command_count;
		}
		command_count++;
	}

	if (command_count >= 3) {
		pid_t pidp = getpid();
		pid1_io = fork();

		if (pid1_io == -1) {
			perror("Failed during fork");
			status = 1;
		} else if (pid1_io == 0) {
			printf("inside");
			childpid_io=getpid();
			char *res[output_loc];
			char **temparg = args;
			for(int i = 0; i < output_loc;i++) {
				int char_count=0;
				char *temp=*temparg;
				while (*temp != '\0') {
					char_count++;
					temp++;
				}
				printf("cc:%d\n",char_count);
				res[i] = malloc((char_count+1)*sizeof(char));
				strcpy(res[i],*temparg);

				if(i+1 == output_loc){
					temparg +=2;
				}
				else {
					++temparg;
				}
			}
			int char_count=0;
			char *temp=*temparg;
			while (*temp != '\0') {
				char_count++;
				temp++;
			}
			char output[char_count+1] ;
			strcpy(output,*temparg);
			for (int i=0;i< output_loc; i++) {
				printf("arg:%s\n",res[i]);
			}
			printf("output%sm\n",output);
			int fd1 = creat(output , 0644) ;
			printf("fd%d\n",fd1);
			dup2(fd1, STDOUT_FILENO);
			printf("b close");
			close(fd1);

			printf("start");
			int val = execvp(res[0], res);
			if (val==-1){
				perror("Failed during execution of commands");
			}
			exit(0);

		} else {
			pid2_io = fork();
			if (pid2_io == 0) {
				if (signal(SIGQUIT, child_sigquit_handler) == SIG_ERR) {
					printf("Signal received for internal error...");
					exit(1);
				}
				while(1){
					char buff[60];
					sleep(timer);
					char ch;
					printf("Enter ctrl+d to terminate, else program will continue:");
					ch = getchar();
					getchar();
					if(ch=='y'){
						kill(pidp, SIGCHLD);
						exit(0);
					}
				}
			}
			waitpid(childpid_io,0,0);
		}

		kill(pid2_io, SIGQUIT);
		waitpid(pid2_io,0,0);
		return 1;
	} else {
		printf("Error in number of inputs");
	}
	return status;
}

int execCommands(char **args)
{

	int status;
	clock_t start, end, total;
	char choice;
	pid_t pidp;
	pidp = getpid();
	pid1 = fork();
	if (pid1 == 0) {
		childpid=getpid();
		if (signal(SIGQUIT, child_sigquit_handler) == SIG_ERR) {
			printf("Signal received for internal error...");
			exit(1);
		}
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

			if (signal(SIGQUIT, child_sigquit_handler) == SIG_ERR) {
				printf("Signal received for internal error...");
				exit(1);
			}

			while(1){
				char buff[60];
				sleep(timer);
				char ch;
				printf("Enter ctrl+d to terminate, else program will continue:");
				ch = getchar();
				getchar();
				if(ch=='y'){
					kill(pidp, SIGCHLD);
					exit(0);
				}
			}
		}

		waitpid(childpid, 0,0);
	}
	kill(pid2, SIGQUIT);
	waitpid(pid2, 0, 0);
	return 1;

}
void sigchild_handler(int num)
{
	kill(pid1, SIGQUIT);
}


int parseArguments(char **args, int is_io)
{

	int status=0;
	if (strcmp(args[0],"exit")==0)
	{
		puts("shell is gone exit");
		exit(0);
	}
	/*else if (is_io == 1) {
		return exec_io_redirection(args);
	}*/
	else
	{
		return execCommands(args);
	}

	return status;

}

/*
reads everything, including the empty spaces.
commands are always [command] [modifiers] [modifiers] ...
so need to make a parser for this string and break it down
into pieces separated by spaces.
Had problems with the scanf("$[^\n]%c"cmdline);
going for a different approach, using getchar();
This will always be cmd args args...
make logic to insert them into 
 */
void read_cmdline(char* cmdline){

	int fid;
	char input;
	char mem;

	int length = 0;

	int last,temp = 0;
	int* aux;

	int found_tab = 0;

	/*
	still doing some reading on the best way to do this...
	I found this raw mode of operation and applied to this,
	worked fine except for this need for treating each case
	separately.
	 */
	while((input=getchar()) != '\n'){

		switch(input){
		case '\t':
			if(length > 0){

				found_tab = 1;

				char* aux = malloc(MAX_LEN);
				memset(aux,0,MAX_LEN);

				strncpy(aux,cmdline,strlen(cmdline));
				clear_line(cmdline);
				suggest(aux,&last);
				memset(cmdline,0,MAX_LEN);
				strcpy(cmdline,aux);
				free(aux);

				length = strlen(cmdline);
				printf("%s",cmdline);
			}
			else
				printf("\b\b  \b\b");
			break;

		case '\b':
			if(length > 0){
				length--;

				char* temp;
				temp = malloc(MAX_LEN);
				strncpy(temp,cmdline,length);
				memset(cmdline,0,strlen(cmdline));
				strncpy(cmdline,temp,length);
				free(temp);

				printf("\b\b\b   \b\b\b");
			}
			else
				printf("\b\b  \b\b");
			break;

		default:
			cmdline[length++] = input;
			cmdline[length] = '\0';
			//printf("\n%s\n",cmdline);
			break;
		}
	}
	//cmdline[length] = '\0';	//appends EOF char to cmdline instead of \n
	/*
	File manipulation to save the command history.
	 */
	if(length > 0){
		fid = open("/mnt/myshell/cmd_history.txt",O_RDWR);
		lseek(fid,0,SEEK_END);		//points to EOF
		write(fid,cmdline,strlen(cmdline));
		write(fid,"\n",1);	//insertion of blank line for next command; don't know if necessary
		close(fid);
	}

	return;
}

//not used but cool.
void parse_cmdline(char* cmdline, char* cmd, char* args){

	char curr[MAX_LEN];
	int index = 0;
	int first_entry = 0;

	while(index!=strlen(cmdline)+1){
		if( (cmdline[index] == ' ') && first_entry == 0){ //first cmdline arg: command
			strncpy(cmd,cmdline,index);
			strncpy(args,&cmdline[index+1],strlen(cmdline)-index-1);
			first_entry++;
		}
		index++;
	}
	//sloppy solution for simple commands, that don't have arguments, such as clear
	if(first_entry == 0){
		strcpy(cmd,cmdline);
	}

	return;
}

/*
Method to parse input line:
change ALL possible separators into terminators (\0)
and subsequently 
 */
void parse_cmdline2(char* cmdline, char** argv){

	int index = 0;

	//change spaces into terminators and make the assignments
	//for each of the argv pointers
	while(*cmdline != '\0'){
		while(*cmdline == ' ')
			*cmdline++ = '\0';	//increments pointer until points to space

		*argv++ = cmdline;

		while(*cmdline != ' ' && *cmdline != '\0')
			cmdline++;
	}
	//last one must be a \0 (NULL)
	*argv = '\0';

	return;
}

/*
makes the suggestion.
probably make it return an integer with the length of
the suggestion to adjust cursor position.
takes the closest match to the cmdline already entered and
substitutes it (hence the pointer).
 */
void suggest(char* line, int* last){

	int fid;
	char *list;
	char *temp;
	char **arr;

	int val = 0;
	val = *last;

	list = malloc(MAX_FILE);
	arr = calloc(MAX_LEN,MAX_LEN);
	temp = malloc(MAX_LEN);

	fid = open("cmd_history.txt",O_RDWR);
	lseek(fid,0,SEEK_SET);
	read(fid,list,MAX_FILE);
	close(fid);

	temp = strtok(list,"\n");
	int c=0;
	while(temp!='\0'){
		arr[c] = temp;
		c++;
		temp = strtok(NULL,"\n");
	}

	int found = FALSE;
	int i = 0;
	for(i = imp; i < c && found == FALSE; i++){
		//printf("arr[%d] = %s\n",i,arr[i]);
		if(strncmp(arr[i],line,strlen(line)) == 0){
			*last = i;
			found = TRUE;
			memset(line,0,MAX_LEN);
			strcpy(line,arr[i]);
			//line[i+1] = '\0';
			break;
		}
	}
	imp = i+1;	//points to next

	free(list);
	free(arr);
	free(temp);

	//printf("c: %d\n",c);
	//printf("imp: %d\n",imp);
	if(imp >= c - 1 || arr[imp] == NULL)
		imp = 0;

	return;
}

void clear_line(char* line){
	for(int i = 0; i<=strlen(line)+1; i++)
		printf("\b");
	for(int i = 0; i<=75; i++)
		printf(" ");
	for(int i = 0; i<=75; i++)
		printf("\b");	

	return;
}

char* get_var(char *file, char *key){
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


//sort
/*
	for(i = 0; i < c; i++){
		for(int j = 0; j < c - 1; j++){
			if(strcmp(arr[j],arr[j+1]) > 0){
				strcpy(temp1,arr[j],MAX_LEN);
				strcpy(arr[j],arr[j+1],MAX_LEN);
				strcpy(arr[j+1],temp1,MAX_LEN);
			}
		}
	}
 */
