#include "Misc/stdafx.h"
#include "ExceptionManager.h"
#include "Engine.h"
#include "WndProc.h"
#include "FileManager.h"
#include "Misc/DebugHelper.h"
#include "../resource.h"
#include <Commctrl.h>

__declspec(naked) LPTOP_LEVEL_EXCEPTION_FILTER WINAPI
	_registerExceptionFilter(_In_ LPTOP_LEVEL_EXCEPTION_FILTER) {
	__asm {
		push ebp
		mov ebp, esp
		mov eax, [SetUnhandledExceptionFilter]
		add eax, 5
		jmp eax
	}
}

void ExceptionManager::Initialize() {
	_registerExceptionFilter(_filter);
}

ExceptionManager::~ExceptionManager() {
	_registerExceptionFilter(nullptr);
}

BOOL CALLBACK ExceptionManager::_dlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND hShutdown = GetDlgItem(hwndDlg, IDC_LBL_SHUTDOWN);
			HWND hReason = GetDlgItem(hwndDlg, IDC_LBL_REASON);
			HWND hErrout = GetDlgItem(hwndDlg, IDC_LBL_ERROUT);

			HFONT hFont8 = CreateFont(13, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				FF_DONTCARE, TEXT("Segoe UI"));

			if (hFont8 != nullptr) {
				SendMessage(hReason, WM_SETFONT, reinterpret_cast<WPARAM>(hFont8), FALSE);
				SendMessage(hErrout, WM_SETFONT, reinterpret_cast<WPARAM>(hFont8), FALSE);
			}

			HFONT hFont18 = CreateFont(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				FF_DONTCARE, TEXT("Segoe UI"));

			if (hFont18 != nullptr)
				SendMessage(hShutdown, WM_SETFONT, reinterpret_cast<WPARAM>(hFont18), FALSE);

			HICON hErrIcon = LoadIcon(nullptr, IDI_ERROR);
			HWND hIconStatic = GetDlgItem(hwndDlg, IDC_ERRPICBOX);
			if (hErrIcon != nullptr)
				SendMessage(hIconStatic, STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(hErrIcon));

			HICON hExclamation = LoadIcon(nullptr, IDI_EXCLAMATION);
			if (hExclamation != nullptr)
				SendMessage(hwndDlg, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hExclamation));
		}
		return TRUE;

	case WM_CLOSE:
		DestroyWindow(hwndDlg);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}

	return FALSE;
}

#define EXCEPTION_MSVC 0xE06D7363
#define SEH_CASE(code) case code: msgStrm << L#code; break;

