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
#include "WndProc.h"
#include "Engine.h"

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

	LONG_PTR nResult = SetWindowLongPtr(hWnd, GWL_WNDPROC,
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

	if (nResult == 0)
		return false;

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
	sWndProc.m_lastMessage = uMsg;
	sWndProc.LastMessageHandled = false;
	
	// Disable the handler if the user is currently resizing the window...
	if (uMsg == WM_SIZING || uMsg == WM_SIZE)
		sWndProc.m_isSizing = true;

	else if (uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN)
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

	// We need to shutdown some stuff or we will get a
	// corrupted unload order when our application exits.
	if (uMsg == WM_DESTROY)
		sEngine.OnWindowDestroy();

	// If we handled the message and we are not resizing the window
	// then we don't need to pass the message to our application.
	else if (sWndProc.LastMessageHandled && !sWndProc.m_isSizing)
		return 0;

	// Don't handle it...
	return CallWindowProcW(sWndProc.m_wndProc, hWnd, uMsg, wParam, lParam);
}
