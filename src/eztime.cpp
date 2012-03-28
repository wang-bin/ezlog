/******************************************************************************
    eztime.cpp: description
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

#include "eztime.h"

void getTime(eztime* t)
{
#ifdef Q_OS_WIN   //WINCE
	SYSTEMTIME stUTC;
	::GetSystemTime(&stUTC);
	t->year = stUTC.wYear, t->month = stUTC.wMonth, t->day = stUTC.wDay;
	t->hour = stUTC.wHour, t->min = stUTC.wMinute, t->sec = stUTC.wSecond;
	t->msec = stUTC.wMilliseconds;
#else
	time_t seconds = time(NULL);
	struct tm* tt = localtime(&seconds);
	t->year = tt->tm_year + 1900, t->month = tt->tm_mon + 1, t->day = tt->tm_mday;
	t->hour = tt->tm_hour, t->min = tt->tm_min, t->sec = tt->tm_sec, t->msec = 0;
#endif
}
