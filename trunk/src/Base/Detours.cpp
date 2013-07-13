#include "Misc/stdafx.h"
#include "Detours.h"
#include "Delegates.h"
#include "D3DManager.h"
#include "DllEntry.h"
#include "Engine.h"
#include "WndProc.h"

void Detours::Initialize() {
	ApplyDetour(sDelegates.IDirect3DDevice9__EndScene, IDirect3DDevice9__EndScene);
	ApplyDetour(sDelegates.IDirect3DDevice9__Reset, IDirect3DDevice9__Reset);
	ApplyDetour(sDelegates.IDirect3DDevice9Ex__ResetEx, IDirect3DDevice9Ex__ResetEx);
	ApplyDetour(sDelegates.IDirect3D9__CreateDevice, IDirect3D9__CreateDevice);
	ApplyDetour(sDelegates.IDirect3D9Ex__CreateDeviceEx, IDirect3D9Ex__CreateDeviceEx);
	ApplyDetour(sDelegates.IDXGISwapChain__Present, IDXGISwapChain__Present);
	ApplyDetour(sDelegates.ID3D11DeviceContext__ClearRenderTargetView, ID3D11DeviceContext__ClearRenderTargetView);
	ApplyDetour(sDelegates.D3D11CreateDeviceAndSwapChain, D3D11CreateDeviceAndSwapChain);
}

HRESULT CALLBACK Detours::IDirect3DDevice9__EndScene(IDirect3DDevice9 *pDevice) {
	if (sD3DMgr.GetDevice9() != pDevice)
		sD3DMgr.SetDevice9(pDevice);
	
	if (!sEngine.IsInitialized())
		sEngine.InitializeEnvironment();

	if (sD3DMgr.IsCreated())
		sD3DMgr.OnRender();

	sEngine.OnPulse();
	HRESULT hFrame = sDelegates.IDirect3DDevice9__EndScene(pDevice);

	// Shutdown?
	if (sEngine.IsShuttingDown())
		sEngine.OnShutDown();

	return hFrame;
}

HRESULT CALLBACK Detours::IDirect3DDevice9__Reset(IDirect3DDevice9 *pDevice,
												  D3DPRESENT_PARAMETERS *pParams)
{
	LOG_DEBUG("D3D9Device Reset called!");

	if (sD3DMgr.IsCreated())
		sD3DMgr.OnLostDevice();

	HRESULT result = sDelegates.IDirect3DDevice9__Reset(pDevice, pParams);

	if (result != D3D_OK)
		LOG_DEBUG("D3D9Device Reset failed. Return value: %u.", result);

	else if (sD3DMgr.IsCreated())
		sD3DMgr.OnResetDevice();

	return result;
}

HRESULT CALLBACK Detours::IDirect3DDevice9Ex__ResetEx(IDirect3DDevice9Ex *pDeviceEx,
													  D3DPRESENT_PARAMETERS *pParams,
													  D3DDISPLAYMODEEX *pDisplayMode)
{
	LOG_DEBUG("D3D9ExDevice ResetEx called!");

	if (sD3DMgr.IsCreated())
		sD3DMgr.OnLostDevice();

	HRESULT result = sDelegates.IDirect3DDevice9Ex__ResetEx(pDeviceEx, pParams, pDisplayMode);

	if (result != D3D_OK)
		LOG_DEBUG("D3D9ExDevice ResetEx failed. Return value: %u.", result);

	else if (sD3DMgr.IsCreated())
		sD3DMgr.OnResetDevice();

	return result;
}

HRESULT CALLBACK Detours::IDirect3D9__CreateDevice(IDirect3D9 *pD3D9,
												   UINT Adapter,
												   D3DDEVTYPE DeviceType,
												   HWND hFocusWindow,
												   DWORD dwBehaviorFlags,
												   D3DPRESENT_PARAMETERS *pParams,
												   IDirect3DDevice9 **ppReturnedDevice)
{
	LOG_DEBUG("D3D9 CreateDevice called!");
	HRESULT result = sDelegates.IDirect3D9__CreateDevice(pD3D9, Adapter, DeviceType,
		hFocusWindow, dwBehaviorFlags, pParams, ppReturnedDevice);

	if (result != D3D_OK)
		LOG_DEBUG("D3D9 CreateDevice failed. Return value: %u.", result);
	else
		sD3DMgr.SetDevice9(*ppReturnedDevice);

	return result;
}

