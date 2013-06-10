#include "Misc/stdafx.h"
#include "DebugHelper.h"
#pragma comment(lib, "Version.lib")

namespace Utils
{
	void DebugHelper::LoadDbgHelp() {
		if (m_dbgHelp != nullptr)
			return;

		m_dbgHelp = GetModuleHandleW(L"dbghelp.dll");
		m_alreadyLoaded = m_dbgHelp != nullptr;

		m_dbgHelp = LoadLibraryW(L"dbghelp.dll");
		if (m_dbgHelp != nullptr) {
			m_stackWalk = reinterpret_cast<tStackWalk64>
				(GetProcAddress(m_dbgHelp, "StackWalk64"));

			m_symInitialize = reinterpret_cast<tSymInitializeW>
				(GetProcAddress(m_dbgHelp, "SymInitializeW"));

			m_symSetOptions = reinterpret_cast<tSymSetOptions>
				(GetProcAddress(m_dbgHelp, "SymSetOptions"));

			m_symFromAddr = reinterpret_cast<tSymFromAddrW>
				(GetProcAddress(m_dbgHelp, "SymFromAddrW"));

			m_symFunctionTableAccess = reinterpret_cast<tSymFunctionTableAccess64>
				(GetProcAddress(m_dbgHelp, "SymFunctionTableAccess64"));

			m_symGetModuleBase = reinterpret_cast<tSymGetModuleBase64>
				(GetProcAddress(m_dbgHelp, "SymGetModuleBase64"));

			m_symCleanup = reinterpret_cast<tSymCleanup>
				(GetProcAddress(m_dbgHelp, "SymCleanup"));

			if (m_stackWalk != nullptr &&
				m_symInitialize != nullptr &&
				m_symSetOptions != nullptr &&
				m_symFromAddr != nullptr &&
				m_symFunctionTableAccess != nullptr &&
				m_symGetModuleBase != nullptr &&
				m_symCleanup != nullptr)
			{
				HANDLE hProcess = GetCurrentProcess();
				if (m_alreadyLoaded)
					m_symCleanup(hProcess);

				m_symSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
				if (m_symInitialize(hProcess, nullptr, TRUE) == TRUE)
					return;
			}

			FreeLibrary(m_dbgHelp);
			m_dbgHelp = nullptr;
		}

		throw std::runtime_error("Could not load dbghelp.dll!");
	}

	std::wstring DebugHelper::DumpMemory(void *pMemory, uint32 dwSize) const {
		Utils::ByteBuffer memoryDump(dwSize);
		SIZE_T dwNumberOfBytesRead = 0;

		if (ReadProcessMemory(GetCurrentProcess(), pMemory,
				memoryDump.data(), dwSize, &dwNumberOfBytesRead) == FALSE)
			throw std::runtime_error("Invalid memory location for dump!");

		memoryDump.resize(dwNumberOfBytesRead);
		DWORD_PTR dwStartAddress = reinterpret_cast<DWORD_PTR>(pMemory);

		std::wostringstream strmMemory;
		for (DWORD_PTR j = 0; j < dwNumberOfBytesRead; j += 0x10) {
			strmMemory << std::hex << std::uppercase
					   << L"  0x" << reinterpret_cast<void*>(dwStartAddress + j)
					   << L':';

			for (DWORD_PTR i = 0; i < 0x10 && (i + j < dwNumberOfBytesRead); ++i) {
				if ((i % 4) == 0)
					strmMemory << L' ';

				strmMemory << std::hex << std::setfill(L'0') << std::setw(2)
						   << memoryDump[j + i] << L' ';
			}

			strmMemory << L' ';

			for (DWORD_PTR i = 0; i < 0x10 && (i + j < dwNumberOfBytesRead); ++i) {
				char c = static_cast<char>(memoryDump[j + i]);
				strmMemory << (isprint(c) ? wchar_t(c) : L'.');
			}

			strmMemory << L"\r\n";
		}

		return strmMemory.str();
	}

