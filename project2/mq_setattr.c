#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<lib.h>
#include"mqueue.h"

int main(int argc, char *argv[])
{

	if(argc < 4) {
		printf("Number of arguments for the mq_setattr process is less...\n");
		printf("Format:./mq_setattr <queuename> <queue-size> <blocking(1)/nonblocking(0)>...\n");
		return -1;
	}

	//queue_name,queue_size,blocking/non-blocking
	char queue_name[30];
	strcpy(queue_name,argv[1]);
	message m1;
	m1.m1_p1 = queue_name;
	//queue_size
	m1.m1_i1 = atoi(argv[2]);
	//blocking-nonblocking
	m1.m1_i2 = atoi(argv[3]);
	mq_setattr(m1);
	return 0;
}
