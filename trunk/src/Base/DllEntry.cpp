#include "Misc/stdafx.h"
#include "DllEntry.h"
#include "Offsets.h"
#include "Delegates.h"
#include "Detours.h"
#include "WndProc.h"
#include "D3DManager.h"
#include "UI/GUIManager.h"
#include "Engine.h"
#include "Misc/DebugHelper.h"
#include "../resource.h"
#include <Commctrl.h>

__declspec(naked) LPTOP_LEVEL_EXCEPTION_FILTER WINAPI
	_SetUnhandledExceptionFilter(_In_ LPTOP_LEVEL_EXCEPTION_FILTER) {
	__asm {
		push ebp
		mov ebp, esp
		mov eax, [SetUnhandledExceptionFilter]
		add eax, 5
		jmp eax
	}
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		_SetUnhandledExceptionFilter(InternalExceptionFilter);

		sEngine.Initialize(hInstance);
		break;

	case DLL_PROCESS_DETACH:
		_SetUnhandledExceptionFilter(nullptr);
		sEngine.OnProcessDetach();
		break;
	}

	return TRUE;
}

// EXPORTS: Load
BOOL WINAPI LoadInternal(LPVOID lpParam) {
	srand(timeGetTime());

	try {
		sLog.Initialize();
		sMemory.Initialize();
		sOffsets.Initialize();
		sDelegates.Initialize();
		sWndProc.Initialize();
		sD3DMgr.Initialize();
		sDetours.Initialize();
	}
	catch (std::exception &e) {
		MessageBoxA(nullptr, e.what(), "ERROR", MB_OK);
	}

	//LOG_DEBUG(L"Initialized!");
	return EXIT_SUCCESS;
}

// EXPORTS: Unload
BOOL WINAPI UnloadInternal(LPVOID lpParam) {
	if (!sEngine.IsShuttingDown())
		sEngine.InitializeShutdown();

	return EXIT_SUCCESS;
}



BOOL CALLBACK ExceptionDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND hShutdown = GetDlgItem(hwndDlg, IDC_SHUTDOWN);
			HFONT hFont = CreateFontW(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				FF_DONTCARE, L"Segoe UI");

			if (hFont != nullptr)
				SendMessageW(hShutdown, WM_SETFONT, (WPARAM) hFont, FALSE);

			HICON errIcon = LoadIconW(nullptr, IDI_ERROR);
			HWND hIconStatic = GetDlgItem(hwndDlg, IDC_ERRPICBOX);
			if (errIcon != nullptr)
				SendMessageW(hIconStatic, STM_SETIMAGE, IMAGE_ICON, (LPARAM)errIcon);

			SendMessageW(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIconW(nullptr, IDI_EXCLAMATION));
		}
		return TRUE;

	case WM_CLOSE:
		PostQuitMessage(0);
		return TRUE;
	}

	return FALSE;
}

#define EXCEPTION_MSVC 0xE06D7363
#define SEH_CASE(code) case code: msgStrm << L#code; break;

