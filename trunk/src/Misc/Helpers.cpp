#include "Misc/stdafx.h"
#include "Helpers.h"
#include "Base/WndProc.h"

namespace Utils {
	std::wstring OsClipboardGetString() {
		std::wstring result;
		if (OpenClipboard(sWndProc.GetHWND()) != FALSE) {
			HANDLE globalMem = GetClipboardData(CF_UNICODETEXT);
			if (globalMem != nullptr) {
				wchar_t *pswzBuffer = reinterpret_cast<wchar_t*>(GlobalLock(globalMem));
				if (pswzBuffer != nullptr) {
					result = pswzBuffer;
					GlobalUnlock(globalMem);
				}
			}

			CloseClipboard();
		}

		return result;
	}

	std::string OsClipboardGetString_UTF8() {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(OsClipboardGetString());
	}

	bool OsClipboardPutString(const std::wstring &string) {
		SIZE_T bufferSize = (string.length() + 1) * sizeof(wchar_t);
		HGLOBAL globalMem = GlobalAlloc(GHND, bufferSize);
		if (globalMem == nullptr)
			return false;

		bool success = false;
		wchar_t *pswzBuffer = reinterpret_cast<wchar_t*>(GlobalLock(globalMem));
		if (pswzBuffer != nullptr) {
			wcsncpy(pswzBuffer, string.data(), string.length());
			GlobalUnlock(globalMem);

			if (OpenClipboard(sWndProc.GetHWND()) != FALSE) {
				EmptyClipboard();
				if (SetClipboardData(CF_UNICODETEXT, globalMem) != nullptr)
					success = true;

				CloseClipboard();
			}
		}

		if (!success)
			GlobalFree(globalMem);

		return success;
	}

	bool OsClipboardPutString_UTF8(const std::string &string) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return OsClipboardPutString(conv.from_bytes(string));
	}

	/* Strings Util */
	const wchar_t BreakingChars[] = L" -+*/\\'\"´`$€%@&()[]{}.:,;|<>!?=#^°~\a\b\t\n\r";

	uint32 FindStartOfWord(const std::wstring &string, uint32 position) {
		if (position == 0)
			return 0;

		uint32 start = string.find_last_of(BreakingChars, position - 1);
		if (start == position - 1)
			start = string.find_last_not_of(BreakingChars, position - 1);

		return (start != std::string::npos ? start + 1 : 0);
	}

	uint32 FindEndOfWord(const std::wstring &string, uint32 position) {
		uint32 end = string.find_first_of(BreakingChars, position);
		if (end == position)
			end = string.find_first_not_of(BreakingChars, position);

		return (end != std::string::npos ? end : string.length());
	}

	std::wstring GetWordAt(const std::wstring &string, uint32 position) {
		uint32 wordStart = 0;
		uint32 wordEnd = 0;
		GetWordPositions(string, position, &wordStart, &wordEnd);
		return string.substr(wordStart, wordEnd - wordStart);
	}

	void GetWordPositions(const std::wstring &string, uint32 position, uint32 *start, uint32 *end) {
		uint32 first = string.find_last_of(BreakingChars, position);
		uint32 last = string.find_first_of(BreakingChars, position);

		if (first == last && first != std::string::npos) {
			first = string.find_last_not_of(BreakingChars, position);
			last = string.find_first_not_of(BreakingChars, position);
		}

		if (first == std::string::npos)
			first = 0;
		else
			++first;

		if (last == std::string::npos)
			last = string.length();

		if (start != nullptr)
			*start = first;

		if (end != nullptr)
			*end = last;
	}

	void ThrowIfFailed(HRESULT hResult) {
		if (FAILED(hResult))
			throw std::runtime_error("Windows operation failed. Bad HRESULT.");
	}
}
