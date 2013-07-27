#pragma once

namespace Utils
{
	class Mutex
	{
	public:
		Mutex() : m_critSect() {
			InitializeCriticalSection(&m_critSect);
		}

		~Mutex() {
			DeleteCriticalSection(&m_critSect);
		}

		void acquire() {
			EnterCriticalSection(&m_critSect);
		}

		void release() {
			LeaveCriticalSection(&m_critSect);
		}

		bool try_acquire() {
			return TryEnterCriticalSection(&m_critSect) != FALSE;
		}

		void lock(const boost::function<void ()> &func);

	protected:
		CRITICAL_SECTION m_critSect;

		Mutex(const Mutex&);
		Mutex& operator=(const Mutex&);
	};

	class ScopedLock
	{
	public:
		ScopedLock(Mutex &mutex) :
			m_mutex(&mutex) {
			m_mutex->acquire();
		}

		ScopedLock(const ScopedLock &other) :
			m_mutex(other.m_mutex) {
			m_mutex->acquire();
		}

		~ScopedLock() {
			m_mutex->release();
		}

	protected:
		Mutex* m_mutex;

		ScopedLock& operator=(const ScopedLock&);
	};

	inline void Mutex::lock(const boost::function<void ()> &func) {
		ScopedLock g(*this);
		func();
	}
}
