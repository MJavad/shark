#include "Misc/stdafx.h"
#include "WndProc.h"

void WndProc::Initialize() {
	m_hWnd = nullptr;
	m_wndProc = nullptr;
	m_isSizing = false;
	LastMessageHandled = false;
}

WndProc::~WndProc() {
	if (IsAttached())
		Detach();
}

bool WndProc::Attach(HWND hWnd) {
	if (m_hWnd != hWnd)
		Detach();
	else return true;

	LONG_PTR nResult = SetWindowLongPtrW(hWnd, GWL_WNDPROC,
		reinterpret_cast<LONG_PTR>(WindowProc));

	if (nResult == 0) {
		LOG_DEBUG("Failed to attach WndProc!");
		return false;
	}

	m_hWnd = hWnd;
	m_wndProc = reinterpret_cast<WNDPROC>(nResult);
	return true;
}

bool WndProc::Detach() {
	if (m_hWnd == nullptr || m_wndProc == nullptr)
		return false;

	LONG_PTR nResult = SetWindowLongPtrW(m_hWnd, GWL_WNDPROC,
		reinterpret_cast<LONG_PTR>(m_wndProc));

	if (nResult == 0) {
		return false;
	}

	m_hWnd = nullptr;
	m_wndProc = nullptr;
	return true;
}

HWND WndProc::FindCurrentWindow() const {
	HWND hWnd = nullptr;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&hWnd));
	return hWnd;
}

BOOL CALLBACK WndProc::EnumWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId != GetCurrentProcessId())
		return TRUE;

	*reinterpret_cast<HWND*>(lParam) = hWnd;
	return FALSE;
}

LRESULT CALLBACK WndProc::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	bool bInClientRect = true;
	sWndProc.LastMessageHandled = false;
	
	// Disable the handler if the user is currently resizing the window...
	if (uMsg == WM_SIZING || uMsg == WM_SIZE)
		sWndProc.m_isSizing = true;

	else if (uMsg == WM_LBUTTONUP)
		sWndProc.m_isSizing = false;

	// Pre mouse fix for games like WoW (turning camera, moving stuff behind the interface...)
	switch (uMsg) {
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
		{
			// Get current window client area and check if cursor is inside it...
			RECT clientRect = {0};
			GetClientRect(hWnd, &clientRect);
			bInClientRect = (PtInRect(&clientRect, Utils::Vector2(lParam)) != FALSE);

			// If one of the mouse states is false, the user clicked somewhere behind the interface...
			// We don't want to handle the following actions in our GUI...
			sWndProc.LastMessageHandled |= !sWndProc.m_lMouseOk;
			sWndProc.LastMessageHandled |= !sWndProc.m_rMouseOk;
			sWndProc.LastMessageHandled |= !bInClientRect;
		}
	}

	// Call events!
	sWndProc.OnMessageReceivedEvent(uMsg, wParam, lParam);

	// Post mouse fix - should we handle the message?
	switch (uMsg) {
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
		{
			// Everything is fine if this is true...
			sWndProc.LastMessageHandled &= sWndProc.m_lMouseOk;
			sWndProc.LastMessageHandled &= sWndProc.m_rMouseOk;
			sWndProc.LastMessageHandled &= bInClientRect;

			if (uMsg == WM_LBUTTONDOWN)
				sWndProc.m_lMouseOk = sWndProc.LastMessageHandled;

			else if (uMsg == WM_LBUTTONUP)
				sWndProc.m_lMouseOk = true;

			else if (uMsg == WM_RBUTTONDOWN)
				sWndProc.m_rMouseOk = sWndProc.LastMessageHandled;

			else if (uMsg == WM_RBUTTONUP)
				sWndProc.m_rMouseOk = true;
		}
	}

	// Handle it if we are not resizing the window...
	if (sWndProc.LastMessageHandled && !sWndProc.m_isSizing)
		return 0;

	// Don't handle it...
	return CallWindowProcW(sWndProc.m_wndProc, hWnd, uMsg, wParam, lParam);
}
