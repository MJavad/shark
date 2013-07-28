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
#include "Handle.h"

namespace Utils
{
	class Signal
	{
	public:
		Signal(bool manualReset = true, bool initState = false) :
			m_manualReset(manualReset) {
			m_event = CreateEventW(nullptr, manualReset, initState, nullptr);
			if (m_event.is_null())
				throw std::runtime_error("Could not create event!");
		}

		Signal(Signal &&other) :
			m_manualReset(other.m_manualReset),
			m_event(std::move(other.m_event)) {
			other.m_manualReset = true;
		}

		Signal& operator=(Signal &&other) {
			m_event = std::move(other.m_event);
			m_manualReset = other.m_manualReset;
			other.m_manualReset = true;
			return *this;
		}

		bool set() const {
			return SetEvent(m_event) != FALSE;
		}

		bool reset() const {
			return ResetEvent(m_event) != FALSE;
		}

		bool pulse() const {
			return PulseEvent(m_event) != FALSE;
		}

		DWORD wait(uint32 time = INFINITE) const {
			return WaitForSingleObject(m_event, time);
		}

		bool is_set() const {
			DWORD status = WaitForSingleObject(m_event, 0);
			if (status == WAIT_OBJECT_0 && !m_manualReset)
				SetEvent(m_event);

			return (status == WAIT_OBJECT_0);
		}

		bool is_manual_reset() const {
			return m_manualReset;
		}

	protected:
		Handle m_event;
		bool m_manualReset;
	};
}
