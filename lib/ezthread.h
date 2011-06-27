#ifndef EZTHREAD_H
#define EZTHREAD_H

#if CONFIG_THREAD_BOOST
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
typedef boost::mutex ezmutex;
typedef boost::mutex::scoped_lock ezscoped_lock;
#endif

unsigned long threadId();
long pid();

struct ezmutexprivate;
class ezmutex
{
public:
	ezmutex();
	~ezmutex();

	void lock();
	void unlock();
private:
	struct ezmutexprivate *d;
	ezmutex(const ezmutex&);
	ezmutex& operator = (const ezmutex&);
};

class ezscoped_lock
{
public:
	inline ezscoped_lock(ezmutex& pMutex):
		mutex(pMutex) {
		mutex.lock();
	}

	inline ~ezscoped_lock() {
		mutex.unlock();
	}
private:
	ezmutex& mutex;
};

#endif // EZTHREAD_H
