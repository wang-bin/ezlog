/******************************************************************************
	eztime: a tiny time wrapper for c++
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

#ifndef EZTIME_H
#define EZTIME_H

#include <time.h>
#include "ezlog_global.h"

#ifdef OS_WIN
#include <windows.h>
#endif //WINCE

typedef struct _eztime {
	int year, month, day;
	int hour, min, sec;
	int msec;
	void init(){
#ifdef OS_WIN
		SYSTEMTIME stUTC;
		::GetSystemTime(&stUTC);
		year = stUTC.wYear, month = stUTC.wMonth, day = stUTC.wDay;
		hour = stUTC.wHour, min = stUTC.wMinute, sec = stUTC.wSecond;
		msec = stUTC.wMilliseconds;
#else
		time_t seconds = time(NULL);
		struct tm* t = localtime(&seconds);
		year = t->tm_year + 1900, month = t->tm_mon + 1, day = t->tm_mday;
		hour = t->tm_hour, min = t->tm_min, sec = t->tm_sec, msec = 0;
#endif //WINCE
	}
	_eztime() { init();}
} eztime;

#endif // EZTIME_H
