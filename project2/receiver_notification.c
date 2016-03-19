#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
	if (signo == SIGCONT)
		printf("Received notification for new message...\n");
}

int main(void)
{
	if (signal(SIGCONT, sig_handler) == SIG_ERR)
		printf("\ncan't catch new signal\n");
	// A long long wait so that we can easily issue a signal to this process
	while(1)
		sleep(1);
	return 0;
}
