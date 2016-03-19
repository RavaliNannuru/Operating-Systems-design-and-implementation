#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<lib.h>
#include"mqueue.h"

int main(int argc, char *argv[])
{
	if(argc < 3) {
		printf("Number of arguments for receiver process is less...\n");
		printf("Format:./receiver <queuename> <list of receivers>...\n");
		return -1;
	}

	char queue_name[30];
	strcpy(queue_name,argv[1]);
	char p[30]="";
	int j=0,r,err;
	int i= 0 ;

	for(j=2;j<argc;j++)
	{
		r = atoi(argv[j]);
		i |= (1<<r);
		message m1;
		m1.m1_i1 = (int)i;
		m1.m1_p1 = p;
		m1.m1_p2 = queue_name;
		err = mq_receive(m1);
		if(err == -2)
			exit(1);
		else if(err == 1)
			printf("Got new message: %s \n",p);
		strcpy(p,"");
	}

	return 0;
}
