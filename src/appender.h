/******************************************************************************
	appender.h: description
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


#ifndef APPENDER_H
#define APPENDER_H
#include "ezlog_global.h"

enum LogOpenMode {
	Append = 0x01, New = 0x02, OPEN_ON_WRITE = 0x04
};

#define IS_OPEN_ON_WRITE(m) ((m & OPEN_ON_WRITE) == OPEN_ON_WRITE)

typedef void (*appender)(const char* msg);


Q_EXPORT void ezlog_registerAppender(appender handle); //installHandler(handler)
Q_EXPORT void ezlog_unregisterAppender(appender handle);
Q_EXPORT void ezlog_unregisterAllAppenders();
Q_EXPORT void ezlog_add_logfile(const char* path, int mode); //LogOpenMode
Q_EXPORT void ezlog_remove_logfile(const char* path);
/*
  pre defined appenders: file, console
  just output the formated message.
*/
Q_EXPORT void console_appender(const char* msg);
Q_EXPORT void file_appender(const char* msg);

#endif // APPENDER_H
