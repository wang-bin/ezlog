/******************************************************************************
	layout.h: description
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


#ifndef LAYOUT_H
#define LAYOUT_H

#include "ezlog_global.h"

struct _eztime;
typedef struct {
	const char* file;
	const char* func;
	int line;
    const _eztime* t;
	unsigned long tid;
	long pid;
	const char* msg;
} ezlog_info;



Q_EXPORT void ezlog_init_layout(const char* format); //move to layout.h
//char* ezlog_layout_msg(const char* msg_extra);

//for internal use
//the result string. use by ezlog.cpp to be put into appenders.
void __format_string(char* result_msg, ezlog_info* info);

#endif // LAYOUT_H
