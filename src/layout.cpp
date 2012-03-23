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


typedef struct
{
	const char* key;
    typedef int (*print_call)(char*, ezlog_info* info);
    print_call print;
} key_print;

int print_year(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->year);
}

int print_month(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->month);
}

int print_day(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->day);
}

int print_hour(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->hour);
}

int print_min(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->min);
}

int print_sec(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->sec);
}

int print_msec(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->t->msec);
}


int print_file(char* str, ezlog_info* info)
{
    return sprintf(str, "%s", info->file);
}

int print_func(char* str, ezlog_info* info)
{
    return sprintf(str, "%s", info->func);
}

int print_line(char* str, ezlog_info* info)
{
    return sprintf(str, "%d", info->line);
}


int print_pid(char* str, ezlog_info* info)
{
    return sprintf(str, "%ld", info->pid);
}

int print_tid(char* str, ezlog_info* info)
{
    return sprintf(str, "%lu", info->tid);
}

int print_msg(char* str, ezlog_info* info)
{
    return sprintf(str, "%s", info->msg);
}



static const key_print year_print = {"YY", print_year};
static const key_print month_print = {"MM", print_month};
static const key_print day_print = {"DD", print_day};
static const key_print hour_print = {"hh", print_hour};
static const key_print min_print = {"mm", print_min};
static const key_print sec_print = {"ss", print_sec};
static const key_print msec_print = {"ms", print_msec};
static const key_print file_print = {"file", print_file};
static const key_print func_print = {"func", print_func};
static const key_print line_print = {"line", print_line};
static const key_print pid_print = {"pid", print_pid};
static const key_print tid_print = {"tid", print_tid};
static const key_print msg_print = {"", print_msg};

typedef struct {
    const key_print *printer;
    struct list_head list;
} key_print_node;



LIST_HEAD(key_print_head);

void ezlog_init_layout(const char *format)
{
	//ezscoped_lock lock(mutex);
    static char* format_str = NULL;
	if(format_str!=NULL) {
		free(format_str);
		format_str = NULL;
	}
    int format_strlen = strlen(format);
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

    struct list_head *pos = &key_print_head;
    list_for_each(pos, &key_print_head) {
        key_print_node* node = list_entry(pos, key_print_node, list);
        list_del(&node->list);
        free(node);
        node = 0;
    }

    char *pch = strtok (format_str,"%");
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
        } else {
            key_print_node *node = (key_print_node*)malloc(sizeof(key_print_node));
            node->printer = &msg_print;
            list_add_tail(&(node->list), &key_print_head);
        }
        *(pch+strlen(pch)) = '%'; //strtok will replace '%' with '\0'.
        pch = strtok (NULL, "%"); //why NULL?
    }
}
/*
char* ezlog_layout_msg(const char *msg_extra)
{
    eztime t;

    ezlog_info info; //static
    info.file = file;
    info.func = func;
    info.line = line;
    info.t = &t;
    info.pid = pid();
    info.tid = threadId();
    info.msg = msg_extra;

    static char result_msg[1024];
    return __layout_string(result_msg, &info);
}
*/
void __format_string(char* result_msg, ezlog_info* info)
{
    int index = 0;
    struct list_head *pos = &key_print_head;
    list_for_each(pos, &key_print_head) {
        key_print_node* node = list_entry(pos, key_print_node, list);
        index += node->printer->print(&result_msg[index], info);
    }

    //return result_msg; //not thread safe if do not copy
}

//copy outside. new char[strlen or char[1024]
