#include <syslog.h>

int main(char argc, char *argv[])
{
	char str[] = "this is a log";

	//本程序打开日记，输出PID， 用户进程产生的信息
	//openlog(argv[0], LOG_PID, LOG_USER);

	//输出日记到系统日记文件/var/log/messages
	syslog(LOG_INFO, "log info: %s\n", str);
	syslog(LOG_ERR, "log err: %s\n", str);
	syslog(LOG_NOTICE, "log notice: %s\n", str);
	closelog();
	return 0;
}
