#include "Misc/stdafx.h"
#include "DebugHelper.h"

namespace Utils
{
	DebugHelper::DebugHelper() {
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

			m_undecorateSymbolName = reinterpret_cast<tUnDecorateSymbolNameW>
				(GetProcAddress(m_dbgHelp, "UnDecorateSymbolNameW"));

			m_symCleanup = reinterpret_cast<tSymCleanup>
				(GetProcAddress(m_dbgHelp, "SymCleanup"));

			if (m_stackWalk != nullptr &&
				m_symInitialize != nullptr &&
				m_symSetOptions != nullptr &&
				m_symFromAddr != nullptr &&
				m_symFunctionTableAccess != nullptr &&
				m_symGetModuleBase != nullptr &&
				m_undecorateSymbolName != nullptr &&
				m_symCleanup != nullptr)
			{
				m_symSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
				if (m_symInitialize(GetCurrentProcess(), nullptr, FALSE) != FALSE)
					return;
			}

			FreeLibrary(m_dbgHelp);
			m_dbgHelp = nullptr;
		}

		throw std::exception("Could not create DebugHelper instance!");
	}

	DebugHelper::~DebugHelper() {
		if (m_dbgHelp != nullptr) {
			m_symCleanup(GetCurrentProcess());
			FreeLibrary(m_dbgHelp);
		}
	}

	std::wstring DebugHelper::DumpCallStack(HANDLE hThread, PCONTEXT pContext) const {
		DWORD64 dwDisplacement = 0;
		HANDLE hProcess = GetCurrentProcess();

		if (hThread == nullptr)
			hThread = GetCurrentThread();

		CONTEXT threadContext = {0};
		if (pContext == nullptr) {
			pContext = &threadContext;
			RtlCaptureContext(pContext);
		}

		STACKFRAME64 stackFrame64 = {0};
		stackFrame64.AddrPC.Offset = pContext->Eip;
		stackFrame64.AddrPC.Mode = AddrModeFlat;
		stackFrame64.AddrStack.Offset = pContext->Esp;
		stackFrame64.AddrStack.Mode = AddrModeFlat;
		stackFrame64.AddrFrame.Offset = pContext->Ebp;
		stackFrame64.AddrFrame.Mode = AddrModeFlat;

		std::wostringstream callStack;
		while (true) {
			BOOL result = m_stackWalk(IMAGE_FILE_MACHINE_I386,
									  hProcess,
									  hThread,
									  &stackFrame64,
									  pContext,
									  nullptr,
									  m_symFunctionTableAccess,
									  m_symGetModuleBase,
									  nullptr);

			if (result != TRUE)
				break;

			callStack << L"   0x" << std::hex << std::uppercase
					  << reinterpret_cast<void*>(stackFrame64.AddrPC.Offset) << L" - ";

			ByteBuffer buffer(sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(wchar_t));
			PSYMBOL_INFOW pSymbol = reinterpret_cast<PSYMBOL_INFOW>(buffer.data());
			pSymbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
			pSymbol->MaxNameLen = MAX_SYM_NAME;

			if (m_symFromAddr(hProcess,
							  stackFrame64.AddrPC.Offset,
							  &dwDisplacement,
							  pSymbol) != FALSE)
				callStack << pSymbol->Name;
			else
				callStack << L"<Unknown Symbol>";

			callStack << L" + 0x" << std::hex << std::uppercase << dwDisplacement << L"\r\n";
		}

		return callStack.str();
	}
}
