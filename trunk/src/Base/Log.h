#pragma once

class Log : public Utils::Singleton<Log>
{
public:
	void Initialize();
	void OutDebug(const std::wstring &swMessage) const;
	void OutMessage(const std::wstring &swMessage) const;

	void OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
		uint32 uLine, const char *pszFormat, ...) const;

	void OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
		uint32 uLine, const wchar_t *pswzFormat, ...) const;
};

#define sLog Utils::Singleton<Log>::Instance()
