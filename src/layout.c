/******************************************************************************
	layout.cpp: description
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


#include "layout.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eztime.h"
#include "list.h"
#include "ezmutex.h"

typedef struct {
	const char* level;
	const char* file;
	const char* func;
	int line;
	const eztime* t;
	unsigned long tid;
	long pid;
	const char* msg;
} ezlog_context;

typedef int (*print_call)(char*, ezlog_context* info);
typedef struct
{
	const char* key;
	char* delimiter; //0: if key != 0; a string between % and % if key == 0
	print_call print;
} key_print;


#define PRINT_DEF(item, tag) \
	int print_##item(char* str, ezlog_context* info) { \
		return sprintf(str, "%"#tag, info->item); \
	}

#define PRINT_DEF_T(item, tag) \
	int print_##item(char* str, ezlog_context* info) { \
		return sprintf(str, "%"#tag, info->t->item); \
	}
PRINT_DEF(level, s)
PRINT_DEF(file, s)
PRINT_DEF(func, s)
PRINT_DEF(line, d)
PRINT_DEF(pid, ld)
PRINT_DEF(tid, #lx) //lu
PRINT_DEF(msg, s)
PRINT_DEF_T(year, d)
PRINT_DEF_T(month, 02d)
PRINT_DEF_T(day, 02d)
PRINT_DEF_T(hour, 02d)
PRINT_DEF_T(min, 02d)
PRINT_DEF_T(sec, 02d)
PRINT_DEF_T(msec, 02d)


static const key_print year_print = {"YY", 0, print_year};
static const key_print month_print = {"MM", 0, print_month};
static const key_print day_print = {"DD", 0, print_day};
static const key_print hour_print = {"hh", 0, print_hour};
static const key_print min_print = {"mm", 0, print_min};
static const key_print sec_print = {"ss", 0, print_sec};
static const key_print msec_print = {"ms", 0, print_msec};
static const key_print level_print = {"level", 0, print_level};
static const key_print file_print = {"file", 0, print_file};
static const key_print func_print = {"func", 0, print_func};
static const key_print line_print = {"line", 0, print_line};
static const key_print pid_print = {"pid", 0, print_pid};
static const key_print tid_print = {"tid", 0, print_tid};
static const key_print msg_print = {"msg", 0, print_msg};


typedef struct {
	const key_print *printer;
	struct list_head list;
} key_print_node;

LIST_HEAD(key_print_head);


/*
	va_list: fmt
	this will not change the source string. Parameter 'format' MUST BE END WITH '%'!
	The reason is in strtok(), but i don't know it now :(
*/
void ezlog_init_layout(const char *format)
{
	int format_strlen;
	struct list_head *pos;
	char *pch;
	static char* format_str = NULL;
	int is_key = 1;
	key_print_node* node = 0;
	key_print *dummy_print = 0;
	_ezmutex_lock();
	if(format_str) {
		free(format_str);
		format_str = NULL;
	}
	format_strlen = strlen(format);
	if(format[format_strlen]!='%') {
		format_strlen += 1;
	}
	format_str = (char*)malloc(format_strlen+1);
	memset(format_str, 0, format_strlen+1);
	memcpy(format_str, format, strlen(format));
	/*
		force end with '%'. If already is, no effect. If not, it will be '%'
	*/
	*(format_str+format_strlen-1) = '%';
	//init key_print_list

	pos = &key_print_head;
	list_for_each(pos, &key_print_head) {
		node = list_entry(pos, key_print_node, list);
		list_del(&node->list);
		free(node);
		node = 0;
	}

	//TODO: print the keywords use '\'
	pch = strtok(format_str,"%");
	while (pch != NULL) {
		is_key = 1;
		//printf ("%s\n",pch);
		if(!strcmp(pch, "YY")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &year_print;
		} else if(!strcmp(pch, "MM")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &month_print;
		} else if(!strcmp(pch, "DD")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &day_print;
		} else if(!strcmp(pch, "hh")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &hour_print;
		} else if(!strcmp(pch, "mm")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &min_print;
		} else if(!strcmp(pch, "ss")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &sec_print;
		} else if(!strcmp(pch, "ms")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &msec_print;
		} else if(!strcmp(pch, "level")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &level_print;
		} else if(!strcmp(pch, "file")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &file_print;
		} else if(!strcmp(pch, "func")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &func_print;
		} else if(!strcmp(pch, "line")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &line_print;
		} else if(!strcmp(pch, "tid")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &tid_print;
		} else if(!strcmp(pch, "pid")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &pid_print;
		} else if(!strcmp(pch, "msg")) {
			node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &msg_print;
		} else if (strlen(pch)) { //if meets %%, skip
			node = (key_print_node*)malloc(sizeof(key_print_node));
			dummy_print = (key_print*)malloc(sizeof(key_print));
			dummy_print->key = 0;
			//FIXME: when to release the memory?
			dummy_print->delimiter = (char*)malloc(strlen(pch) + 1);
			strcpy(dummy_print->delimiter, pch);
			dummy_print->print = 0;
			node->printer = dummy_print;
		} else {
			is_key = 0;
		}
		if (is_key) {
			list_add_tail(&(node->list), &key_print_head);
		}
		*(pch+strlen(pch)) = '%'; //strtok will replace '%' with '\0'.
		pch = strtok (NULL, "%"); //why NULL?
	}
	_ezmutex_unlock();
}

//the result string. use by ezlog.cpp to be put into appenders.
void __format_msg(char* result_msg, ezlog_context* info)
{
	int index = 0;
	struct list_head *pos;
	_ezmutex_lock();
	pos = &key_print_head;
	list_for_each(pos, &key_print_head) {
		key_print_node* node = list_entry(pos, key_print_node, list);
		//printf("index=%d, key=%s, delimiter=%s\n", index, node->printer->key, node->printer->delimiter);
		if (node->printer->key == 0)
			index += sprintf(result_msg + index, "%s", node->printer->delimiter);
		else
			index += node->printer->print(result_msg + index, info);
	}
	_ezmutex_unlock();
}

