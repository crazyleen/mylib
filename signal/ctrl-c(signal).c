/* 
 * typedef void sigfunc(int);
 * sigfunc *signal(int, sigfunc *);
 * #kill -usr1 pid
 * crazyleen <357228736@qq.com>
 */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */

void ctrl_c(int signo)
{
	printf("\nsigno:%d, ctrl+c, exit...\n", signo);
    _exit(1);

}

void user_signal(int signo)
{
	if (signo == SIGUSR1)
		printf("sigusr1:%d, catch sigusr1!\n", signo);
	else 	if (signo == SIGUSR2)
		printf("sigusr2:%d, catch sigusr2!\n", signo);
	else
		printf("unknow signal:%d\n", signo);
}

int main(char argc, char *argv[])
{
	
	//typedef void sigfunc(int);
	//sigfunc *signal(int, sigfunc *);
	if (signal(SIGINT, ctrl_c) == SIG_ERR){//ctrl^c
		printf("can't catch SIGINT\n");
	}

	//use defined sig
	if (signal(SIGUSR1, user_signal) == SIG_ERR){
		printf("can't catch SIGUSR1\n");
	}

	if (signal(SIGUSR2, user_signal) == SIG_ERR){
		printf("can't catch SIGUSR2\n");
	}

	printf("UNIX\n");
	pause();
}


