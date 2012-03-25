/******************************************************************************
	appender.cpp: description
	Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/


#include "appender.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct {
	appender handle;
	struct list_head list;
} appender_node;

//appender_node console_appender_node = {console_appender, {}};
//appender_node file_appender_node = {file_appender, {}};

#ifndef PATH_MAX
#define PATH_MAX 256
#endif
typedef struct {
	FILE *file;
	char name[PATH_MAX];
	LogOpenMode mode; //LogOpenMode
	struct list_head list;
} logfile_node;

LIST_HEAD(appenders_head);
LIST_HEAD(logfiles_head);

void ezlog_registerAppender(appender handle)
{
	appender_node *node = (appender_node*)malloc(sizeof(appender_node));
	node->handle = handle;
	list_add_tail(&(node->list), &appenders_head);
}

void ezlog_unregisterAppender(appender handle)
{
	struct list_head *pos = &appenders_head;
	list_for_each(pos, &appenders_head) {
		appender_node* node = list_entry(pos, appender_node, list);
		if (node->handle == handle) {
			list_del(&node->list);
			free(node);
			node = 0;
			break;
		}
	}
}

void ezlog_unregisterAllAppenders()
{
	struct list_head *pos = &appenders_head;
	list_for_each(pos, &appenders_head) {
		appender_node* node = list_entry(pos, appender_node, list);
		//FIXME: seg fault!
		//list_del(&node->list);
		free(node);
		node = 0;
	}
}

FILE* __open_logfile(const char *path, int mode)
{
	//First time is the same as mode. Then will will append the msg if OPEN_ON_WRITE
	static int _mode = mode;
	FILE *file = 0;
	if(!strcmp(path, "stdout")) {
		file = stdout;
	} else if(!strcmp(path, "stderr")) {
		file = stderr;
	} else {
		const char* m = "ab";
		if ((_mode & New) == New) {
			m = "wb";
		}

		file = fopen(path, m);
		if(!file) {
			//perror("open log file failed!"); //wince does not support
		}
	}
	_mode = Append;
	return file;
}

void ezlog_add_logfile(const char *path, int mode)
{
	//ezscoped_lock lock(mutex);

	logfile_node *node = (logfile_node*)malloc(sizeof(logfile_node));
	if (!IS_OPEN_ON_WRITE(mode)) {
		FILE *file = __open_logfile(path, mode);
		node->file = file;
	}
	strcpy(node->name, path);
	node->mode = (LogOpenMode)mode;
	//printf("%s: %p\n", node->name, node->file);
	list_add_tail(&(node->list), &logfiles_head);
}

//convert to abs path?
void ezlog_remove_logfile(const char *path)
{
	struct list_head *pos = &logfiles_head;
	list_for_each(pos, &logfiles_head) { //list_for_each_entry
		logfile_node* node = list_entry(pos, logfile_node, list);
		if (strcmp(node->name, path) == 0) {
			if (!IS_OPEN_ON_WRITE(node->mode))
				fclose(node->file);
			list_del(&(node->list));
			break;
		}
	}
}


void console_appender(const char *msg)
{
	fprintf(stdout, "%s\n", msg);
	fflush(stdout);  //condition?
}

void file_appender(const char *msg)
{
	struct list_head *pos = &logfiles_head;
	list_for_each(pos, &logfiles_head) { //list_for_each_entry
		logfile_node* node = list_entry(pos, logfile_node, list);
		if (IS_OPEN_ON_WRITE(node->mode)) {
			FILE *file = __open_logfile(node->name, node->mode);
			fprintf(file, "%s\n", msg);
			fclose(file);
		} else {
			fprintf(node->file, "%s\n", msg);
			fflush(node->file);  //condition?
		}
	}
}


//for internal use
void __log_to_appenders(const char* msg)
{
	struct list_head *pos = &appenders_head;
	list_for_each(pos, &appenders_head) { //list_for_each_entry
		appender_node* node = list_entry(pos, appender_node, list);
		node->handle(msg);
	}
}
