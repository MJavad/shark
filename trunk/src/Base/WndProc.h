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

#pragma once

class WndProc {
	SINGLETON_CLASS(WndProc) :
		m_hWnd(nullptr),
		m_wndProc(nullptr),
		m_rMouseOk(true),
		m_lMouseOk(true),
		m_isSizing(false),
		m_lastMessage(0) {}

public:
	~WndProc();

	void Initialize();
	bool Attach(HWND hWnd);
	bool Detach();

	HWND FindCurrentWindow() const;

	UINT GetLastMessage() const {
		return m_lastMessage;
	}

	HWND GetHWND() const {
		return m_hWnd;
	}

	WNDPROC GetWndProc() const {
		return m_wndProc;
	}

	bool IsAttached() const {
		return m_wndProc != nullptr;
	}
	
	bool LastMessageHandled;
	Utils::Event<void (UINT, WPARAM, LPARAM)> OnMessageReceivedEvent;

protected:
	HWND m_hWnd;
	WNDPROC m_wndProc;
	bool m_rMouseOk, m_lMouseOk;
	bool m_isSizing;
	UINT m_lastMessage;

	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#define sWndProc ::WndProc::Instance()
