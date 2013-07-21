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

	class LockGuard
	{
	public:
		LockGuard(Mutex &mutex) :
			m_mutex(&mutex) {
			m_mutex->acquire();
		}

		LockGuard(const LockGuard &other) :
			m_mutex(other.m_mutex) {
			m_mutex->acquire();
		}

		~LockGuard() {
			m_mutex->release();
		}

	protected:
		Mutex* m_mutex;

		LockGuard& operator=(const LockGuard&);
	};

	inline void Mutex::lock(const boost::function<void ()> &func) {
		LockGuard g(*this);
		func();
	}
}
