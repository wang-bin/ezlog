/******************************************************************************
    ezlog:  a tiny any flexible log system written in C
    Copyright (C) 2011-2013 Wang Bin <wbsecg1@gmail.com>

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

#ifndef EZLOG_H
#define EZLOG_H

#if defined(NO_EZLOG) || defined(NO_DEBUG)
#define ezlog(...)
#define ezlog_msg(...)
#define ezlog_debug(...)
#define ezlog_info(...)
#define ezlog_warn(...)
#define ezlog_error(...)
#define ezlog_fatal(...)
#define ezlog_version(...)
#define ezlog_version_string(...)
#define ezlog_init_default(...)
#define ezlog_set_appender_with_layout(...)
#define ezlog_fini(...)
#define ezlog_set_global_layout(...)
#define ezlog_init_layout(...)
#define ezlog_register_appender(...)
#define ezlog_unregister_appender(...)
#define ezlog_unregister_appenders(...)
#define console_appender(...)
#define file_appender(...)
#else
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

/* runtime version. used to compare with compile time version */
EZLOG_EXPORT int ezlog_version();
EZLOG_EXPORT const char* ezlog_version_string();

EZLOG_EXPORT void ezlog_init_default();
/* If appender not exists, insert it. If already exists, set the new format, because
 * an appender only has 1 layout, a layout can attach many appenders*/
EZLOG_EXPORT void ezlog_set_appender_with_layout(appender_t *appender, const char* format);

//DO NOT use (fmt, args...), MSVC does not support it. use (fmt, ...)
#define ezlog(level, ...) ezlog_##level(##__VA_ARGS__)
/*align level string*/
#define ezlog_debug(fmt, ...) _ezlog_print("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, #fmt, ##__VA_ARGS__)
#define ezlog_info(fmt, ...) _ezlog_print("INFO ", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_warn(fmt, ...) _ezlog_print("WARN ", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_error(fmt, ...) _ezlog_print("ERROR", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define ezlog_fatal(fmt, ...) _ezlog_print("FATAL", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)

/*Usually this is called automatically after main()*/
EZLOG_EXPORT void ezlog_fini();

/* for internal use */
EZLOG_EXPORT void _ezlog_print(const char* level, const char* file, const int line, const char* func, const char* fmt, ...);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif

#endif //EZLOG_H
