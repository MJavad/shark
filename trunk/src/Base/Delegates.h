#pragma once

class Delegates : public Utils::Singleton
{
	SINGLETON_OBJECT
	Delegates() : IDirect3DDevice9__EndScene(nullptr),
				  IDirect3DDevice9__Reset(nullptr),
				  IDirect3D9__CreateDevice(nullptr),
				  IDXGISwapChain__Present(nullptr),
				  ID3D11DeviceContext__ClearRenderTargetView(nullptr),
				  D3D11CreateDeviceAndSwapChain(nullptr) {}

public:
	void Initialize();

	typedef HRESULT (CALLBACK *IDirect3DDevice9__EndScene_t) (IDirect3DDevice9 *pDevice);
	IDirect3DDevice9__EndScene_t IDirect3DDevice9__EndScene;

	typedef HRESULT (CALLBACK *IDirect3DDevice9__Reset_t) (IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pParams);
	IDirect3DDevice9__Reset_t IDirect3DDevice9__Reset;

	typedef HRESULT (CALLBACK *IDirect3D9__CreateDevice_t) (IDirect3D9 *pD3D9, UINT Adapter, D3DDEVTYPE DeviceType,
		HWND hFocusWindow, DWORD dwBehaviorFlags, D3DPRESENT_PARAMETERS *pParams, IDirect3DDevice9 **ppReturnedDevice);
	IDirect3D9__CreateDevice_t IDirect3D9__CreateDevice;

	typedef HRESULT (CALLBACK *IDXGISwapChain__Present_t) (IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags);
	IDXGISwapChain__Present_t IDXGISwapChain__Present;

	typedef void (CALLBACK *ID3D11DeviceContext__ClearRenderTargetView_t) (ID3D11DeviceContext *pDeviceContext,
		ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);
	ID3D11DeviceContext__ClearRenderTargetView_t ID3D11DeviceContext__ClearRenderTargetView;

	typedef HRESULT (WINAPI *D3D11CreateDeviceAndSwapChain_t) (IDXGIAdapter *pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
		UINT Flags, const D3D_FEATURE_LEVEL *pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
		IDXGISwapChain **ppSwapChain, ID3D11Device **ppDevice, D3D_FEATURE_LEVEL *pFeatureLevel, ID3D11DeviceContext **ppImmediateContext);
	D3D11CreateDeviceAndSwapChain_t D3D11CreateDeviceAndSwapChain;
};

#define sDelegates GET_INSTANCE(Delegates)
