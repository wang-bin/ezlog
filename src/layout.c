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

typedef struct {
	const char* level;
	const char* file;
	const char* func;
	int line;
	const eztime* t;
	unsigned long tid;
	long pid;
	const char* msg;
} ezlog_info;

typedef int (*print_call)(char*, ezlog_info* info);
typedef struct
{
	const char* key;
	char* delimiter; //0: if key != 0; a string between % and % if key == 0
	print_call print;
} key_print;


#define PRINT_DEF(item, tag) \
	int print_##item(char* str, ezlog_info* info) { \
		return sprintf(str, "%"#tag, info->item); \
	}

#define PRINT_DEF_T(item, tag) \
	int print_##item(char* str, ezlog_info* info) { \
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
	if(format_str!=NULL) {
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
		key_print_node* node = list_entry(pos, key_print_node, list);
		list_del(&node->list);
		free(node);
		node = 0;
	}

	//TODO: print the keywords use '\'
	pch = strtok(format_str,"%");
	while (pch != NULL) {
		//printf ("%s\n",pch);
		if(!strcmp(pch, "YY")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &year_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "MM")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &month_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "DD")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &day_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "hh")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &hour_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "mm")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &min_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "ss")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &sec_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "ms")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &msec_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "level")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &level_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "file")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &file_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "func")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &func_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "line")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &line_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "tid")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &tid_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "pid")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &pid_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if(!strcmp(pch, "msg")) {
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			node->printer = &msg_print;
			list_add_tail(&(node->list), &key_print_head);
		} else if (strlen(pch)) { //if meets %%, skip
			key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
			key_print *dummy_print = (key_print*)malloc(sizeof(key_print));
			dummy_print->key = 0;
			//FIXME: when to release the memory?
			dummy_print->delimiter = (char*)malloc(strlen(pch) + 1);
			strcpy(dummy_print->delimiter, pch);
			dummy_print->print = 0;
			node->printer = dummy_print;
			list_add_tail(&(node->list), &key_print_head);
		}
		*(pch+strlen(pch)) = '%'; //strtok will replace '%' with '\0'.
		pch = strtok (NULL, "%"); //why NULL?
	}
}

//the result string. use by ezlog.cpp to be put into appenders.
void __format_msg(char* result_msg, ezlog_info* info)
{
	int index = 0;
	struct list_head *pos = &key_print_head;
	list_for_each(pos, &key_print_head) {
		key_print_node* node = list_entry(pos, key_print_node, list);
		//printf("index=%d, key=%s, delimiter=%s\n", index, node->printer->key, node->printer->delimiter);
		if (node->printer->key == 0)
			index += sprintf(result_msg + index, "%s", node->printer->delimiter);
		else
			index += node->printer->print(result_msg + index, info);
	}
}

