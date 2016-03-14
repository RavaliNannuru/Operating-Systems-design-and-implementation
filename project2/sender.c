#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<lib.h>
#include"mqueue.h"

int main(int argc, char *argv[])
{
	char queue_name[30];
	strcpy(queue_name,"firstqueue5");
	message m1,m2;
	m1.m1_p1 = queue_name;
	int queue_identifier = mq_open(m1);
	if(queue_identifier == -1) {
		printf("Queue Creation failed...\n");
	} else {
		char message[30];
		int r=0;
		int i=0,j;
		strcpy(message,argv[1]);
		for(j=2;j<argc;j++)
		{
			i=atoi(argv[j]);
			r |= (1<<i);
			m2.m1_i1 = (int)r;
			//m1_p1 contains queue_name
			m2.m1_p1 = queue_name;
			//m1_p2 contains message
			m2.m1_p2 = message;
			//send message
			mq_send(m2);
		}
	}
	return 0;
}
