#pragma once
#undef GetModuleFileName

class FileManager {
	SINGLETON_CLASS(FileManager) {}

public:
	void Initialize();

	std::wstring GetModuleFileName() const {
		return m_moduleFileName;
	}

	std::wstring GetLogsDirectory() const {
		return m_logsDirectory;
	}

	std::wstring GetModuleDirectory() const {
		return m_moduleDirectory;
	}

	std::wstring GetScriptsDirectory() const {
		return m_scriptsDirectory;
	}

private:
	std::wstring m_moduleFileName;
	std::wstring m_moduleDirectory;
	std::wstring m_logsDirectory;
	std::wstring m_scriptsDirectory;
};

#define sFileMgr ::FileManager::Instance()
