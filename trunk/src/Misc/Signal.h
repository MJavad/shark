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
