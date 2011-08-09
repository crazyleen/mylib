/* mynotify.h
 * this function is to display a notification on the screan,
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Copyright (C) 2011 crazyleen <357228736@qq.com>, All Rights Reserved.
 */

#ifndef MYNOTIFY_H
#define MYNOTIFY_H

/* body & icoon are both optional
 * Returns : error: -1, success return 0
 */
int notification_show(char *title, char *body, char *icon);

#endif
