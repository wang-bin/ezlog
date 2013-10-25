/******************************************************************************
    ezlog:  a tiny any flexible log system written in C
    Copyright (C) 2012-2013 Wang Bin <wbsecg1@gmail.com>

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


#ifndef APPENDER_H
#define APPENDER_H
#include "ezlog_global.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

typedef enum {
	Append = 0x01, New = 0x02, OPEN_ON_WRITE = 0x04
} LogOpenMode;

#define IS_OPEN_ON_WRITE(m) ((m & OPEN_ON_WRITE) == OPEN_ON_WRITE)

typedef struct {
    void (*handle)(const char* msg, void*);
    void *opaque;
} appender_t;

/*bind to global layout (not const)*/
Q_EXPORT void ezlog_registerAppender(appender_t* appender); //installHandler(handler)
Q_EXPORT void ezlog_unregisterAppender(appender_t* appender);
Q_EXPORT void ezlog_unregisterAllAppenders();
/*
 *The default log file's name is yyyyMMddhhmmss.log. The default log file will be ignored if
 *ezlog_add_logfile() is called.
*/
Q_EXPORT void ezlog_add_logfile(const char* path, int mode); //LogOpenMode
Q_EXPORT void ezlog_remove_logfile(const char* path);
Q_EXPORT void ezlog_remove_logfiles();
/*
  pre defined appenders: file, console
  just output the formated message.
  TODO: how to release?
*/
Q_EXPORT appender_t* console_appender();
/* return a file appender with file name */
Q_EXPORT appender_t* file_appender(const char* name);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // APPENDER_H
