#pragma once

class Log : public Utils::Singleton<Log>
{
public:
	void Initialize();
	void OutDebug(const std::wstring &swMessage) const;
	void OutMessage(const std::wstring &swMessage) const;
};

#define sLog Utils::Singleton<Log>::Instance()
