/******************************************************************************
	ezmutex.h: description
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


#ifndef EZMUTEX_H
#define EZMUTEX_H
/*
	TODO: use local static mutex instead of gloabl mutex. use macros
	#define EZMUTEX_LOCK() static ezmutex mutex; _ezmutex_init(&mutex); _ezmutex_lock(&mutex)
*/

#include "ezlog_global.h"

#if !COMPILER(MSVC) || CONFIG_PTHREAD
#define USE_PTHREAD
#include <pthread.h>
typedef pthread_mutex_t ezmutex;
#else
#define USE_CRITICAL_SECTION
#include <windows.h>
typedef CRITICAL_SECTION ezmutex;
#endif

static ezmutex g_mutex = PTHREAD_MUTEX_INITIALIZER;

static ALWAYS_INLINE void _ezmutex_init()
{
#ifdef USE_CRITICAL_SECTION
	InitializeCriticalSection(g_mutex)
#endif //USE_CRITICAL_SECTION
}

static ALWAYS_INLINE void _ezmutex_lock()
{
#ifdef USE_PTHREAD
	pthread_mutex_lock(&g_mutex);
#else
	EnterCriticalSection(&g_mutex);
#endif
}

static ALWAYS_INLINE void _ezmutex_unlock()
{
#ifdef USE_PTHREAD
	pthread_mutex_unlock(&g_mutex);
#else
	LeaveCriticalSection(&g_mutex);
#endif
}

#endif // EZMUTEX_H
