/******************************************************************************
    ezmutex.c: description
    Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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


ALWAYS_INLINE void _ezmutex_lock()
{
#ifdef USE_PTHREAD
    pthread_mutex_lock(&g_mutex);
#else
    EnterCriticalSection(&g_mutex);
#endif
}

ALWAYS_INLINE void _ezmutex_unlock()
{
#ifdef USE_PTHREAD
    pthread_mutex_unlock(&g_mutex);
#else
    LeaveCriticalSection(&g_mutex);
#endif
}
