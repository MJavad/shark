#include "Misc/stdafx.h"
#include "DllEntry.h"
#include "Offsets.h"
#include "Delegates.h"
#include "Detours.h"
#include "WndProc.h"
#include "D3DManager.h"
#include "Engine.h"
#include "ExceptionManager.h"
#include "FileManager.h"

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		sEngine.Initialize(hInstance);
		sFileMgr.Initialize();
		sExceptionMgr.Initialize();
		break;

	case DLL_PROCESS_DETACH:
		sEngine.OnProcessDetach();
		break;
	}

	return TRUE;
}

// EXPORTS: Load
BOOL WINAPI LoadInternal(LPVOID lpParam) {
	srand(timeGetTime());

	sLog.Initialize();
	sOffsets.Initialize();
	sDelegates.Initialize();
	sMemory.Initialize();
	sWndProc.Initialize();
	sD3DMgr.Initialize();
	sDetours.Initialize();

	//LOG_DEBUG(L"Initialized!");
	return EXIT_SUCCESS;
}

// EXPORTS: Unload
BOOL WINAPI UnloadInternal(LPVOID lpParam) {
	if (!sEngine.IsShuttingDown())
		sEngine.InitializeShutdown();

	return EXIT_SUCCESS;
}
