#pragma once

class Offsets : public Utils::Singleton<Offsets>
{
public:
	void Initialize();

	struct SModule
	{
		SModule();
		void Initialize();

		HINSTANCE MainModule;
		HINSTANCE CurrentInstance;
		PIMAGE_NT_HEADERS MainNtHeaders;
		PIMAGE_NT_HEADERS CurrentNtHeaders;
		PIMAGE_DOS_HEADER MainDosHeader;
		PIMAGE_DOS_HEADER CurrentDosHeader;
	};
	SModule Module;

	struct SDirectX
	{
		SDirectX();
		void Initialize();

		DWORD_PTR IDirect3D9__CreateDevice;
		DWORD_PTR IDirect3DDevice9__EndScene;
		DWORD_PTR IDirect3DDevice9__Reset;

		DWORD_PTR D3D11CreateDeviceAndSwapChain;
		DWORD_PTR IDXGISwapChain__Present;
		DWORD_PTR ID3D11DeviceContext__ClearRenderTargetView;
	};
	SDirectX DirectX;
};

extern Offsets sOffsets;
