#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


char *prompt="Do you want to END the process?";

void ctrl_c_op(int signo)
{
	write(STDERR_FILENO,prompt,strlen(prompt));
	printf("\nctrl+c, exit...\n");
    exit(1);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler=ctrl_c_op;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGINT,&act,NULL);

	while(1);
}
