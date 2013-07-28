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
#include "DllEntry.h"
#include "Offsets.h"
#include "Delegates.h"
#include "Detours.h"
#include "WndProc.h"
#include "D3DManager.h"
#include "Engine.h"
#include "ExceptionManager.h"
#include "FileManager.h"

#ifdef DEBUG_USE_LOG
#include <io.h>
#include <fcntl.h>

// Creates a console window and redirects output (std::cout etc. to it)
void SetupDebugConsole() {
	AllocConsole();
	SetConsoleTitleW(L"Debug Console");

	// set stdout
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int nOutputHandle = _open_osfhandle(reinterpret_cast<intptr_t>(hOutput), _O_TEXT);
	*stdout = *_fdopen(nOutputHandle, "w");
	setvbuf(stdout, nullptr, _IONBF, 0);

	// set stdin
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	int nInputHandle = _open_osfhandle(reinterpret_cast<intptr_t>(hInput), _O_TEXT);
	*stdin = *_fdopen(nInputHandle, "r");
	setvbuf(stdin, nullptr, _IONBF, 0);

	// set sterr
	HANDLE hError = GetStdHandle(STD_ERROR_HANDLE);
	int nErrorHandle = _open_osfhandle(reinterpret_cast<intptr_t>(hError), _O_TEXT);
	*stderr = *_fdopen(nErrorHandle, "w");
	setvbuf(stderr, nullptr, _IONBF, 0);

	std::ios::sync_with_stdio();
}
#endif

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#ifdef DEBUG_USE_LOG
		SetupDebugConsole();
#endif

		DisableThreadLibraryCalls(hInstance);
		sEngine.Initialize(hInstance);
		sFileMgr.Initialize();
		sExceptionMgr.Initialize();
		break;

	case DLL_PROCESS_DETACH:
		sEngine.OnProcessDetach();

#ifdef DEBUG_USE_LOG
		FreeConsole();
#endif
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

	LOG_DEBUG("Initialized!");
	return EXIT_SUCCESS;
}

// EXPORTS: Unload
BOOL WINAPI UnloadInternal(LPVOID lpParam) {
	if (!sEngine.IsShuttingDown())
		sEngine.InitializeShutdown();

	return EXIT_SUCCESS;
}
