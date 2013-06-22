#include "Misc/stdafx.h"
#include "Offsets.h"
#include "Engine.h"
#include "Misc/DebugHelper.h"

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
	IDirect3D9Ex__CreateDeviceEx = 0;
	IDirect3DDevice9__EndScene = 0;
	IDirect3DDevice9__Reset = 0;
	IDirect3DDevice9Ex__ResetEx = 0;
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
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.lpszClassName = L"D3DShark_D3DInit";
	ATOM hAtom = RegisterClassW(&wndClass);

	if (hAtom == NULL)
		LOG_DEBUG("Could not register window class.");

	HWND hWindow = CreateWindow(MAKEINTATOM(hAtom), nullptr, 0, 0, 0,
		0, 0, nullptr, nullptr, wndClass.hInstance, nullptr);

	if (hWindow == nullptr)
		LOG_DEBUG("Could not create dummy window.");

	// DirectX 9 - TODO: Think about something better instead of LoadLibrary here...
	HMODULE d3d9Handle = LoadLibraryW(L"d3d9.dll");
	if (d3d9Handle != nullptr) {
		typedef IDirect3D9* (WINAPI *Direct3DCreate9_t) (UINT SDKVersion);
		Direct3DCreate9_t pDirect3DCreate9 = reinterpret_cast<Direct3DCreate9_t>
			(GetProcAddress(d3d9Handle, "Direct3DCreate9"));

		if (pDirect3DCreate9 != nullptr) {
			IDirect3D9 *pD3D9 = pDirect3DCreate9(D3D_SDK_VERSION);
			if (pD3D9 != nullptr) {
				DWORD_PTR **ppD3D9VMT = reinterpret_cast<DWORD_PTR**>(pD3D9);
				IDirect3D9__CreateDevice = (*ppD3D9VMT) [16];
				pD3D9->Release();
			}
			else
				LOG_DEBUG("D3D9: Direct3DCreate9 failed.");
		}

		typedef HRESULT (WINAPI *Direct3DCreate9Ex_t) (_In_ UINT SDKVersion, _Out_ IDirect3D9Ex **ppD3D);
		Direct3DCreate9Ex_t pDirect3DCreate9Ex = reinterpret_cast<Direct3DCreate9Ex_t>
			(GetProcAddress(d3d9Handle, "Direct3DCreate9Ex"));

		if (pDirect3DCreate9Ex != nullptr) {
			IDirect3D9Ex *pD3D9Ex = nullptr;
			if (pDirect3DCreate9Ex(D3D_SDK_VERSION, &pD3D9Ex) == S_OK) {
				DWORD_PTR **d3d9ExVMT = reinterpret_cast<DWORD_PTR**>(pD3D9Ex);
				IDirect3D9Ex__CreateDeviceEx = (*d3d9ExVMT) [20];

				D3DPRESENT_PARAMETERS presentParams = {0};
				presentParams.BackBufferCount = 1;
				presentParams.BackBufferFormat = D3DFMT_UNKNOWN;
				presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
				presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
				presentParams.hDeviceWindow = hWindow;
				presentParams.Windowed = TRUE;
				presentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

				IDirect3DDevice9Ex *pDevice9Ex = nullptr;
				if (pD3D9Ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow,
						D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParams,
						nullptr, &pDevice9Ex) == S_OK)
				{
					DWORD_PTR **device9ExVMT = reinterpret_cast<DWORD_PTR**>(pDevice9Ex);
					IDirect3DDevice9Ex__ResetEx = (*device9ExVMT) [132];
					pDevice9Ex->Release();
				}
				else
					LOG_DEBUG("D3D9Ex: CreateDeviceEx failed.");

				pD3D9Ex->Release();
			}
			else
				LOG_DEBUG("D3D9Ex: Direct3DCreate9Ex failed.");
		}

		PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(d3d9Handle);
		PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>
			(reinterpret_cast<DWORD_PTR>(d3d9Handle) + pDosHeader->e_lfanew);

		// Points to somewhere inside the CD3DBase constructor
		const byte d3dBasePattern[] = {0x8B, 0x00,						   // mov register, register
									   0xE8, 0x00, 0x00, 0x00, 0x00,	   // call offset
									   0x33, 0xC0,						   // xor eax, eax
									   0xC7, 0x06, 0x00, 0x00, 0x00, 0x00, // mov [esi], offset <- vmt
									   0x89, 0x86, 0x00, 0x00, 0x00, 0x00, // mov [esi+offset], eax
									   0x89, 0x86};						   // mov [esi+offset], eax

		DWORD_PTR d3dBaseOffset = sMemory.FindMemoryPattern(
			pNtHeaders->OptionalHeader.ImageBase + pNtHeaders->OptionalHeader.BaseOfCode,
			pNtHeaders->OptionalHeader.SizeOfCode, d3dBasePattern, "x?x????xxxx????xx????xx");

		if (d3dBaseOffset != 0) {
			DWORD_PTR **d3dBaseVMT = reinterpret_cast<DWORD_PTR**>(d3dBaseOffset + 0x0B);
			IDirect3DDevice9__EndScene = (*d3dBaseVMT) [42];
			IDirect3DDevice9__Reset = (*d3dBaseVMT) [16];
		}
		else
			LOG_DEBUG("D3D9: Could not find CD3DBase vm-table.");
	}

	// DirectX 11 - Optional for now
	HMODULE d3d11Handle = GetModuleHandleW(L"d3d11.dll");
	if (d3d11Handle != nullptr) {
		ID3D11Device *pDevice11 = nullptr;
		ID3D11DeviceContext *pDeviceContext11 = nullptr;
		IDXGISwapChain *pSwapChain = nullptr;

		DXGI_SWAP_CHAIN_DESC swapDesc = {0};
		D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL featuresSupported;

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = hWindow;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.Windowed = TRUE;

		typedef HRESULT (WINAPI *D3D11CreateDeviceAndSwapChain_t) (IDXGIAdapter *pAdapter, D3D_DRIVER_TYPE DriverType,
			HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL *pFeatureLevels, UINT FeatureLevels,
			UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc, IDXGISwapChain **ppSwapChain,
			ID3D11Device **ppDevice, D3D_FEATURE_LEVEL *pFeatureLevel, ID3D11DeviceContext **ppImmediateContext);

		D3D11CreateDeviceAndSwapChain = reinterpret_cast<DWORD_PTR>
			(GetProcAddress(d3d11Handle, "D3D11CreateDeviceAndSwapChain"));

		D3D11CreateDeviceAndSwapChain_t pD3D11CreateDeviceAndSwapChain =
			reinterpret_cast<D3D11CreateDeviceAndSwapChain_t>(D3D11CreateDeviceAndSwapChain);

		if (pD3D11CreateDeviceAndSwapChain != nullptr &&
			pD3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
				nullptr, 0, &featureLevels, 1, D3D11_SDK_VERSION, &swapDesc, &pSwapChain,
				&pDevice11, &featuresSupported, &pDeviceContext11) == S_OK)
		{
			DWORD_PTR **swapChain11VMT = reinterpret_cast<DWORD_PTR**>(pSwapChain);
			DWORD_PTR **deviceContext11VMT = reinterpret_cast<DWORD_PTR**>(pDeviceContext11);
			IDXGISwapChain__Present = (*swapChain11VMT) [8];
			ID3D11DeviceContext__ClearRenderTargetView = (*deviceContext11VMT) [50];

			pDevice11->Release();
			pDeviceContext11->Release();
			pSwapChain->Release();
		}
		else
			LOG_DEBUG("D3D11: D3D11CreateDeviceAndSwapChain failed.");
	}

	if (hWindow != nullptr)
		DestroyWindow(hWindow);

	if (hAtom != NULL)
		UnregisterClass(MAKEINTATOM(hAtom), wndClass.hInstance);
}
