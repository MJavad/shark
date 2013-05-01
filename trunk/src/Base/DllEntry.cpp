#include "Misc/stdafx.h"
#include "DllEntry.h"
#include "Offsets.h"
#include "Delegates.h"
#include "Detours.h"
#include "WndProc.h"
#include "D3DManager.h"
#include "UI/GUIManager.h"
#include "Engine.h"

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		sEngine->Initialize(hInstance);
		break;

	case DLL_PROCESS_DETACH:
		sEngine->OnProcessDetach();
		break;
	}

	return TRUE;
}

// EXPORTS: Load
BOOL WINAPI LoadInternal(LPVOID lpParam) {
	srand(timeGetTime());

	try {
		sLog->Initialize();
		sMemory->Initialize();
		sOffsets->Initialize();
		sDelegates->Initialize();
		sWndProc->Initialize();
		sD3DMgr->Initialize();
		sDetours->Initialize();
	}
	catch (std::exception &e) {
		MessageBoxA(nullptr, e.what(), "ERROR", MB_OK);
	}

	//LOG_MESSAGE("Initialized!");
	return EXIT_SUCCESS;
}

// EXPORTS: Unload
BOOL WINAPI UnloadInternal(LPVOID lpParam) {
	sEngine->InitializeShutdown();
	return EXIT_SUCCESS;
}
