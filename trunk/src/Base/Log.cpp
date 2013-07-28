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
#include "Log.h"
#include "UI/ConsoleWindow.h"

void Log::Initialize() {

}

// TODO: Proper implementation
void Log::OutDebug(const std::wstring &swMessage) const {
	std::wcout << swMessage << std::endl;
	sConsoleWindow.AddLine(swMessage);
}

void Log::OutDebug_UTF8(const std::string &sMessage) const {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	OutDebug(conv.from_bytes(sMessage));
}

// TODO: Proper implementation
void Log::OutMessage(const std::wstring &swMessage) const {
	std::wcout << swMessage << std::endl;
}

void Log::OutMessage_UTF8(const std::string &sMessage) const {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	OutMessage(conv.from_bytes(sMessage));
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
