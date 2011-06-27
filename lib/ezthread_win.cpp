#ifndef EZTHREAD_WIN_H
#define EZTHREAD_WIN_H

#include "ezthread.h"
#include <windows.h>
#include <process.h>

unsigned long threadId()
{
	return GetCurrentThreadId();
}

long pid()
{
	return _getpid();
}

struct EZMutexPrivate {
	CRITICAL_SECTION _criticalSection;
};

EZMutex::EZMutex()
:d(new EZMutexPrivate)
{
	InitializeCriticalSection(&d->_criticalSection);
}

EZMutex::~EZMutex()
{
	DeleteCriticalSection(&d->_criticalSection);
	delete d;
}

void EZMutex::lock()
{
	EnterCriticalSection(&d->_criticalSection);
}

void EZMutex::unlock()
{
	LeaveCriticalSection(&d->_criticalSection);
}


#endif // EZTHREAD_WIN_H
