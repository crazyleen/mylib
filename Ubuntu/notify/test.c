/* file.c
 * Copyright (C) 2011 crazyleen <357228736@qq.com>, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mynotify.h"


int main(char argc, char *argv[])
{
	notification_show("cheery", "here is body...", "/usr/share/pixmaps/apple-green.png");
	sleep(5);
	notification_show("cheery", NULL, NULL);
	printf("UNIX\n");
	return 0;
}
