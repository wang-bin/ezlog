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

#include "ezmutex.h"
#include "prepost.h"

#if COMPILER(MSVC)
#define USE_CRITICAL_SECTION
#include <windows.h>
typedef CRITICAL_SECTION ezmutex;
static ezmutex g_mutex;
/* use pthread: *nix, g++. For windows with vc, no pthread*/
#else //if COMPILER(GCC) || !defined(Q_OS_WIN) /*|| CONFIG_PTHREAD*/
#define USE_PTHREAD
#include <pthread.h>
typedef pthread_mutex_t ezmutex;
static ezmutex g_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif


#if COMPILER(MSVC)
PRE_FUNC_ADD(InitializeCriticalSection, &g_mutex)
#endif


void _ezmutex_lock()
{
#ifdef USE_PTHREAD
    pthread_mutex_lock(&g_mutex);
#else
    EnterCriticalSection(&g_mutex);
#endif
}

void _ezmutex_unlock()
{
#ifdef USE_PTHREAD
    pthread_mutex_unlock(&g_mutex);
#else
    LeaveCriticalSection(&g_mutex);
#endif
}
