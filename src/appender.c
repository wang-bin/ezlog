/******************************************************************************
    ezlog:  a tiny any flexible log system written in C
    Copyright (C) 2012-2013 Wang Bin <wbsecg1@gmail.com>

*   This file is part of ezlog

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/


#include "appender.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eztime.h"
#include "list.h"
#include "ezmutex.h"

extern char* const g_global_layout;
extern void ezlog_set_appender_with_layout(appender_t *appender, const char* format);

typedef struct {
    appender_t *appender;
	struct list_head list;
} appender_node;

//appender_node console_appender_node = {console_appender, {}};
//appender_node file_appender_node = {file_appender, {}};

#ifndef PATH_MAX
#define PATH_MAX 256
#endif
typedef struct {
	char first;
    char is_open;
	FILE *file;
	char name[PATH_MAX];
	LogOpenMode mode; //LogOpenMode
} logfile_t;

LIST_HEAD(appenders_head);

#define SIZE_LOGFILENAME 19 //yyyyMMddhhmmss.log

/*!
 * \brief ezlog_registerAppender
 * \param handle
 *
 */
void ezlog_registerAppender(appender_t* appender)
{
	appender_node *node;
	_ezmutex_lock();
	node = (appender_node*)malloc(sizeof(appender_node));
    node->appender = appender;
	INIT_LIST_HEAD(&node->list);
	list_add_tail(&(node->list), &appenders_head);

    ezlog_set_appender_with_layout(appender, g_global_layout);
	_ezmutex_unlock();
}

void ezlog_unregisterAppender(appender_t *appender)
{
	struct list_head *pos;
	_ezmutex_lock();
	pos = &appenders_head;
	/*TODO: use while(!list_empty())*/
	list_for_each(pos, &appenders_head) {
		appender_node* node = list_entry(pos, appender_node, list);
        if (node->appender == appender) {
			list_del(&node->list);
            if (node->appender->close) {
                node->appender->close(node->appender->opaque);
            }
			free(node);
			node = 0;
			break;
		}
	}
	_ezmutex_unlock();
}

/* TODO: lock*/
void ezlog_unregisterAllAppenders()
{
	appender_node* node;
	while (!list_empty(&appenders_head)) {
		node = list_entry(appenders_head.next, appender_node, list);
		list_del(appenders_head.next);
        if (node->appender->close) {
            node->appender->close(node->appender->opaque);
        }
		free(node);
	}
}

static FILE* __open_logfile(const char *path, int mode, logfile_t* lf)
{
	//First time is the same as mode. Then will will append the msg if OPEN_ON_WRITE
	FILE *file = 0;
	if(!strcmp(path, "stdout")) {
		file = stdout;
        lf->is_open = 1;
	} else if(!strcmp(path, "stderr")) {
		file = stderr;
        lf->is_open = 1;
	} else {
		const char* m = "ab";
        if ((mode & New) == New && lf->first) {
			m = "wb";
		}

		file = fopen(path, m);
		if(!file) {
#ifndef Q_OS_WINCE
			perror("Error opening log file"); //wince does not support
#endif
            lf->is_open = 0;
			return 0;
		} else {
            lf->first = 0;
            lf->is_open = 1;
		}
	}
	return file;
}

/* DO NOT LOCK */
void console_appender_handle(const char* msg, void* opaque)
{
    fprintf(stdout, "%s\n", msg);
    fflush(stdout);  //condition?
}

appender_t *console_appender()
{
    appender_t *a = (appender_t*)malloc(sizeof(appender_t));
    a->handle = console_appender_handle;
    return a;
}

void file_appender_handle(const char* msg, void* opaque)
{
    logfile_t* lf = (logfile_t*)opaque;
    if (IS_OPEN_ON_WRITE(lf->mode) || !lf->is_open) {
        FILE *file = __open_logfile(lf->name, lf->mode, lf);
        if (!file) {
            return;
        }
        lf->file = file;
    }
    fprintf(lf->file, "%s\n", msg);
    //fflush(lf->file);  //condition?
    if (IS_OPEN_ON_WRITE(lf->mode) && lf->file != stdout && lf->file != stderr) {
        lf->is_open = 0;
        fclose(lf->file); //TODO: check error
    }
}

void file_appender_close(void *opaque)
{
    logfile_t* lf = (logfile_t*)opaque;
    if (lf->is_open) {
        lf->is_open = 0;
        fclose(lf->file); //TODO: check error
    }
}

appender_t *file_appender(const char *name, LogOpenMode om)
{
    logfile_t* lf;
    eztime t;
    appender_t *appender = (appender_t*)malloc(sizeof(appender_t));
    if (!appender) {
        return 0;
    }
    lf = (logfile_t*)malloc(sizeof(logfile_t));
    if (!lf) {
        return 0;
    }
    memset(lf, 0, sizeof(logfile_t));
    lf->mode = om;
    lf->first = 1;
    lf->is_open = 0;
    if (!name) {
        getTime(&t);
        memset(lf->name, 0, sizeof(lf->name));
        sprintf(lf->name, "%d%02d%02d%02d%02d%02d.log", t.year, t.month, t.day
                , t.hour, t.min, t.sec);
    } else {
        strcpy(lf->name, name);
    }
    appender->opaque = lf;
    appender->handle = file_appender_handle;
    appender->close = file_appender_close;
    //open file here?
    return appender;
}


/*
	for internal use. lock here so that appender need not care about thread issues
*/
void __log_to_appender(appender_t *appender, const char* msg)
{
    appender->handle(msg, appender->opaque);
}

void __log_to_appenders(const char* msg)
{
	struct list_head *pos;
	/*_ezmutex_lock();*/
	pos = &appenders_head;
	list_for_each(pos, &appenders_head) { //list_for_each_entry
		appender_node* node = list_entry(pos, appender_node, list);
        node->appender->handle(msg, node->appender->opaque);
	}
	/*_ezmutex_unlock();*/
}
