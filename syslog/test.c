#include "mylog.h"
char *p = "crazyleen";
int main(int argc, char **argv)
{
	openmylog("test", 0, 0);
	mylog(1, "myname is: %s\n", p);
	mylog(2, "well done!!!!\n");
}
