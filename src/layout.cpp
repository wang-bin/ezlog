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
#if 0
#include <stdio.h>
#include <string.h>

#include "eztime.h"
#include "list.h"

typedef struct
{
	const char* key;
	typedef int (*print_call)(char*, ezlog_info* info);
	print_call print;
} key_print;

//use snprintf to get the result string so it can be used anywhere!
int print_year(char* str, ezlog_info* info)
{
	return snprintf(str, "%s", info->t->year);
}

int print_line(char* str, ezlog_info* info)
{
	//return snprintf
}

static const key_print year_print = {"YY", print_year};
static const key_print line_print = {"line", print_line};



/*

list.insert(&year_print);
....
list.insert(&line_print);
typedef key_print* iterator;
static iterator list_begin, list_end; //used when looping the list
sort the list
*/
void ezlog_init_layout(const char *format)
{
	//ezscoped_lock lock(mutex);
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
	init key_print;
}


char* layout_string(key_print_list* list)
{
	static msg[1024];
	memset(msg, 0, sizeof(msg);
	int pos = 0;
	iterator it= list_begin;
	while(it != list_end) {
		pos = list.node->print(&msg[pos], info);
		list=list.next
		it = list.node
	}

	return msg; //not thread safe if do not copy

}
#endif
//copy outside. new char[strlen or char[1024]
