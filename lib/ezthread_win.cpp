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

struct ezmutexprivate {
	CRITICAL_SECTION _criticalSection;
};

ezmutex::ezmutex()
:d(new ezmutexprivate)
{
	InitializeCriticalSection(&d->_criticalSection);
}

ezmutex::~ezmutex()
{
	DeleteCriticalSection(&d->_criticalSection);
	delete d;
}

void ezmutex::lock()
{
	EnterCriticalSection(&d->_criticalSection);
}

void ezmutex::unlock()
{
	LeaveCriticalSection(&d->_criticalSection);
}


#endif // EZTHREAD_WIN_H