LONG WINAPI ExceptionManager::_filter(PEXCEPTION_POINTERS pInfo)
{
	Utils::ThreadGrabber threadGrabber;
	threadGrabber.update(GetCurrentProcessId());
	uint32 currentThreadId = GetCurrentThreadId();

	HWND hGameWindow = sWndProc.FindCurrentWindow();
	if (hGameWindow != nullptr) {
		if (sEngine.GetMainThreadId() != currentThreadId)
			ShowWindowAsync(hGameWindow, SW_HIDE);
		else
			ShowWindow(hGameWindow, SW_HIDE);

		Sleep(100);
	}

	for (auto &thread: threadGrabber.threads()) {
		if (thread->id() != currentThreadId &&
			thread->open(thread->access() | THREAD_SUSPEND_RESUME))
			thread->suspend();
	}

	std::wostringstream msgStrm;
	msgStrm << L"An unhandled exception occured at 0x" << std::hex << std::uppercase
			<< pInfo->ExceptionRecord->ExceptionAddress << L".\r\n"
			<< L"Thread: " << std::dec << currentThreadId << L"\r\n"
			<< L"Type: ";

	switch (pInfo->ExceptionRecord->ExceptionCode) {
		SEH_CASE(EXCEPTION_ACCESS_VIOLATION);
		SEH_CASE(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
		SEH_CASE(EXCEPTION_BREAKPOINT);
		SEH_CASE(EXCEPTION_DATATYPE_MISALIGNMENT);
		SEH_CASE(EXCEPTION_FLT_DENORMAL_OPERAND);
		SEH_CASE(EXCEPTION_FLT_DIVIDE_BY_ZERO);
		SEH_CASE(EXCEPTION_FLT_INEXACT_RESULT);
		SEH_CASE(EXCEPTION_FLT_INVALID_OPERATION);
		SEH_CASE(EXCEPTION_FLT_OVERFLOW);
		SEH_CASE(EXCEPTION_FLT_STACK_CHECK);
		SEH_CASE(EXCEPTION_FLT_UNDERFLOW);
		SEH_CASE(EXCEPTION_ILLEGAL_INSTRUCTION);
		SEH_CASE(EXCEPTION_IN_PAGE_ERROR);
		SEH_CASE(EXCEPTION_INT_DIVIDE_BY_ZERO);
		SEH_CASE(EXCEPTION_INT_OVERFLOW);
		SEH_CASE(EXCEPTION_INVALID_DISPOSITION);
		SEH_CASE(EXCEPTION_NONCONTINUABLE_EXCEPTION);
		SEH_CASE(EXCEPTION_PRIV_INSTRUCTION);
		SEH_CASE(EXCEPTION_SINGLE_STEP);
		SEH_CASE(EXCEPTION_STACK_OVERFLOW);
		SEH_CASE(EXCEPTION_GUARD_PAGE);
		SEH_CASE(EXCEPTION_INVALID_HANDLE);
		SEH_CASE(EXCEPTION_MSVC);

	default:
		msgStrm << L"Unknown exception";
	}

	msgStrm << L" (0x" << std::hex << std::uppercase
			<< pInfo->ExceptionRecord->ExceptionCode
			<< L")\r\n\r\n";

	if ((pInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
		 pInfo->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) &&
		pInfo->ExceptionRecord->NumberParameters > 1) {
		std::wstring accessType;
		switch (pInfo->ExceptionRecord->ExceptionInformation[0])
		{
		case EXCEPTION_READ_FAULT:
			accessType = L"read";
			break;

		case EXCEPTION_WRITE_FAULT:
			accessType = L"written";
			break;

		case EXCEPTION_EXECUTE_FAULT:
			accessType = L"executed";
			break;
		}

		msgStrm << L"Extended information:\r\n";
		msgStrm << L"The memory at 0x" << std::hex << std::uppercase
				<< reinterpret_cast<void*>(pInfo->ExceptionRecord->ExceptionInformation[1])
				<< L" could not be " << accessType << L".\r\n";
	}

	else if (pInfo->ExceptionRecord->ExceptionCode == EXCEPTION_MSVC &&
			 pInfo->ExceptionRecord->NumberParameters > 2) {
		ULONG_PTR dwBaseObj = pInfo->ExceptionRecord->ExceptionInformation[1];
		_ThrowInfo *pThrowInfo = reinterpret_cast<_ThrowInfo*>(pInfo->ExceptionRecord->ExceptionInformation[2]);

		if (dwBaseObj != 0 && pThrowInfo != nullptr &&
			pThrowInfo->pCatchableTypeArray != nullptr)
		{
			msgStrm << L"Extended information:\r\n";
			static const std::type_info& stdExInfo = typeid(std::exception);
			static const std::type_info& stdStringInfo = typeid(std::string);
			static const std::type_info& stdWStringInfo = typeid(std::wstring);
			static const std::type_info& charInfo = typeid(char*);
			static const std::type_info& wcharInfo = typeid(wchar_t*);

			std::type_info *pBaseType = nullptr;
			std::type_info *pLastType = nullptr;

			_CatchableTypeArray *pTypeArray = pThrowInfo->pCatchableTypeArray;
			for (uint32 i = 0; i < static_cast<uint32>(pTypeArray->nCatchableTypes); ++i) {
				// Get the object for each catchable type inside the catchable type array...
				_CatchableType *pCatchableType = pTypeArray->arrayOfCatchableTypes[i];
				std::type_info *pTypeInfo = reinterpret_cast<std::type_info*>(pCatchableType->pType);
				void *pmDisp = reinterpret_cast<void*>(dwBaseObj + pCatchableType->thisDisplacement.mdisp);

				if (i == 0)
					pBaseType = pTypeInfo;

				if (*pTypeInfo == stdExInfo) {
					std::string sTypeName(pBaseType->name());
					msgStrm << L"Type: " << std::wstring(sTypeName.begin(), sTypeName.end()) << L"\r\n";

					std::exception *pException = reinterpret_cast<std::exception*>(pmDisp);
					if (pException != nullptr) {
						const char *pszMessage = pException->what();
						std::string sMessage(pszMessage != nullptr ? pszMessage : "<null>");
						msgStrm << L"Message: " << std::wstring(sMessage.begin(), sMessage.end()) << L"\r\n";
					}
					break;
				}

				if (*pTypeInfo == stdStringInfo) {
					msgStrm << L"Type: class std::string\r\n";

					std::string *pString = reinterpret_cast<std::string*>(pmDisp);
					if (pString != nullptr)
						msgStrm << L"Message: " << std::wstring(pString->begin(), pString->end()) << L"\r\n";
					break;
				}

				if (*pTypeInfo == stdWStringInfo) {
					msgStrm << L"Type: class std::wstring\r\n";

					std::wstring *pString = reinterpret_cast<std::wstring*>(pmDisp);
					if (pString != nullptr)
						msgStrm << L"Message: " << *pString << L"\r\n";
					break;
				}

				if (*pTypeInfo == charInfo) {
					msgStrm << L"Type: string (char*)\r\n";

					const char **ppszMessage = reinterpret_cast<const char**>(pmDisp);
					if (ppszMessage != nullptr) {
						std::string sMessage(*ppszMessage != nullptr ? *ppszMessage : "<null>");
						msgStrm << L"Message: " << std::wstring(sMessage.begin(), sMessage.end()) << L"\r\n";
					}
					break;
				}

				if (*pTypeInfo == wcharInfo) {
					msgStrm << L"Type: wstring (wchar_t*)\r\n";

					const wchar_t **ppswzMessage = reinterpret_cast<const wchar_t**>(pmDisp);
					if (ppswzMessage != nullptr)
						msgStrm << L"Message: " << (*ppswzMessage != nullptr ? *ppswzMessage : L"<null>") << L"\r\n";
					break;
				}

				if (i == static_cast<uint32>(pTypeArray->nCatchableTypes - 1))
					pLastType = pTypeInfo;
			}

			if (pLastType != nullptr && pBaseType != nullptr) {
				std::string sTypeName(pBaseType->name());
				msgStrm << L"Type: " << std::wstring(sTypeName.begin(), sTypeName.end()) << L"\r\n";
			}
		}
	}

	std::wostringstream strmAdditionalInfo;

	OSVERSIONINFOEXW versionInfo = {0};
	versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);

	// Print OS version...
	if (GetVersionExW(reinterpret_cast<LPOSVERSIONINFOW>(&versionInfo)) != FALSE) {
		strmAdditionalInfo << L"Windows Build: " << std::dec
						   << versionInfo.dwMajorVersion << L'.'
						   << versionInfo.dwMinorVersion << L'.'
						   << versionInfo.dwBuildNumber;

		if (versionInfo.wServicePackMajor != 0 ||
			versionInfo.wServicePackMinor != 0)
			strmAdditionalInfo << L" (SP "
							   << versionInfo.wServicePackMajor << L'.'
							   << versionInfo.wServicePackMinor
							   << L')';

		strmAdditionalInfo << L"\r\n";
	}

	MEMORYSTATUSEX memoryStatus = {0};
	memoryStatus.dwLength = sizeof(memoryStatus);
	if (GlobalMemoryStatusEx(&memoryStatus) != FALSE)
		strmAdditionalInfo << L"Memory Load: "
						   << memoryStatus.dwMemoryLoad
						   << L"%\r\n";

	DWORD dwLastError = GetLastError();
	if (dwLastError != NO_ERROR)
		strmAdditionalInfo << L"Last Error: "
						   << std::dec << dwLastError
						   << L"\r\n";

	strmAdditionalInfo << L"\r\n";

	// Print exception info array...
	if (pInfo->ExceptionRecord->NumberParameters > 0) {
		strmAdditionalInfo << L"Exception Information:\r\n";
		for (uint32 i = 0; i < pInfo->ExceptionRecord->NumberParameters; ++i) {
			strmAdditionalInfo << L"  [" << std::dec << i << L"]: 0x" << std::hex << std::uppercase
							   << reinterpret_cast<void*>(pInfo->ExceptionRecord->ExceptionInformation[i])
							   << L"\r\n";
		}

		strmAdditionalInfo << L"\r\n";
	}

	// Print register dump...
	strmAdditionalInfo << L"Register Dump (x86):" << std::hex << std::uppercase
					   << L"\r\n  EAX: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Eax)
					   << L"\r\n  EBX: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Ebx)
					   << L"\r\n  ECX: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Ecx)
					   << L"\r\n  EDX: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Edx)
					   << L"\r\n  ESI: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Esi)
					   << L"\r\n  EDI: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Edi)
					   << L"\r\n  EBP: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Ebp)
					   << L"\r\n  ESP: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Esp)
					   << L"\r\n  EIP: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->Eip)
					   << L"\r\n  EFlags: 0x" << reinterpret_cast<void*>(pInfo->ContextRecord->EFlags)

					   << L"\r\n\r\nSegment Registers:"
						  L"\r\n  CS: 0x" << std::setfill(L'0') << std::setw(4) << pInfo->ContextRecord->SegCs
					   << L"\r\n  DS: 0x" << std::setfill(L'0') << std::setw(4) << pInfo->ContextRecord->SegDs
					   << L"\r\n  ES: 0x" << std::setfill(L'0') << std::setw(4) << pInfo->ContextRecord->SegEs
					   << L"\r\n  SS: 0x" << std::setfill(L'0') << std::setw(4) << pInfo->ContextRecord->SegSs
					   << L"\r\n  FS: 0x" << std::setfill(L'0') << std::setw(4) << pInfo->ContextRecord->SegFs
					   << L"\r\n  GS: 0x" << std::setfill(L'0') << std::setw(4) << pInfo->ContextRecord->SegGs
					   << L"\r\n\r\n";

	// Print module dump...
	strmAdditionalInfo << L"Module Dump:\r\n  0x"
					   << sEngine.GetInstance()
					   << L": <Current Module>\r\n";

	{   // Initialize debug helper
		Utils::DebugHelper dbgHelper;

		try {
			strmAdditionalInfo << dbgHelper.DumpModules(GetCurrentProcessId());
		}
		catch (std::exception &e) {
			strmAdditionalInfo << L"  " << e.what() << L"\r\n";
		}

		// Print stack trace...
		strmAdditionalInfo << L"\r\nStack Trace:\r\n";

		try {
			dbgHelper.LoadDbgHelp();
			strmAdditionalInfo << dbgHelper.DumpCallStack(GetCurrentThread(), pInfo->ContextRecord);
		}
		catch (std::exception &e) {
			strmAdditionalInfo << L"  " << e.what() << L"\r\n";
		}

		// Print code dump...
		strmAdditionalInfo << L"\r\nCode Dump (EIP, 64 Bytes):\r\n";

		try {
			void *pCodeAddr = reinterpret_cast<void*>(pInfo->ContextRecord->Eip & ~0xF);
			strmAdditionalInfo << dbgHelper.DumpMemory(pCodeAddr, 64);
		}
		catch (std::exception &e) {
			strmAdditionalInfo << L"  " << e.what() << L"\r\n";
		}

		// Print stack dump...
		strmAdditionalInfo << L"\r\nStack Dump (ESP, 1024 Bytes):\r\n";

		try {
			void *pStackTop = reinterpret_cast<void*>(pInfo->ContextRecord->Esp & ~0xF);
			strmAdditionalInfo << dbgHelper.DumpMemory(pStackTop, 1024);
		}
		catch (std::exception &e) {
			strmAdditionalInfo << L"  " << e.what() << L"\r\n";
		}
	}

	// Generate timestamp for crash log file...
	time_t t = time(nullptr);
	tm *now = localtime(&t);
	std::wostringstream timestamp;
	timestamp << std::setfill(L'0') << std::setw(4)
			  << now->tm_year + 1900 << L'-'
			  << std::setfill(L'0') << std::setw(2)
			  << now->tm_mon + 1 << L'-'
			  << std::setfill(L'0') << std::setw(2)
			  << now->tm_mday << '_'
			  << std::setfill(L'0') << std::setw(2)
			  << now->tm_hour
			  << std::setfill(L'0') << std::setw(2)
			  << now->tm_min
			  << std::setfill(L'0') << std::setw(2)
			  << now->tm_sec;

	// Write dump to our log...
	std::wstring logsDir = sFileMgr.GetLogsDirectory() + L'\\';
	std::wofstream crashDumpFile(logsDir + timestamp.str() + L".log",
		std::fstream::out | std::fstream::trunc | std::fstream::binary);

	crashDumpFile << L"-------------------------------------\r\n"
				     L"--           Crash Reason          --\r\n"
				     L"-------------------------------------\r\n"
					 L"\r\n"
				  << msgStrm.str()
				  << L"\r\n"
					 L"-------------------------------------\r\n"
				     L"--      Additional Information     --\r\n"
				     L"-------------------------------------\r\n"
					 L"\r\n"
				  << strmAdditionalInfo.str()
				  << L"\r\n";

	crashDumpFile.close();

	ACTCTX actCtx = {0};
	actCtx.cbSize = sizeof(actCtx);
	actCtx.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_HMODULE_VALID | ACTCTX_FLAG_LANGID_VALID;
	actCtx.wLangId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	actCtx.lpResourceName = MAKEINTRESOURCE(IDR_RT_MANIFEST1);
	actCtx.hModule = sEngine.GetInstance();
	HANDLE hActCtx = CreateActCtx(&actCtx);

	ULONG_PTR actCookie = 0;
	if (hActCtx != INVALID_HANDLE_VALUE)
		ActivateActCtx(hActCtx, &actCookie);

	HMODULE hComctl32 = LoadLibraryW(L"COMCTL32.dll");
	if (hComctl32 != nullptr) {
		typedef BOOL (WINAPI *tInitCommonControlsEx) (_In_ const LPINITCOMMONCONTROLSEX lpInitCtrls);
		tInitCommonControlsEx pInitCommonControlsEx = reinterpret_cast<tInitCommonControlsEx>
			(GetProcAddress(hComctl32, "InitCommonControlsEx"));

		if (pInitCommonControlsEx != nullptr) {
			INITCOMMONCONTROLSEX ice = {0};
			ice.dwSize = sizeof(ice);
			ice.dwICC = ICC_STANDARD_CLASSES;
			pInitCommonControlsEx(&ice);
		}
	}

	HWND hDlg = CreateDialog(sEngine.GetInstance(),
		MAKEINTRESOURCE(IDD_DIALOG1), nullptr,
		reinterpret_cast<DLGPROC>(_dlgProc));

	HWND hReason = GetDlgItem(hDlg, IDC_EB_REASON);
	SetWindowTextW(hReason, msgStrm.str().c_str());

	HWND hErroutBox = GetDlgItem(hDlg, IDC_EB_ERROUT);
	SetWindowTextW(hErroutBox, strmAdditionalInfo.str().c_str());
	ShowWindow(hDlg, SW_SHOW);
	SetForegroundWindow(hDlg);

	MSG msg = {0};
	BOOL messageResult = FALSE;
	while ((messageResult = GetMessage(&msg, nullptr, 0, 0)) != FALSE) {
		if (messageResult != -1 &&
			(!IsWindow(hDlg) || !IsDialogMessage(hDlg, &msg))) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (hComctl32 != nullptr)
		FreeLibrary(hComctl32);

	return EXCEPTION_EXECUTE_HANDLER;
}
