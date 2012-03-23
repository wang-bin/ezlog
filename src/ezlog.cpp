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

#include "ezlog.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ezthread.h"
#include "eztime.h"
#include "layout.h"


ezmutex mutex;
//__func__
//strftime()

void ezlog_init_default()
{
    ezlog_init_layout(layout_format[DEFAULT_LAYOUT]);
}

/*!
	va_list: fmt
	format_print() will not change the source string. Parameter str MUST BE END WITH '%'!
	The reason is in strtok(), but i don't know it now :(
*/
int _ezlog_print(const char* file, const int line, const char* func, const char* fmt, ...)
{
	int r=0;

    char msg[512];
	va_list args;
	va_start(args, fmt);
    r += vsprintf(msg, fmt, args);
	va_end(args);
    r += sprintf(msg + r, "\n");

    eztime t;
    ezlog_info info; //static
    info.file = file;
    info.func = func;
    info.line = line;
    info.t = &t;
    info.pid = pid();
    info.tid = threadId();
    info.msg = msg;

    static char result_msg[1024];
    memset(result_msg, 0, sizeof(result_msg));
    __format_msg(result_msg, &info);
    __log_to_appenders(result_msg);

	return r;
}


void ezlog_fini() {
    ezlog_unregisterAllAppenders();
}
