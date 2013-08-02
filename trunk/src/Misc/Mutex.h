/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

		void lock(const std::function<void ()> &func);

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

	inline void Mutex::lock(const std::function<void ()> &func) {
		ScopedLock g(*this);
		func();
	}
}
