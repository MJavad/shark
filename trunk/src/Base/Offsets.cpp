#include "Misc/stdafx.h"
#include "Offsets.h"
#include "Engine.h"
#include "Misc/DebugHelper.h"

INIT_SINGLETON(Offsets);
Offsets sOffsets;

Offsets::SModule::SModule() {
	MainModule = nullptr;
	CurrentInstance = nullptr;
	MainNtHeaders = nullptr;
	CurrentNtHeaders = nullptr;
	MainDosHeader = nullptr;
	CurrentDosHeader = nullptr;
}

Offsets::SDirectX::SDirectX() {
	IDirect3D9__CreateDevice = 0;
	IDirect3DDevice9__EndScene = 0;
	IDirect3DDevice9__Reset = 0;
	D3D11CreateDeviceAndSwapChain = 0;
	IDXGISwapChain__Present = 0;
	ID3D11DeviceContext__ClearRenderTargetView = 0;
}

void Offsets::Initialize() {
	Module.Initialize();
	DirectX.Initialize();
}

void Offsets::SModule::Initialize() {
	MainModule = GetModuleHandleW(nullptr);
	DWORD_PTR dwMainModule = reinterpret_cast<DWORD_PTR>(MainModule);
	MainDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(MainModule);
	MainNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwMainModule + MainDosHeader->e_lfanew);
	
	CurrentInstance = sEngine.GetInstance();
	DWORD_PTR dwCurrentModule = reinterpret_cast<DWORD_PTR>(CurrentInstance);
	CurrentDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(CurrentInstance);
	CurrentNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwCurrentModule + CurrentDosHeader->e_lfanew);
}

void Offsets::SDirectX::Initialize() {
	WNDCLASSW wndClass = {0};
	wndClass.lpfnWndProc = DefWindowProcW;
	wndClass.hInstance = sEngine.GetInstance();
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
	wndClass.lpszClassName = L"D3DShark_D3DInit";

	ATOM hAtom = RegisterClassW(&wndClass);
	if (hAtom != FALSE) {
		HWND hWindow = CreateWindowW(MAKEINTATOM(hAtom), nullptr, 0, 0, 0,
			0, 0, nullptr, nullptr, wndClass.hInstance, nullptr);

		if (hWindow != nullptr) {
			// DirectX 9
			IDirect3D9 *pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
			if (pD3D9 != nullptr) {
				DWORD_PTR **ppD3D9VMT = reinterpret_cast<DWORD_PTR**>(pD3D9);
				IDirect3D9__CreateDevice = (*ppD3D9VMT) [16];

				IDirect3DDevice9 *pDevice9 = nullptr;
				D3DPRESENT_PARAMETERS presentParams = {0};
				presentParams.Windowed = true;
				presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
				presentParams.BackBufferFormat = D3DFMT_X8R8G8B8;

				if (pD3D9->CreateDevice(D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL, hWindow,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
						&presentParams, &pDevice9 ) == D3D_OK) {
					DWORD_PTR **ppDevice9VMT = reinterpret_cast<DWORD_PTR**>(pDevice9);
					IDirect3DDevice9__EndScene = (*ppDevice9VMT) [42];
					IDirect3DDevice9__Reset = (*ppDevice9VMT) [16];
					pDevice9->Release();
				}
				else {
					DWORD_PTR dwModuleBase = reinterpret_cast<DWORD_PTR>(GetModuleHandleW(L"d3d9.dll"));
					if (dwModuleBase != 0) {
						PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dwModuleBase);
						PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwModuleBase + pDosHeader->e_lfanew);
					
						const byte devicePattern[] = {0xC7, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0x86, 0x00, 0x00, 0x00, 0x00, 0x89, 0x86};
						DWORD_PTR dwVtblAddr = sMemory.FindMemoryPattern(dwModuleBase,
							pNtHeaders->OptionalHeader.SizeOfCode, devicePattern, "xx????xx????xx");

						if (dwVtblAddr != 0) {
							DWORD_PTR **ppDevice9VMT = reinterpret_cast<DWORD_PTR**>(dwVtblAddr + 2);
							IDirect3DDevice9__EndScene = (*ppDevice9VMT) [42];
							IDirect3DDevice9__Reset = (*ppDevice9VMT) [16];
						}
						else
							LOG_DEBUG("Could not find CD3DBase VTable.");
					}
					else
						LOG_DEBUG("Could not find d3d9.dll");
				}

				pD3D9->Release();
			}

			// DirectX 11
			ID3D11Device *pDevice11 = nullptr;
			ID3D11DeviceContext *pDeviceContext11 = nullptr;
			IDXGISwapChain *pSwapChain = nullptr;

			DXGI_SWAP_CHAIN_DESC swapDesc = {0};
			D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
			D3D_FEATURE_LEVEL featuresSupported;
			swapDesc.BufferCount = 1;
			swapDesc.BufferDesc.Width = 0;
			swapDesc.BufferDesc.Height = 0;
			swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapDesc.OutputWindow = hWindow;
			swapDesc.SampleDesc.Count = 1;
			swapDesc.SampleDesc.Quality = 0;
			swapDesc.Windowed = TRUE;

			HMODULE hD3D11Dll = GetModuleHandleW(L"d3d11.dll");
			if (hD3D11Dll != nullptr) {
				typedef HRESULT (WINAPI *D3D11CreateDeviceAndSwapChain_t) (IDXGIAdapter *pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
					UINT Flags, const D3D_FEATURE_LEVEL *pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
					IDXGISwapChain **ppSwapChain, ID3D11Device **ppDevice, D3D_FEATURE_LEVEL *pFeatureLevel, ID3D11DeviceContext **ppImmediateContext);

				D3D11CreateDeviceAndSwapChain = reinterpret_cast<DWORD_PTR>(GetProcAddress(hD3D11Dll, "D3D11CreateDeviceAndSwapChain"));
				D3D11CreateDeviceAndSwapChain_t pD3D11CreateDeviceAndSwapChain = reinterpret_cast<D3D11CreateDeviceAndSwapChain_t>(D3D11CreateDeviceAndSwapChain);

				if (pD3D11CreateDeviceAndSwapChain != nullptr &&
					pD3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
						nullptr, 0, &featureLevels, 1, D3D11_SDK_VERSION, &swapDesc, &pSwapChain,
						&pDevice11, &featuresSupported, &pDeviceContext11) == S_OK) {

					DWORD_PTR **ppSwapChainVMT = reinterpret_cast<DWORD_PTR**>(pSwapChain);
					DWORD_PTR **ppDeviceContext11VMT = reinterpret_cast<DWORD_PTR**>(pDeviceContext11);
					IDXGISwapChain__Present = (*ppSwapChainVMT) [8];
					ID3D11DeviceContext__ClearRenderTargetView = (*ppDeviceContext11VMT) [50];

					pDevice11->Release();
					pDeviceContext11->Release();
					pSwapChain->Release();
				}
			}

			DestroyWindow(hWindow);
		}
		UnregisterClassW(MAKEINTATOM(hAtom), wndClass.hInstance);
	}
}
