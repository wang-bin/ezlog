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
/* use pthread: *nix, g++. For windows with vc, no pthread*/
#if COMPILER(GCC) || !defined(Q_OS_WIN) /*|| CONFIG_PTHREAD*/
#define USE_PTHREAD
#include <pthread.h>
typedef pthread_mutex_t ezmutex;
static ezmutex g_mutex = PTHREAD_MUTEX_INITIALIZER;
#else
#define USE_CRITICAL_SECTION
#include <windows.h>
typedef CRITICAL_SECTION ezmutex;
static ezmutex g_mutex;

static void __cdecl _ezmutex_init()
{
	InitializeCriticalSection(&g_mutex);
}
/*static int _ezmutex_ok = _ezmutex_init();*/
#pragma section(".CRT$XIC",long,read)
#define _CRTALLOC(x) __declspec(allocate(x))

_CRTALLOC(".CRT$XIC") static void (*pinit)() = _ezmutex_init;
/*
#pragma data_seg(".CRT$XIU")
static int autostart[] = { _ezmutex_init };
*/
#endif




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
