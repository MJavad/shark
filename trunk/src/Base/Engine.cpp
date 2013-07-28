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

#include "Misc/stdafx.h"
#include "Engine.h"
#include "UI/GUIManager.h"
#include "UI/ConsoleWindow.h"
#include "D3DManager.h"

void Engine::Initialize(HINSTANCE hInstance) {
	m_instance = hInstance;
	m_lastPulse = 0;
}

void Engine::OnPulse() {
	uint32 time_now = timeGetTime();
	uint32 time_diff = time_now - m_lastPulse;
	m_lastPulse = time_now;

	OnPulseEvent(time_now, time_diff);
	PulseTimer.DispatchTimers(time_now);
}

void Engine::InitializeEnvironment() {
	m_isInitialized = true;
	m_mainThreadId = GetCurrentThreadId();

	try {
		sGUIMgr.Initialize();
		sConsoleWindow.Initialize();
		sConsoleWindow.AddLine(L"D3DShark initialized!");
	}
	catch (std::exception &e) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		sLog.OutDebug(conv.from_bytes(e.what()));
	}
}

void Engine::OnShutDown() {
	sMemory.RemoveAllDetours();
	m_shutdownComplete.set();
}

void Engine::OnProcessDetach() {
	sD3DMgr.Shutdown();
}

void Engine::OnWindowDestroy() {
	sD3DMgr.ReleaseDeviceResources();
}

bool Engine::InitializeShutdown() const {
	DWORD dwThreadId = 0;
	Utils::Handle hThread = CreateThread(nullptr, 0,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(_shutdownThread),
		nullptr, 0, &dwThreadId);

	return !hThread.is_null();
}

// This is probably an unfixable race condition... Just wait a little.
// Also possible: WaitForMultipleObjects to wait until each working thread has finished
BOOL WINAPI Engine::_shutdownThread(LPVOID lpParam) {
	sEngine.m_shutdownEvent.set();
	sEngine.m_shutdownComplete.wait();

	Sleep(100);
	FreeLibraryAndExitThread(sEngine.GetInstance(), EXIT_SUCCESS);
}
