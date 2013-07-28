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

class Log {
	SINGLETON_CLASS(Log) {}

public:
	void Initialize();
	void OutDebug(const std::wstring &swMessage) const;
	void OutDebug_UTF8(const std::string &sMessage) const;
	void OutMessage(const std::wstring &swMessage) const;
	void OutMessage_UTF8(const std::string &sMessage) const;

	void OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
		uint32 uLine, const char *pszFormat, ...) const;

	void OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
		uint32 uLine, const wchar_t *pswzFormat, ...) const;
};

#define sLog ::Log::Instance()

#ifdef DEBUG_USE_LOG
	#ifdef DEBUG_USE_FUNCTIONNAMES
		#ifdef DEBUG_USE_FILENAMES
			#define LOG_DEBUG(fmt, ...) (sLog.OutDebugFormatted(__FUNCTIONW__, __FILEW__, __LINE__, fmt, __VA_ARGS__))
		#else
			#define LOG_DEBUG(fmt, ...) (sLog.OutDebugFormatted(__FUNCTIONW__, nullptr, 0, fmt, __VA_ARGS__))
		#endif
	#else
		#ifdef DEBUG_USE_FILENAMES
			#define LOG_DEBUG(fmt, ...) (sLog.OutDebugFormatted(nullptr, __FILEW__, __LINE__, fmt, __VA_ARGS__))
		#else
			#define LOG_DEBUG(fmt, ...) (sLog.OutDebugFormatted(nullptr, nullptr, 0, fmt, __VA_ARGS__))
		#endif
	#endif
#else
	#define LOG_DEBUG(fmt, ...) ((void)0)
#endif