LONG WINAPI InternalExceptionFilter(PEXCEPTION_POINTERS pInfo) {
	HWND hGameWindow = sWndProc.FindCurrentWindow();
	if (hGameWindow != nullptr &&
		ShowWindow(hGameWindow, SW_FORCEMINIMIZE) != FALSE)
		Sleep(50);

	Utils::ThreadGrabber threadGrabber;
	threadGrabber.update(GetCurrentProcessId());
	uint32 currentThreadId = GetCurrentThreadId();

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

	if (pInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
		pInfo->ExceptionRecord->NumberParameters >= 2) {
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

	else if (pInfo->ExceptionRecord->ExceptionCode == EXCEPTION_MSVC) {
		msgStrm << L"Extended information:\r\n";

		static const std::type_info& stdExInfo = typeid(std::exception);
		static const std::type_info& stdStringInfo = typeid(std::string);
		static const std::type_info& stdWStringInfo = typeid(std::wstring);
		static const std::type_info& charInfo = typeid(char*);
		static const std::type_info& wcharInfo = typeid(wchar_t*);

		std::type_info *pBaseType = nullptr;
		std::type_info *pLastType = nullptr;

		ULONG_PTR dwBaseObj = pInfo->ExceptionRecord->ExceptionInformation[1];
		_CatchableTypeArray *pTypeArray = ((_ThrowInfo*) pInfo->ExceptionRecord->ExceptionInformation[2])->pCatchableTypeArray;

		for (uint32 i = 0; i < (uint32) pTypeArray->nCatchableTypes; ++i) {
			_CatchableType *pCatchableType = pTypeArray->arrayOfCatchableTypes[i];
			std::type_info *pTypeInfo = (std::type_info*) pCatchableType->pType;
			void *pmDisp = (void*) (dwBaseObj + pCatchableType->thisDisplacement.mdisp);

			if (i == 0)
				pBaseType = pTypeInfo;

			if (*pTypeInfo == stdExInfo) {
				std::exception *pException = (std::exception*) pmDisp;
				std::string sTypeName(pBaseType->name());
				std::string sMessage(pException->what());

				msgStrm << L"Type: " << std::wstring(sTypeName.begin(), sTypeName.end()) << L"\r\n";
				msgStrm << L"Message: " << std::wstring(sMessage.begin(), sMessage.end()) << L"\r\n";
				break;
			}

			if (*pTypeInfo == stdStringInfo) {
				std::string *pString = (std::string*) pmDisp;
				msgStrm << L"Type: class std::string\r\n";
				msgStrm << L"Message: " << std::wstring(pString->begin(), pString->end()) << L"\r\n";
				break;
			}

			if (*pTypeInfo == stdWStringInfo) {
				std::wstring *pString = (std::wstring*) pmDisp;
				msgStrm << L"Type: class std::wstring\r\n";
				msgStrm << L"Message: " << *pString << L"\r\n";
				break;
			}

			if (*pTypeInfo == charInfo) {
				const char **ppMessage = (const char**) pmDisp;
				std::string sMessage(ppMessage != nullptr ? *ppMessage : "null");

				msgStrm << L"Type: string (char*)\r\n";
				msgStrm << L"Message: " << std::wstring(sMessage.begin(), sMessage.end()) << L"\r\n";
				break;
			}

			if (*pTypeInfo == wcharInfo) {
				const wchar_t **ppMessage = (const wchar_t**) pmDisp;

				msgStrm << L"Type: wstring (wchar_t*)\r\n";
				msgStrm << L"Message: " << (ppMessage != nullptr ? *ppMessage : L"null") << L"\r\n";
				break;
			}

			if (i == (uint32) (pTypeArray->nCatchableTypes - 1))
				pLastType = pTypeInfo;
		}

		if (pLastType != nullptr && pBaseType != nullptr) {
			std::string sTypeName(pBaseType->name());
			msgStrm << L"Type: " << std::wstring(sTypeName.begin(), sTypeName.end()) << L"\r\n";
		}
	}

	ACTCTXW actCtx = {0};
	actCtx.cbSize = sizeof(ACTCTX);
	actCtx.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_HMODULE_VALID | ACTCTX_FLAG_LANGID_VALID;
	actCtx.wLangId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	actCtx.lpResourceName = MAKEINTRESOURCE(IDR_RT_MANIFEST1);
	actCtx.hModule = sEngine.GetInstance();
	HANDLE hActCtx = CreateActCtxW(&actCtx);

	ULONG_PTR actCookie = 0;
	if (hActCtx != INVALID_HANDLE_VALUE)
		ActivateActCtx(hActCtx, &actCookie);

	HMODULE hComctl32 = LoadLibraryW(L"Comctl32.dll");
	typedef BOOL (WINAPI *tInitCommonControlsEx) (const INITCOMMONCONTROLSEX *picce);
	tInitCommonControlsEx pInitCommonControlsEx = reinterpret_cast<tInitCommonControlsEx>(GetProcAddress(hComctl32, "InitCommonControlsEx"));

	INITCOMMONCONTROLSEX ice = {0};
	ice.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ice.dwICC = ICC_STANDARD_CLASSES;

	if (pInitCommonControlsEx != nullptr)
		pInitCommonControlsEx(&ice);

	HWND hDlg = CreateDialogW(sEngine.GetInstance(),
		MAKEINTRESOURCE(IDD_DIALOG1), nullptr, (DLGPROC) ExceptionDlgProc);

	HWND hReason = GetDlgItem(hDlg, IDC_REASON);
	SetWindowTextW(hReason, msgStrm.str().c_str());

	// Print register dump...
	std::wostringstream strmAdditionalInfo;
	strmAdditionalInfo << L"Register Dump (x86):"
					   << L"\r\n   EAX: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Eax)
					   << L"\r\n   EBX: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Ebx)
					   << L"\r\n   ECX: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Ecx)
					   << L"\r\n   EDX: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Edx)
					   << L"\r\n   ESI: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Esi)
					   << L"\r\n   EDI: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Edi)
					   << L"\r\n   EBP: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Ebp)
					   << L"\r\n   ESP: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Esp)
					   << L"\r\n   EIP: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->Eip)
					   << L"\r\n   EFlags: 0x" << std::hex << std::uppercase << reinterpret_cast<void*>(pInfo->ContextRecord->EFlags)
					   << L"\r\n\r\n";

	// Print module dump...
	strmAdditionalInfo << L"Module Dump:\r\n   0x"
					   << std::hex << std::uppercase << sEngine.GetInstance()
					   << L" - <Current Module>\r\n";

	// Initialize debug helper
	Utils::DebugHelper dbgHelper;

	try {
		strmAdditionalInfo << dbgHelper.DumpModules(GetCurrentProcessId());
	}
	catch (std::exception &e) {
		strmAdditionalInfo << L"   " << e.what() << L"\r\n";
	}

	// Print stack trace...
	strmAdditionalInfo << L"\r\nStack Trace:\r\n";

	try {
		dbgHelper.LoadDbgHelp();
		strmAdditionalInfo << dbgHelper.DumpCallStack(GetCurrentThread(), pInfo->ContextRecord);
	}
	catch (std::exception &e) {
		strmAdditionalInfo << L"   " << e.what() << L"\r\n";
	}

	HWND hErroutBox = GetDlgItem(hDlg, IDC_ERROUT);
	SetWindowTextW(hErroutBox, strmAdditionalInfo.str().c_str());
	ShowWindow(hDlg, SW_SHOW);
	SetForegroundWindow(hDlg);

	MSG msg = {0};
	BOOL messageResult = FALSE;
	while ((messageResult = GetMessageW(&msg, nullptr, 0, 0)) != FALSE) {
		if (messageResult != -1 &&
			(!IsWindow(hDlg) || !IsDialogMessageW(hDlg, &msg))) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	EndDialog(hDlg, 0);
	FreeLibrary(hComctl32);
	return EXCEPTION_EXECUTE_HANDLER;
}
