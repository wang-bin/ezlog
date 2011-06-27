#ifndef EZTHREAD_H
#define EZTHREAD_H

#if CONFIG_THREAD_BOOST
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
typedef boost::mutex EZMutex;
typedef boost::mutex::scoped_lock EZScopedLock;
#endif

unsigned long threadId();
long pid();

struct EZMutexPrivate;
class EZMutex
{
public:
	EZMutex();
	~EZMutex();

	void lock();
	void unlock();
private:
	struct EZMutexPrivate *d;
	EZMutex(const EZMutex&);
	EZMutex& operator = (const EZMutex&);
};

class EZScopedLock
{
public:
	inline EZScopedLock(EZMutex& pMutex):
		mutex(pMutex) {
		mutex.lock();
	}

	inline ~EZScopedLock() {
		mutex.unlock();
	}
private:
	EZMutex& mutex;
};

#endif // EZTHREAD_H
