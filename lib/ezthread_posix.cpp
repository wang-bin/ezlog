#ifndef EZTHREAD_POSIX_H
#define EZTHREAD_POSIX_H

#include "ezthread.h"
#include <pthread.h>
#include <unistd.h>

/*
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
pid_t tid = (pid_t) syscall (SYS_gettid);
*/
unsigned long threadId()
{
	return pthread_self();
}

long pid()
{
	return getpid();
}

struct EZMutexPrivate {
	pthread_mutex_t mutex;
};

EZMutex::EZMutex()
:d(new EZMutexPrivate)
{
	pthread_mutex_init(&d->mutex, NULL);
}

EZMutex::~EZMutex()
{
	pthread_mutex_destroy(&d->mutex);
	delete d;
}

void EZMutex::lock()
{
	pthread_mutex_lock(&d->mutex);
}

void EZMutex::unlock()
{
	pthread_mutex_destroy(&d->mutex);
}

#endif // EZTHREAD_POSIX_H
