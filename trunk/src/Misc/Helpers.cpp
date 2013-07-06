#include "Misc/stdafx.h"
#include "Helpers.h"

namespace Utils {
	std::wstring OsClipboardGetString(HWND hWndNewOwner) {
		std::wstring result;
		if (OpenClipboard(hWndNewOwner) != FALSE) {
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

	bool OsClipboardPutString(const std::wstring &string, HWND hWndNewOwner) {
		SIZE_T bufferSize = (string.length() + 1) * sizeof(wchar_t);
		HGLOBAL globalMem = GlobalAlloc(GHND, bufferSize);
		if (globalMem == nullptr)
			return false;

		bool success = false;
		wchar_t *pswzBuffer = reinterpret_cast<wchar_t*>(GlobalLock(globalMem));
		if (pswzBuffer != nullptr) {
			wcsncpy(pswzBuffer, string.data(), string.length());
			GlobalUnlock(globalMem);

			if (OpenClipboard(hWndNewOwner) != FALSE) {
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
}
