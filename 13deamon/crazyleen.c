/* 
 *	守护进程 函数库
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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include<sys/resource.h>
#if defined(SOLARIS)
#define _XOPEN_SOURCE	500	/* Single UNIX Specification, Version 2  for Solaris 9 */
#define CMSG_LEN(x)	_CMSG_DATA_ALIGN(sizeof(struct cmsghdr)+(x))
#elif !defined(BSD)
#define _XOPEN_SOURCE	600	/* Single UNIX Specification, Version 3 */
#endif

#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */

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
	fd0 = open("/dev/null", O_RDWR);
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




/*
 * file lock. lock will be deleted automatically when program ends.
 * return value: < 0 , faile
 */
int
lockfile(int fd)
{
	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return(fcntl(fd, F_SETLK, &fl));
}


/*
 * Ensure only one daemon runs at one time.
 * return: 0 , no one running.
 * 			1, one running.
 */
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
int
already_running(const char *fpathname)
{
	int		fd;
	char	buf[16];
	
	fd = open(fpathname, O_RDWR|O_CREAT, LOCKMODE);
	if (fd < 0) {
		syslog(LOG_ERR, "can't open %s: %s", fpathname, strerror(errno));
		exit(1);
	}
	if (lockfile(fd) < 0) {
		if (errno == EACCES || errno == EAGAIN) {
			close(fd);
			return(1);
		}
		syslog(LOG_ERR, "can't lock %s: %s", fpathname, strerror(errno));
		exit(1);
	}

	//cut file length to 0.
	ftruncate(fd, 0);	
	
	//write pid to file.
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf)+1);	
	return(0);
}

/* 
 * Change stdin, 1, 2 to a file
 */
void
chgstd123(const char *fpathname)
{
	int					i, fd0, fd1, fd2;

	for (i = 0; i < 3; i++)
		close(i);

	/*
	 * Attach file descriptors 0, 1, and 2 to your file.
	 */
	fd0 = open(fpathname, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH);
	fd1 = dup(0);
	fd2 = dup(0);

	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		printf("unexpected file descriptors %d %d %d\n", fd0, fd1, fd2);
		exit(1);
	}
	printf("change stdio to %s: OK!\n", fpathname);
	printf("expected file descriptors %d %d %d\n", fd0, fd1, fd2);
}

#if 0
sigset_t	mask;

void
reread(void)
{
	/* ... */
}

void *
thr_fn(void *arg)
{
	int err, signo;

	for (;;) {
		err = sigwait(&mask, &signo);
		if (err != 0) {
			syslog(LOG_ERR, "sigwait failed");
			exit(1);
		}

		switch (signo) {
		case SIGHUP:
			syslog(LOG_INFO, "Re-reading configuration file");
			reread();
			break;

		case SIGTERM:
			syslog(LOG_INFO, "got SIGTERM; exiting");
			exit(0);

		default:
			syslog(LOG_INFO, "unexpected signal %d\n", signo);
		}
	}
	return(0);
}


int
main(int argc, char *argv[])
{
	int					err;
	pthread_t			tid;
	char				*cmd;
	struct sigaction	sa;

	if ((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

	/*
	 * Become a daemon.
	 */
	daemonize(cmd);

	/*
	 * Make sure only one copy of the daemon is running.
	 */
	if (already_running("/var/run/daemon.pid")) {
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}

	/*
	 * Restore SIGHUP default and block all signals.
	 */
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0){
		printf("main: can't restore SIGHUP default");
		exit(1);
	}
	sigfillset(&mask);
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0){
		perror("pthread_sigmask");
		exit(1);
	}

	/*
	 * Create a thread to handle SIGHUP and SIGTERM.
	 */
	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0){
		perror("pthread_create:");
		exit(1);
	}

	/*
	 * Proceed with the rest of the daemon.
	 */
	while(1){
		sleep(30);
		syslog(LOG_INFO, "sleep again...");
	}
	exit(0);
}

#endif

#if 0
void
reread(void)
{
	/* ... */
	printf("reread file...\n");
}

void
sigterm(int signo)
{
	syslog(LOG_INFO, "got SIGTERM; exiting");
	exit(0);
}

void
sighup(int signo)
{
	syslog(LOG_INFO, "Re-reading configuration file");
	reread();
}

int
main(int argc, char *argv[])
{
	char				*cmd;
	struct sigaction	sa;

	if ((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

	/*
	 * Become a daemon.
	 */
	daemonize(cmd);

	/*
	 * Make sure only one copy of the daemon is running.
	 */
	if (already_running("/var/run/daemon.pid")) {
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}
	
	/*
	 * Handle signals of interest.
	 */
	sa.sa_handler = sigterm;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGHUP);
	sa.sa_flags = 0;
	if (sigaction(SIGTERM, &sa, NULL) < 0) {
		syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
		exit(1);
	}
	sa.sa_handler = sighup;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		syslog(LOG_ERR, "can't catch SIGHUP: %s", strerror(errno));
		exit(1);
	}

	/*
	 * Proceed with the rest of the daemon.
	 */
	while(1){
		printf("SLEEP AGIAN\n");
		sleep(10);
	}
	exit(0);
}

#endif

int
main(int argc, char *argv[])
{
	chgstd123("logfile");
	printf("hello log file");

	return 0;
}

