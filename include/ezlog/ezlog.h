/******************************************************************************
	ezlog: A tiny log for C/C++
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
#include "ezlog_global.h"
#include "appender.h"
#include "layout.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

enum Level {
	debug, info, warn, error, fatal
};


Q_EXPORT int ezlog_version();
Q_EXPORT const char* ezlog_version_string();

Q_EXPORT void ezlog_init_default();

#if defined(NO_EZLOG) || defined(NO_DEBUG)
#define ezlog(...)
#define ezlog_msg(...)
#define ezlog_debug(...)
#define ezlog_info(...)
#define ezlog_warn(...)
#define ezlog_error(...)
#define ezlog_fatal(...)
#else
//DO NOT use (fmt, args...), MSVC does not support it. use (fmt, ...)
#define ezlog(level, ...) ezlog_##level(##__VA_ARGS__)
/*align level string*/
#define ezlog_debug(fmt, ...) _ezlog_print("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_info(fmt, ...) _ezlog_print("INFO ", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_warn(fmt, ...) _ezlog_print("WARN ", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_error(fmt, ...) _ezlog_print("ERROR", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_fatal(fmt, ...) _ezlog_print("FATAL", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#endif

/*Usually this is called automatically after main()*/
Q_EXPORT void ezlog_fini();

/* for internal use */
Q_EXPORT void _ezlog_print(const char* level, const char* file, const int line, const char* func, const char* fmt, ...);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //EZLOG_H
