#pragma once

class ExceptionManager {
	SINGLETON_CLASS(ExceptionManager) {}

public:
	~ExceptionManager();
	void Initialize();

private:
	static LONG WINAPI _filter(PEXCEPTION_POINTERS pInfo);
	static BOOL CALLBACK _dlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#define sExceptionMgr ::ExceptionManager::Instance()
