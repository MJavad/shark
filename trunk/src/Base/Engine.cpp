#include "Misc/stdafx.h"
#include "Engine.h"
#include "UI/GUIManager.h"
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
	}
	catch (std::exception &e) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		sLog.OutDebug(conv.from_bytes(e.what()));
	}
}

void Engine::OnShutDown() {
	sD3DMgr.Shutdown();
	sMemory.RemoveAllDetours();
	m_shutdownComplete.set();
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

	Sleep(10);
	FreeLibraryAndExitThread(sEngine.GetInstance(), EXIT_SUCCESS);
}
