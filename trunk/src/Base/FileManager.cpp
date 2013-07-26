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
