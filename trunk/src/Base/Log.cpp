#include "Misc/stdafx.h"
#include "Log.h"

void Log::Initialize() {

}

// TODO: Proper implementation
void Log::OutDebug(const std::wstring &swMessage) const {
	std::wstring swBuffer(L"[DEBUG] ");
	swBuffer += swMessage;
	MessageBoxW(nullptr, swBuffer.c_str(), L"Debug Message", MB_OK);
}

// TODO: Proper implementation
void Log::OutMessage(const std::wstring &swMessage) const {
	MessageBoxW(nullptr, swMessage.c_str(), L"Message", MB_OK);
}

void Log::OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
	uint32 uLine, const char *pszFormat, ...) const {
	std::string sBuffer;
	va_list vaArgs = nullptr;
	va_start(vaArgs, pszFormat);
	size_t length = _vscprintf(pszFormat, vaArgs);
	sBuffer.resize(length);
	_vsnprintf(&sBuffer[0], length, pszFormat, vaArgs);
	va_end(vaArgs);

	std::wostringstream o;
#ifdef DEBUG_USE_FUNCTIONNAMES
	o << pswzFunction << L' ';
#endif

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	o << L'(' << pswzFile << L':' << uLine << L") - "
	  << conv.from_bytes(sBuffer);
	OutDebug(o.str());
}

void Log::OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
	uint32 uLine, const wchar_t *pswzFormat, ...) const {
	std::wstring swBuffer;
	va_list vaArgs = nullptr;
	va_start(vaArgs, pswzFormat);
	size_t length = _vscwprintf(pswzFormat, vaArgs);
	swBuffer.resize(length);
	_vsnwprintf(&swBuffer[0], length, pswzFormat, vaArgs);
	va_end(vaArgs);

	std::wostringstream o;
#ifdef DEBUG_USE_FUNCTIONNAMES
	o << pswzFunction << L' ';
#endif

	o << L'(' << pswzFile << L':' << uLine << L") - "
	  << swBuffer;
	OutDebug(o.str());
}
