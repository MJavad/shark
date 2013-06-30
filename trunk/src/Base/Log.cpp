#include "Misc/stdafx.h"
#include "Log.h"

void Log::Initialize() {

}

// TODO: Proper implementation
void Log::OutDebug(const std::wstring &swMessage) const {
	std::wcout << swMessage << std::endl;
}

// TODO: Proper implementation
void Log::OutMessage(const std::wstring &swMessage) const {
	std::wcout << swMessage << std::endl;
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
	if (pswzFunction != nullptr)
		o << pswzFunction << L' ';

	if (pswzFile != nullptr) {
		if (pswzFunction != nullptr)
			o << L'(';

		o << pswzFile << L':' << uLine;

		if (pswzFunction != nullptr)
			o << L')';

		o << L' ';
	}

	if (pswzFunction != nullptr || pswzFile != nullptr)
		o << L"- ";

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	o << conv.from_bytes(sBuffer);
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
	if (pswzFunction != nullptr)
		o << pswzFunction << L' ';

	if (pswzFile != nullptr) {
		if (pswzFunction != nullptr)
			o << L'(';

		o << pswzFile << L':' << uLine;

		if (pswzFunction != nullptr)
			o << L')';

		o << L' ';
	}

	if (pswzFunction != nullptr || pswzFile != nullptr)
		o << L"- ";

	o << swBuffer;
	OutDebug(o.str());
}
