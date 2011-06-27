/******************************************************************************
	ezthread: a tiny thread wrapper for c++
	Copyright (C) 2011 Wang Bin <wbsecg1@gmail.com>

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
	inline ezscoped_lock(ezmutex& pMutex): mutex(pMutex) { mutex.lock();}
	inline ~ezscoped_lock() { mutex.unlock();}
private:
	ezmutex& mutex;
};

#endif // EZTHREAD_H