	std::wstring DebugHelper::DumpModules(uint32 processId) const {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Could not create module snapshot!");

		std::wostringstream strmModules;
		MODULEENTRY32W moduleEntry = {0};
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32FirstW(hSnapshot, &moduleEntry) != FALSE) {
			do if (moduleEntry.th32ProcessID == processId) {
				strmModules << L"  0x" << std::hex << std::uppercase
							<< reinterpret_cast<void*>(moduleEntry.modBaseAddr) << L": "
							<< (moduleEntry.szModule != nullptr ? moduleEntry.szModule : L"<null>");

				if (moduleEntry.szExePath != nullptr) {
					DWORD dwVersionInfoHandle = 0;
					DWORD dwVersionInfoSize = GetFileVersionInfoSizeW(moduleEntry.szExePath, &dwVersionInfoHandle);

					UINT fileInfoSize = 0;
					VS_FIXEDFILEINFO *pFileInfo = nullptr;
					Utils::ByteBuffer versionInfoBuffer(dwVersionInfoSize);

					if (dwVersionInfoSize != 0 &&

						GetFileVersionInfoW(moduleEntry.szExePath, dwVersionInfoHandle,
							dwVersionInfoSize, versionInfoBuffer.data()) != FALSE &&

						VerQueryValueW(versionInfoBuffer.data(), L"\\",
							reinterpret_cast<LPVOID*>(&pFileInfo), &fileInfoSize) != FALSE &&

						fileInfoSize >= sizeof(VS_FIXEDFILEINFO))
					{
						// Output file version string...
						strmModules << L" (" << std::dec
									<< HIWORD(pFileInfo->dwFileVersionMS) << L'.'
									<< LOWORD(pFileInfo->dwFileVersionMS) << L'.'
									<< HIWORD(pFileInfo->dwFileVersionLS) << L'.'
									<< LOWORD(pFileInfo->dwFileVersionLS) << L')';
					}
				}

				strmModules << L"\r\n";
			}
			while (Module32NextW(hSnapshot, &moduleEntry) != FALSE);
		}

		CloseHandle(hSnapshot);
		return strmModules.str();
	}

	std::wstring DebugHelper::DumpCallStack(HANDLE hThread, PCONTEXT pContext) const {
		assert(m_dbgHelp != nullptr);
		DWORD64 dwDisplacement = 0;
		HANDLE hProcess = GetCurrentProcess();

		if (hThread == nullptr)
			hThread = GetCurrentThread();

		CONTEXT threadContext = {0};
		if (pContext != nullptr)
			threadContext = *pContext;
		else
			RtlCaptureContext(&threadContext);

		STACKFRAME64 stackFrame64 = {0};
		stackFrame64.AddrPC.Offset = threadContext.Eip;
		stackFrame64.AddrPC.Mode = AddrModeFlat;
		stackFrame64.AddrStack.Offset = threadContext.Esp;
		stackFrame64.AddrStack.Mode = AddrModeFlat;
		stackFrame64.AddrFrame.Offset = threadContext.Ebp;
		stackFrame64.AddrFrame.Mode = AddrModeFlat;

		std::wostringstream callStack;
		while (true) {
			BOOL result = m_stackWalk(IMAGE_FILE_MACHINE_I386,
									  hProcess,
									  hThread,
									  &stackFrame64,
									  &threadContext,
									  nullptr,
									  m_symFunctionTableAccess,
									  m_symGetModuleBase,
									  nullptr);

			if (result != TRUE)
				break;

			callStack << L"  0x" << std::hex << std::uppercase
					  << reinterpret_cast<void*>(stackFrame64.AddrPC.Offset) << L": ";

			ByteBuffer buffer(sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(wchar_t));
			PSYMBOL_INFOW pSymbol = reinterpret_cast<PSYMBOL_INFOW>(buffer.data());
			pSymbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
			pSymbol->MaxNameLen = MAX_SYM_NAME;

			if ((m_symFromAddr(hProcess,
							  stackFrame64.AddrPC.Offset,
							  &dwDisplacement,
							  pSymbol) == TRUE)
				 && pSymbol->Name != nullptr)
				callStack << pSymbol->Name << L" + 0x" << std::hex << std::uppercase << dwDisplacement;
			else
				callStack << L"<Unknown Symbol>";

			callStack << L"\r\n";
		}

		return callStack.str();
	}

	std::wstring DebugHelper::FormatSystemCode(DWORD dwCode, DWORD dwLanguageId) const {
		LPWSTR lpBuffer = nullptr;
		std::wstring swMessage(L"Unknown system code!");
		DWORD dwLength = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
										nullptr,
										dwCode,
										dwLanguageId,
										reinterpret_cast<LPWSTR>(&lpBuffer),
										0,
										nullptr);

		if (lpBuffer != nullptr) {
			if (dwLength != 0)
				swMessage = lpBuffer;

			LocalFree(lpBuffer);
		}

		return swMessage;
	}
}
