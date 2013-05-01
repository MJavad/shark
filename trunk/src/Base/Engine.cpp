#include "Misc/stdafx.h"
#include "Engine.h"
#include "UI/GUIManager.h"

INIT_SINGLETON(Engine);

bool Engine::s_isInitialized = false;

void Engine::Initialize(HINSTANCE hInstance) {
	m_instance = hInstance;
}

void Engine::InitializeEnvironment() {
	s_isInitialized = true;

	try {
		sGUIMgr->Initialize();
	}
	catch (std::exception &e) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		sLog->OutDebug(conv.from_bytes(e.what()));
	}
}

void Engine::OnShutDown() {
	sMemory->RemoveAllDetours();
	m_shutdownComplete.set();
}

BOOL WINAPI Engine::_shutdownThread(LPVOID lpParam) {
	sEngine->m_shutdownEvent.set();
	sEngine->m_shutdownComplete.wait();
	FreeLibraryAndExitThread(sEngine->m_instance, EXIT_SUCCESS);
}
