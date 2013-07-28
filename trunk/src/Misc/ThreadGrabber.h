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
	class Thread
	{
	public:
		Thread(uint32 id) : m_threadId(id),
			m_currentAccess(0) {}

		bool open(DWORD access) {
			if ((m_currentAccess & access) == access)
				return true;

			HANDLE hThread = OpenThread(access, FALSE, m_threadId);
			if (hThread != nullptr) {
				close();
				m_threadHandle = hThread;
				m_currentAccess = access;
			}

			return hThread != nullptr;
		}

		void close() {
			m_threadHandle.close();
			m_currentAccess = 0;
		}

		DWORD access() const {
			return m_currentAccess;
		}

		Handle handle() const {
			return m_threadHandle;
		}

		uint32 id() const {
			return m_threadId;
		}

		bool suspend() const {
			return SuspendThread(m_threadHandle) != -1;
		}

		bool resume() const {
			return ResumeThread(m_threadHandle) != -1;
		}

	private:
		uint32 m_threadId;
		Handle m_threadHandle;
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
			threadEntry.dwSize = sizeof(threadEntry);

			if (Thread32First(hSnapshot, &threadEntry) != FALSE) {
				do if (threadEntry.th32OwnerProcessID == pid)
					m_threads.push_back(boost::make_shared<Thread>(threadEntry.th32ThreadID));

				while (Thread32Next(hSnapshot, &threadEntry) != FALSE);
			}

			CloseHandle(hSnapshot);
			return true;
		}

		const std::list<boost::shared_ptr<Thread>>& threads() const {
			return m_threads;
		}

	private:
		std::list<boost::shared_ptr<Thread>> m_threads;
	};
}
