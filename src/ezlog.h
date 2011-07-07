/******************************************************************************
	ezlog: a tiny log for c++
	Copyright (C) 2011 Wang Bin <wbsecg1@gmail.com>

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

#ifndef EZLOG_H
#define EZLOG_H

#include <stdio.h>
#include "global.h"

typedef enum {
	Append, New
} LogOpenMode;


/*
  If path is empty, then just output log messages to stdout or stderr or do nothing. Otherwise, record log to a log file at the same time.
*/
int ezlog_init_output(const char* path, LogOpenMode mode = New);
void ezlog_init_format(const char* format);

#define ezlog_msg(fmt, ...) _ezlog_print(stdout, __FILE__, __LINE__, __PRETTY_FUNCTION__, ""#fmt, ##__VA_ARGS__)
#define ezlog_error(fmt, ...) _ezlog_print(stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__, ""#fmt, ##__VA_ARGS__)
#define ezlog_file(fmt, ...) _ezlog_print(0, __FILE__, __LINE__, __PRETTY_FUNCTION__, ""#fmt, ##__VA_ARGS__)
//DO NOT use (fmt, args...), MSVC does not support it. use (fmt, ...)

void ezlog_fini();
/*
  Internal. out is stdout, stderr or 0. Each will put log message to a log file if exists.
*/
int _ezlog_print(FILE* out, const char* file, const int line, const char* func, const char* fmt, ...);

#endif //EZLOG_H
