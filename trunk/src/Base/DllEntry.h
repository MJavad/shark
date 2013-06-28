#pragma once

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
BOOL WINAPI LoadInternal(LPVOID lpParam);
BOOL WINAPI UnloadInternal(LPVOID lpParam);
LONG WINAPI InternalExceptionFilter(PEXCEPTION_POINTERS pInfo);
void SetupDebugConsole();