HRESULT CALLBACK Detours::IDirect3D9Ex__CreateDeviceEx(IDirect3D9Ex *pD3D9Ex,
													   UINT Adapter,
													   D3DDEVTYPE DeviceType,
													   HWND hFocusWindow,
													   DWORD dwBehaviorFlags,
													   D3DPRESENT_PARAMETERS *pParams,
													   D3DDISPLAYMODEEX *pDisplayMode,
													   IDirect3DDevice9Ex **ppReturnedDevice)
{
	LOG_DEBUG("D3D9Ex CreateDeviceEx called!");
	HRESULT result = sDelegates.IDirect3D9Ex__CreateDeviceEx(pD3D9Ex, Adapter, DeviceType,
		hFocusWindow, dwBehaviorFlags, pParams, pDisplayMode, ppReturnedDevice);

	if (result != S_OK)
		LOG_DEBUG("D3D9Ex CreateDeviceEx failed. Return value: %u.", result);
	else
		sD3DMgr.SetDevice9(*ppReturnedDevice);

	return result;
}

HRESULT CALLBACK Detours::IDXGISwapChain__Present(IDXGISwapChain *pSwapChain,
												  UINT SyncInterval,
												  UINT Flags)
{
	if (sD3DMgr.GetSwapChain() != pSwapChain)
		sD3DMgr.SetSwapChain(pSwapChain);
	
	if (!sEngine.IsInitialized())
		sEngine.InitializeEnvironment();

	if (sD3DMgr.IsCreated())
		sD3DMgr.OnRender();

	sEngine.OnPulse();
	HRESULT hFrame = sDelegates.IDXGISwapChain__Present(pSwapChain, SyncInterval, Flags);

	// Shutdown?
	if (sEngine.IsShuttingDown())
		sEngine.OnShutDown();

	return hFrame;
}

void CALLBACK Detours::ID3D11DeviceContext__ClearRenderTargetView(ID3D11DeviceContext *pDeviceContext,
																  ID3D11RenderTargetView *pRenderTargetView,
																  const FLOAT ColorRGBA[4])
{
	if (sD3DMgr.GetDeviceContext11() != pDeviceContext) {
		sD3DMgr.SetDeviceContext11(pDeviceContext);

		CComPtr<ID3D11Device> pDevice = nullptr;
		pDeviceContext->GetDevice(&pDevice);
		if (sD3DMgr.GetDevice11() != pDevice)
			sD3DMgr.SetDevice11(pDevice);
	}

	return sDelegates.ID3D11DeviceContext__ClearRenderTargetView(pDeviceContext, pRenderTargetView, ColorRGBA);
}

HRESULT WINAPI Detours::D3D11CreateDeviceAndSwapChain(IDXGIAdapter *pAdapter,
													  D3D_DRIVER_TYPE DriverType,
													  HMODULE Software,
													  UINT Flags,
													  const D3D_FEATURE_LEVEL *pFeatureLevels,
													  UINT FeatureLevels,
													  UINT SDKVersion,
													  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
													  IDXGISwapChain **ppSwapChain,
													  ID3D11Device **ppDevice,
													  D3D_FEATURE_LEVEL *pFeatureLevel,
													  ID3D11DeviceContext **ppImmediateContext)
{
	LOG_DEBUG("D3D11 CreateDeviceAndSwapChain called!");

	sD3DMgr.SetDevice11(nullptr);
	sD3DMgr.SetDeviceContext11(nullptr);
	sD3DMgr.SetSwapChain(nullptr);

	HRESULT result = sDelegates.D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels,
		FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

	sD3DMgr.SetDevice11(*ppDevice);
	sD3DMgr.SetDeviceContext11(*ppImmediateContext);
	sD3DMgr.SetSwapChain(*ppSwapChain);
	return result;
}
