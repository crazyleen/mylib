/*  
 *  Copyright (C) 2011 crazyleen <357228736@qq.com>, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * HOWTO:
 * 	$(CC) $(Flags) -c mynotify.c -o mynotify.o `pkg-config --cflags --libs gtk+-2.0` -l notify
 */

#include <libnotify/notify.h>
#include "mynotify.h"

int notification_show(char *title, char *body, char *icon)
{
	GError *error = NULL;
	NotifyNotification *notify_p;

	if (title == NULL)
		return -1;

	//you should init notify using app's name
	if(notify_is_initted() == FALSE)
		if(notify_init("argv[0]") == FALSE){
			return -1;
		}
	
    notify_p = notify_notification_new(title, body, icon, NULL);
	
    //miliseconds, NOTIFY_EXPIRES_DEFAULT NOTIFY_EXPIRES_NEVER
	notify_notification_set_timeout(notify_p, 1000);
   
	//NOTIFY_URGENCY_LOW,NOTIFY_URGENCY_NORMAL, NOTIFY_URGENCY_CRITICAL,
    notify_notification_set_urgency (notify_p,NOTIFY_URGENCY_NORMAL);

	//notify_notification_update(title, body, icon, NULL);
	//notify_notification_close(notify_p, &error);
    if (notify_notification_show(notify_p, &error) == FALSE)
		return -1;

	//notify_uninit();
	return 0;
}
