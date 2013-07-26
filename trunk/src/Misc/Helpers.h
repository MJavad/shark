#pragma once

namespace Utils {
	std::wstring OsClipboardGetString();
	std::string OsClipboardGetString_UTF8();

	bool OsClipboardPutString(const std::wstring &string);
	bool OsClipboardPutString_UTF8(const std::string &string);

	uint32 FindStartOfWord(const std::wstring &string, uint32 position);
	uint32 FindEndOfWord(const std::wstring &string, uint32 position);

	std::wstring GetWordAt(const std::wstring &string, uint32 position);
	void GetWordPositions(const std::wstring &string, uint32 position, uint32 *start, uint32 *end);

	void ThrowIfFailed(HRESULT hResult);
}
