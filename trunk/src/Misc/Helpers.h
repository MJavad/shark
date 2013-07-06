#pragma once

namespace Utils {
	std::wstring OsClipboardGetString(HWND hWndNewOwner = nullptr);
	bool OsClipboardPutString(const std::wstring &string, HWND hWndNewOwner = nullptr);
}
