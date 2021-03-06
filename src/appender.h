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

typedef struct {
    void (*handle)(const char* msg, void* opaque);
    int (*open)(void* opaque);
    int (*close)(void* opaque);
    void *opaque;
} appender_t;

/*bind to global layout (not const)*/
EZLOG_EXPORT void ezlog_register_appender(appender_t* appender); //installHandler(handler)
EZLOG_EXPORT void ezlog_unregister_appender(appender_t* appender);
EZLOG_EXPORT void ezlog_unregister_appenders();
/*!
   unregister old one.
   appender==0 means disable default appender. console appender is the default appender by default
*/
EZLOG_EXPORT void ezlog_set_default_appender(appender_t *appender);
EZLOG_EXPORT appender_t* ezlog_get_default_appender();
/*
  pre defined appenders: file, console
  just output the formated message.
*/
EZLOG_EXPORT appender_t* console_appender();

typedef enum {
    Append = 0x01, New = 0x02, OPEN_ON_WRITE = 0x04
} LogOpenMode;
#define IS_OPEN_ON_WRITE(m) ((m & OPEN_ON_WRITE) == OPEN_ON_WRITE)
/*
 * use default file name if name == 0. The default log file's name is yyyyMMddhhmmss.log.
*/
EZLOG_EXPORT appender_t* file_appender(const char* name, LogOpenMode om);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // APPENDER_H
