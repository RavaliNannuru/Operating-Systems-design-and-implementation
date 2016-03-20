#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<lib.h>
#include"mqueue.h"

int main(int argc, char *argv[])
{

	if(argc < 2) {
		printf("Number of arguments for the mq_getattr process is less...\n");
		printf("Format:./mq_getattr <queuename>...\n");
		return -1;
	}
	char queue_name[30];
	strcpy(queue_name,argv[1]);
	message m1;
	m1.m1_p1 = queue_name;
	mq_getattr(m1);
	return 0;
}
