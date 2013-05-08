#pragma once

class Engine : public Utils::Singleton<Engine>
{
public:
	void Initialize(HINSTANCE hInstance);
	void InitializeEnvironment();
	void OnShutDown();
	void OnPulse();

	void OnProcessDetach() {
		s_isInitialized = false;
	}

	// Must be static, because of access
	// from destructors, when the dll unloads
	static bool IsInitialized() {
		return s_isInitialized;
	}

	void InitializeShutdown() const {
		DWORD dwThreadId = 0;
		Utils::Handle hThread = CreateThread(nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(_shutdownThread),
			nullptr, 0, &dwThreadId);
	}

	bool IsShuttingDown() const {
		return m_shutdownEvent.is_set();
	}

	HINSTANCE GetInstance() const {
		return m_instance;
	}

	// Events and Timers...
	Utils::TimerDispatcher PulseTimer;
	Utils::Event<void (uint32, uint32)> OnPulseEvent;

protected:
	HINSTANCE m_instance;
	static bool s_isInitialized;
	Utils::Signal m_shutdownEvent;
	Utils::Signal m_shutdownComplete;
	uint32 m_lastPulse;

	static BOOL WINAPI _shutdownThread(LPVOID lpParam);
};

#define sEngine Utils::Singleton<Engine>::Instance()
