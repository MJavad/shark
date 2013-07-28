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
#include "FileManager.h"
#include "Engine.h"

void FileManager::Initialize() {
	m_moduleFileName.resize(MAX_PATH);
	m_moduleFileName.resize(GetModuleFileNameW(
		sEngine.GetInstance(), &m_moduleFileName[0], MAX_PATH));

	auto filePathEnd = m_moduleFileName.find_last_of(L'\\');
	m_moduleDirectory = m_moduleFileName.substr(0, filePathEnd);
	m_logsDirectory = m_moduleDirectory + L"\\logs";
	m_scriptsDirectory = m_moduleDirectory + L"\\scripts";
	CreateDirectoryW(m_logsDirectory.c_str(), nullptr);
}
