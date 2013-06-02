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
