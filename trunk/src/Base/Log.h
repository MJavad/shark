#pragma once

class Log {
	SINGLETON_CLASS(Log) {}

public:
	void Initialize();
	void OutDebug(const std::wstring &swMessage) const;
	void OutMessage(const std::wstring &swMessage) const;

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
