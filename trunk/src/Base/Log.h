#pragma once

class Log : public Utils::Singleton
{
	SINGLETON_OBJECT
	Log() {}

public:
	void Initialize();
	void OutDebug(const std::wstring &swMessage) const;
	void OutMessage(const std::wstring &swMessage) const;

	void OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
		uint32 uLine, const char *pszFormat, ...) const;

	void OutDebugFormatted(const wchar_t *pswzFunction, const wchar_t *pswzFile,
		uint32 uLine, const wchar_t *pswzFormat, ...) const;
};

#define sLog GET_INSTANCE(Log)

#ifdef DEBUG_USE_FUNCTIONNAMES
#define LOG_DEBUG(fmt, ...) sLog.OutDebugFormatted(__FUNCTIONW__, __FILEW__, __LINE__, fmt, __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) sLog.OutDebugFormatted(nullptr, __FILEW__, __LINE__, fmt, __VA_ARGS__)
#endif
