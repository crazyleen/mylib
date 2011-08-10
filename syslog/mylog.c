#include <stdio.h>
#include <errno.h>
#include <paths.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdarg.h>
#include <stdlib.h>
#include "mylog.h"

static char progname[20 + 1] = "\0";

void openmylog(const char *ident, int option, int facility)
{
	snprintf(progname, 20, "%s", ident);
}

static int openlogUNIX(void)
{
	char lfile[30];
	int sock_fd = -1;
	struct sockaddr_un sunx;
	socklen_t addrLength;

	//lfile=(_PATH_LOG)'s path name.
	if (realpath(MY_PATH_LOG, lfile) == NULL) {
		return -1;
	}

	memset(&sunx, 0, sizeof(sunx));
	sunx.sun_family = AF_UNIX;
	strncpy(sunx.sun_path, lfile, sizeof(sunx.sun_path));
	if ((sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		//printf("Couldn't get file descriptor for socket\n");
		return -1;
	}
	addrLength = sizeof(sunx.sun_family) + strlen(sunx.sun_path);
	if (connect(sock_fd, (struct sockaddr *)&sunx, addrLength) < 0) {
		return -1;
	}
	return sock_fd;
}

#define LOGBUF_SIZE 1024
void mylog(int priority, const char *format, ...)
{
	va_list arg;
	char buf[LOGBUF_SIZE]="\0";
	static int sock_fd = -1;

	/* See if UNIX sock is opened */
	if(sock_fd == -1) {
		sock_fd = openlogUNIX();
		if(sock_fd == -1) {
			perror("openmylog:");
			return;		
		}
	}
	
	strcpy(buf, progname);
	sprintf(buf + strlen(buf), " [%d] ", getpid());
	/* print log message to buf as a string. */
	va_start(arg, format);
	vsnprintf(buf + strlen(buf), LOGBUF_SIZE, format, arg);
	va_end(arg);

	/* Send log message to mylogd */
	write(sock_fd, buf, strlen(buf));
	
	return;
}

void closemylog(void)
{

}


