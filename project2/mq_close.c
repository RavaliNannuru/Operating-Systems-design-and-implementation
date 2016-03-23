#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<lib.h>
#include"mqueue.h"
#include <signal.h>

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("Number of arguments for the sender process is less...\n");
		printf("Format:./sender <queuename>...\n");
		return -1;
	}
	char queue_name[30];
	strcpy(queue_name,argv[1]);
	message m1;
	m1.m1_p1 = queue_name;
	mq_close(m1);
	return 0;
}
