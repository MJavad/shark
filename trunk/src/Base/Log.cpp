#include "Misc/stdafx.h"
#include "Log.h"

INIT_SINGLETON(Log);

void Log::Initialize() {

}

// TODO: Proper implementation
void Log::OutDebug(const std::wstring &swMessage) const {
	MessageBoxW(nullptr, swMessage.c_str(), L"Debug Message", MB_OK);
}

// TODO: Proper implementation
void Log::OutMessage(const std::wstring &swMessage) const {
	MessageBoxW(nullptr, swMessage.c_str(), L"Message", MB_OK);
}
