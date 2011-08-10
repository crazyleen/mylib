#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <netdb.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/file.h>
#include<sys/resource.h>
#include "mylog.h"


/* Path to the unix socket */
static char lfile[30];

static const char *logFilePath = MY__LOG_FILE;

/* log file pointer */
static FILE *fp_logfile;

/* interval between marks in seconds */
static int MarkInterval = 20 * 60;

/* log recv buffer */
static char tmpbuf[TMP_BUF_SZ];
/*
 * Become a daemon. 
 * arg: cmd, program name.
 */
void
daemonize(const char *cmd)
{
	int					i, fd0, fd1, fd2;
	pid_t				pid;
	struct rlimit		rl;
	struct sigaction	sa;

	/*
	 * Clear file creation mask.
	 */
	umask(0);

	/*
	 * Get maximum number of file descriptors.
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
		printf("%s: can't get file limit", cmd);
		exit(1);
	}

	/*
	 * Become a session leader to lose controlling TTY.
	 */
	if ((pid = fork()) < 0){
		printf("%s: can't fork", cmd);
		exit(1);
	} else if (pid != 0) /* parent */
		exit(0);
	setsid();

	/*
	 * Ensure future opens won't allocate controlling TTYs.
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0){
		printf("%s: can't ignore SIGHUP", cmd);
		exit(1);
	}
	if ((pid = fork()) < 0){
		printf("%s: can't fork", cmd);
		exit(1);
	} else if (pid != 0) /* parent */
		exit(0);

	/*
	 * Change the current working directory to the root so
	 * we won't prevent file systems from being unmounted.
	 */
	if (chdir("/") < 0) {
		printf("%s: can't change directory to /", cmd);
		exit(1);
	}

	/*
	 * Close all open file descriptors.
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	/*
	 * Attach file descriptors 0, 1, and 2 to /dev/null.
	 */
	fd0 = open("/dev/console", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * Initialize the log file.
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
		  fd0, fd1, fd2);
		exit(1);
	}
	syslog(LOG_INFO, "%s become daemon now.", cmd);
}

static void quit_signal(int sig)
{
	//exit daemon.
	unlink(lfile);
	fclose(fp_logfile);
	exit(0);
}

static void domark(int sig)
{
	if (MarkInterval > 0) {
		//logMessage(LOG_SYSLOG | LOG_INFO, "-- MARK --");
		alarm(MarkInterval);
	}
}

/* handle log messages from client */
void savemylog(char *msg, int len)
{
	
	fprintf(fp_logfile, "%s", msg);
	//printf("%s", msg);
}

static void doSyslogd(void)
{
	struct sockaddr_un sunx;
	socklen_t addrLength;

	int sock_fd;
	fd_set fds;

	/* Set up signal handlers. */
	signal(SIGINT, quit_signal);
	signal(SIGTERM, quit_signal);
	signal(SIGQUIT, quit_signal);
	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
#ifdef SIGCLD
	signal(SIGCLD, SIG_IGN);
#endif
	signal(SIGALRM, domark);
	alarm(MarkInterval);

	/* Create the syslog file so realpath() can work. */
	if (realpath(MY_PATH_LOG, lfile) != NULL) {
		unlink(lfile);
	}

	memset(&sunx, 0, sizeof(sunx));
	sunx.sun_family = AF_UNIX;
	strncpy(sunx.sun_path, lfile, sizeof(sunx.sun_path));
	if ((sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("doSyslogd-socket");
		exit(1);
	}

	addrLength = sizeof(sunx.sun_family) + strlen(sunx.sun_path);
	if (bind(sock_fd, (struct sockaddr *) &sunx, addrLength) < 0) {
		perror("doSyslod-bind");
	}

	if (chmod(lfile, 0666) < 0) {
		perror("doSyslod-chmod");
	}

	//open log file
	if((fp_logfile = fopen(logFilePath, "a")) == NULL)
		perror("fopen");

	while (1) {

		FD_ZERO(&fds);
		FD_SET(sock_fd, &fds);

		//printf("start to select...\n");
		if (select(sock_fd + 1, &fds, NULL, NULL, NULL) < 0) {
			if (errno == EINTR) {
				/* alarm may have happened. */
				continue;
			}
			perror("select error");
		}

		if (FD_ISSET(sock_fd, &fds)) {
			int i;

			if ((i = recv(sock_fd, tmpbuf, TMP_BUF_SZ, 0)) > 0) {
				tmpbuf[i] = '\0';
				savemylog(tmpbuf, i);
			} else {
				perror("UNIX socket error");
			}
		}				/* FD_ISSET() */
	}					/* for main loop */
}

int main(int argc, char **argv)
{
	//daemonize(argv[0]);
	doSyslogd();
	return -1;
}
