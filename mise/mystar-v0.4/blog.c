/*
 *
 * crazyleen <357228736@qq.com>
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
char * what_time(void)
{
	char *p;
	static char t[9];
	time_t timep;
	time(&timep);
	p = ctime(&timep);
	strncpy(t, p+11, 8);
	return t;
}


void mylog(char *s, ...)
{
	va_list ap;
	int t;
	va_start(ap, s);

	printf("%s", s);
	while ((t = va_arg(ap, int)))
		printf("%d ", t);

	va_end(ap);
}
