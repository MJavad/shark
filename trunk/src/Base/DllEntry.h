#pragma once

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
BOOL WINAPI LoadInternal(LPVOID lpParam);
BOOL WINAPI UnloadInternal(LPVOID lpParam);
