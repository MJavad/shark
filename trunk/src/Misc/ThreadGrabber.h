#pragma once

namespace Utils
{
	class Thread
	{
	public:
		Thread(uint32 id) : m_threadId(id), m_currentAccess(0),
			m_threadHandle(nullptr) {}

		~Thread() {
			if (m_threadHandle != nullptr)
				CloseHandle(m_threadHandle);
		}

		bool open(DWORD dwAccess) {
			if ((m_currentAccess & dwAccess) == dwAccess)
				return true;

			HANDLE hThread = OpenThread(dwAccess, FALSE, m_threadId);
			if (hThread != nullptr) {
				close();
				m_threadHandle = hThread;
				m_currentAccess = dwAccess;
			}

			return hThread != nullptr;
		}

		void close() {
			if (m_threadHandle != nullptr) {
				CloseHandle(m_threadHandle);
				m_threadHandle = nullptr;
				m_currentAccess = 0;
			}
		}

		DWORD access() const {
			return m_currentAccess;
		}

		HANDLE handle() const {
			return m_threadHandle;
		}

		uint32 id() const {
			return m_threadId;
		}

	private:
		uint32 m_threadId;
		HANDLE m_threadHandle;
		DWORD m_currentAccess;
	};

	class ThreadGrabber
	{
	public:
		bool update(uint32 pid) {
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
			if (hSnapshot == INVALID_HANDLE_VALUE)
				return false;

			m_threads.clear();
			THREADENTRY32 threadEntry = {0};
			threadEntry.dwSize = sizeof(THREADENTRY32);

			if (Thread32First(hSnapshot, &threadEntry) != FALSE) {
				do if (threadEntry.th32OwnerProcessID == pid)
					m_threads.push_back(std::make_shared<Thread>(threadEntry.th32ThreadID));

				while (Thread32Next(hSnapshot, &threadEntry) != FALSE);
			}

			CloseHandle(hSnapshot);
			return true;
		}

		const std::list<std::shared_ptr<Thread>>& threads() const {
			return m_threads;
		}

	private:
		std::list<std::shared_ptr<Thread>> m_threads;
	};
}
