

#ifndef __MY_LOG_C
#define __MY_LOG_C
/* Path for the file where all log messages are written */
#define MY__LOG_FILE "/var/log/sniffer"

#define MY_PATH_LOG  "/dev/mylog"

/* log recv buff size */
# define TMP_BUF_SZ 2048

void openmylog(const char *ident, int option, int facility);
void mylog(int priority, const char *format, ...);
void closemylog(void);
#endif
