#pragma once

class Detours : public Utils::Singleton<Detours>
{
public:
	Detours() {}
	void Initialize();

	Utils::TimerDispatcher FrameTimer;
	Utils::Event<void ()> OnFrameEvent;
	Utils::Event<void (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> OnLostDeviceEvent;
	Utils::Event<void (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> OnResetDeviceEvent;

private:
	static HRESULT CALLBACK IDirect3DDevice9__EndScene(IDirect3DDevice9 *pDevice);
	static HRESULT CALLBACK IDirect3DDevice9__Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pParams);
	static HRESULT CALLBACK IDirect3D9__CreateDevice(IDirect3D9 *pD3D9, UINT Adapter,
		D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD dwBehaviorFlags,
		D3DPRESENT_PARAMETERS *pParams, IDirect3DDevice9 **ppReturnedDevice);

	static HRESULT CALLBACK IDXGISwapChain__Present(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags);
	static void CALLBACK ID3D11DeviceContext__ClearRenderTargetView(ID3D11DeviceContext *pDeviceContext,
		ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);
	static HRESULT WINAPI D3D11CreateDeviceAndSwapChain(IDXGIAdapter *pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
		UINT Flags, const D3D_FEATURE_LEVEL *pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
		IDXGISwapChain **ppSwapChain, ID3D11Device **ppDevice, D3D_FEATURE_LEVEL *pFeatureLevel, ID3D11DeviceContext **ppImmediateContext);

	template <typename T>
	void ApplyDetour(T &tDelegate, void *pRedirect) const {
		if (tDelegate != nullptr && pRedirect != nullptr)
			sMemory->DetourFunction(reinterpret_cast<void**>(&tDelegate), pRedirect);
	}
};

#define sDetours Utils::Singleton<Detours>::Instance()
