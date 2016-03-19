#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<lib.h>
#include"mqueue.h"

int main(int argc, char *argv[])
{

	if(argc < 3) {
		printf("Number of arguments for the notification process is less...\n");
		printf("Format:./req_notifi <queuename> <reciever_pid>...\n");
		return -1;
	}
	char queue_name[30];
	strcpy(queue_name,argv[1]);
	message m1,m2;
	m1.m1_p1 = queue_name;
	int r=0;
	int i=0,j;
	for(j=2;j<argc;j++)
	{
		i=atoi(argv[j]);
		m1.m1_i1 = (int)i;
		//send req_notification
		mq_reqnotify(m1);
	}
	return 0;
}
