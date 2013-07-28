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
#include "TokenManager.h"

#define TIMER_STOP_EXECUTION -1

namespace Utils
{
	struct STimerDispatchEvt {
		uint64 token;
		uint32 interval;
		uint32 creation_time;
		boost::function<uint32 (const STimerDispatchEvt&)> callback;
	};

	// Threadsafe
	class TimerDispatcher
	{
	public:
		TimerDispatcher() : m_mutex(), m_tokenMgr(), m_timers() {}

		STimerDispatchEvt AddTimer(uint32 interval, const boost::function<uint32 (const STimerDispatchEvt&)> &callback) {
			ScopedLock g(m_mutex);
			STimerDispatchEvt deleg = { m_tokenMgr.get(), interval, timeGetTime(), callback };
			m_timers[deleg.token] = deleg;
			return deleg;
		}

		void RemoveTimer(const STimerDispatchEvt &timer) {
			RemoveTimer(timer.token);
		}

		void RemoveTimer(const uint64 &token) {
			ScopedLock g(m_mutex);
			auto itr = m_timers.find(token);
			if (itr != m_timers.end()) {
				m_tokenMgr.remove(token);
				m_timers.erase(itr);
			}
		}

		void ClearTimers() {
			m_mutex.acquire();
			m_timers.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

		void DispatchTimers(uint32 time_now) {
			m_mutex.acquire();
			auto mTimers = m_timers;
			m_mutex.release();

			for (auto& pair: mTimers) {
				STimerDispatchEvt &evt = pair.second;
				if (evt.creation_time + evt.interval > time_now)
					continue;

				uint32 retval = evt.callback(evt);

				ScopedLock g(m_mutex);
				auto itr = m_timers.find(pair.first);
				if (itr != m_timers.end()) {
					STimerDispatchEvt *ref = &itr->second;
					if (ref->creation_time != evt.creation_time)
						continue;

					if (retval != TIMER_STOP_EXECUTION) {
						ref->interval = retval;
						ref->creation_time = time_now;
					}
					else {
						m_timers.erase(itr);
						m_tokenMgr.remove(pair.first);
					}
				}
			}
		}

	protected:
		Mutex m_mutex;
		TokenManager m_tokenMgr;
		std::map<uint64, STimerDispatchEvt> m_timers;
	};
}
