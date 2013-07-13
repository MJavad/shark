#pragma once

namespace Utils {
	std::wstring OsClipboardGetString(HWND hWndNewOwner = nullptr);
	bool OsClipboardPutString(const std::wstring &string, HWND hWndNewOwner = nullptr);
	uint32 FindStartOfWord(const std::wstring &string, uint32 position);
	uint32 FindEndOfWord(const std::wstring &string, uint32 position);
	void GetWordPositions(const std::wstring &string, uint32 position, uint32 *start, uint32 *end);
	std::wstring GetWordAt(const std::wstring &string, uint32 position);
}
