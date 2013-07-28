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
