/* 
 *	建立一个简单的守护进程
 *	实现守护进程：每隔1s在/tmp/dameon.log中写入一句话
 *	crazyleen <357228736@qq.com>
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#define MAXFILE 65535

volatile sig_atomic_t _running = 1;

void sigterm_handler(int arg)
{
	_running = 0;
}

int main()
{

	pid_t pc;
	int i,fd,len;
	char *buf="this is a Dameon\n";//每隔10s在/tmp/dameon.log中写入一句话
	len = strlen(buf);

	pc = fork(); //第一步，创建子进程，父进程退出，子进程成为init子进程
	if(pc<0){
		exit(EXIT_FAILURE);
	}
	else if(pc>0)
		exit(0);

	/*调用了fork函数时，子进程全盘拷贝了父进程的会话期、进程组、控制终端等，
	 *虽然父进程退出了，但会话期、进程组、控制终端等并没有改变，因此，还还不是
	 *真正意义上的独立开来，而setsid函数能够使进程完全独立出来，从而摆脱其他进程的控制
	 */
	setsid(); 	//第二步，摆脱原会话、原进程组、原终端的控制

	chdir("/"); //第三步，改变当前目录

	//子进程继承了父进程的文件权限掩码，给该子进程使用文件带来了诸多的麻烦
	//把文件权限掩码设置为0，增强该守护进程的灵活性
	umask(0); 	//第四步，设置文件权限掩码（屏蔽掉文件权限中的对应位）

	//同文件权限码一样，子进程从父进程继承已经打开的文件。这些被打开的文件可能永远不会被守护进程读写
	for(i=0;i<MAXFILE;i++) 	//第五步，关闭文件描述符
		close(i);

	//实现kill发出的signal信号处理
	signal(SIGTERM, sigterm_handler);


	while( _running ){ //写log
		if((fd=open("/tmp/dameon.log",O_CREAT|O_WRONLY|O_APPEND,0600))<0){
			perror("open");
			exit(1);
		}
		write(fd,buf,len);
		close(fd);
		sleep(1); //1s
	}

}


