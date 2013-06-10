#pragma once
#include <DbgHelp.h>

typedef BOOL (WINAPI *tStackWalk64) (_In_ DWORD MachineType,
									 _In_ HANDLE hProcess,
									 _In_ HANDLE hThread,
									 _Inout_ LPSTACKFRAME64 StackFrame,
									 _Inout_ PVOID ContextRecord,
									 _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
									 _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
									 _In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
									 _In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);

typedef BOOL (WINAPI *tSymInitializeW) (_In_ HANDLE hProcess,
										_In_opt_ PCWSTR UserSearchPath,
										_In_ BOOL fInvadeProcess);

typedef DWORD (WINAPI *tSymSetOptions) (_In_ DWORD SymOptions);

typedef BOOL (WINAPI *tSymFromAddrW) (_In_ HANDLE hProcess,
									  _In_ DWORD64 Address,
									  _Out_opt_ PDWORD64 Displacement,
									  _Inout_ PSYMBOL_INFOW Symbol);

typedef PVOID (WINAPI *tSymFunctionTableAccess64) (_In_ HANDLE hProcess,
												   _In_ DWORD64 AddrBase);

typedef DWORD64 (WINAPI *tSymGetModuleBase64) (_In_ HANDLE hProcess,
											   _In_ DWORD64 dwAddr);

typedef BOOL (WINAPI *tSymCleanup) (_In_ HANDLE hProcess);

namespace Utils
{
	class DebugHelper
	{
	public:
		DebugHelper() : m_dbgHelp(nullptr),
						m_alreadyLoaded(false),
						m_stackWalk(nullptr),
						m_symInitialize(nullptr),
						m_symSetOptions(nullptr),
						m_symFromAddr(nullptr),
						m_symFunctionTableAccess(nullptr),
						m_symGetModuleBase(nullptr),
						m_symCleanup(nullptr) {}

		~DebugHelper() {
			Cleanup();
		}

		void Cleanup() {
			if (m_dbgHelp != nullptr) {
				if (!m_alreadyLoaded && m_symCleanup != nullptr)
					m_symCleanup(GetCurrentProcess());

				FreeLibrary(m_dbgHelp);
				m_dbgHelp = nullptr;
			}
		}

		void LoadDbgHelp();
		std::wstring DumpModules(uint32 processId) const;
		std::wstring DumpMemory(void *pMemory, uint32 dwSize) const;
		std::wstring DumpCallStack(HANDLE hThread = 0, PCONTEXT pContext = nullptr) const;
		std::wstring FormatSystemCode(DWORD dwCode,
			DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)) const;

	protected:
		HMODULE m_dbgHelp;
		bool m_alreadyLoaded;
		tStackWalk64 m_stackWalk;
		tSymInitializeW m_symInitialize;
		tSymSetOptions m_symSetOptions;
		tSymFromAddrW m_symFromAddr;
		tSymFunctionTableAccess64 m_symFunctionTableAccess;
		tSymGetModuleBase64 m_symGetModuleBase;
		tSymCleanup m_symCleanup;
	};
}
