/* 
 *	����һ���򵥵��ػ�����
 *	ʵ���ػ����̣�ÿ��1s��/tmp/dameon.log��д��һ�仰
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
	char *buf="this is a Dameon\n";//ÿ��10s��/tmp/dameon.log��д��һ�仰
	len = strlen(buf);

	pc = fork(); //��һ���������ӽ��̣��������˳����ӽ��̳�Ϊinit�ӽ���
	if(pc<0){
		exit(EXIT_FAILURE);
	}
	else if(pc>0)
		exit(0);

	/*������fork����ʱ���ӽ���ȫ�̿����˸����̵ĻỰ�ڡ������顢�����ն˵ȣ�
	 *��Ȼ�������˳��ˣ����Ự�ڡ������顢�����ն˵Ȳ�û�иı䣬��ˣ���������
	 *���������ϵĶ�����������setsid�����ܹ�ʹ������ȫ�����������Ӷ������������̵Ŀ���
	 */
	setsid(); 	//�ڶ���������ԭ�Ự��ԭ�����顢ԭ�ն˵Ŀ���

	chdir("/"); //���������ı䵱ǰĿ¼

	//�ӽ��̼̳��˸����̵��ļ�Ȩ�����룬�����ӽ���ʹ���ļ������������鷳
	//���ļ�Ȩ����������Ϊ0����ǿ���ػ����̵������
	umask(0); 	//���Ĳ��������ļ�Ȩ�����루���ε��ļ�Ȩ���еĶ�Ӧλ��

	//ͬ�ļ�Ȩ����һ�����ӽ��̴Ӹ����̼̳��Ѿ��򿪵��ļ�����Щ���򿪵��ļ�������Զ���ᱻ�ػ����̶�д
	for(i=0;i<MAXFILE;i++) 	//���岽���ر��ļ�������
		close(i);

	//ʵ��kill������signal�źŴ���
	signal(SIGTERM, sigterm_handler);


	while( _running ){ //дlog
		if((fd=open("/tmp/dameon.log",O_CREAT|O_WRONLY|O_APPEND,0600))<0){
			perror("open");
			exit(1);
		}
		write(fd,buf,len);
		close(fd);
		sleep(1); //1s
	}

}


