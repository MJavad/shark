/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

class Delegates {
	SINGLETON_CLASS(Delegates) :
		IDirect3DDevice9__EndScene(nullptr),
		IDirect3DDevice9__Reset(nullptr),
		IDirect3DDevice9Ex__ResetEx(nullptr),
		IDirect3D9__CreateDevice(nullptr),
		IDirect3D9Ex__CreateDeviceEx(nullptr),
		IDXGISwapChain__Present(nullptr),
		ID3D11DeviceContext__ClearRenderTargetView(nullptr),
		D3D11CreateDeviceAndSwapChain(nullptr) {}

public:
	void Initialize();

	typedef HRESULT (CALLBACK *IDirect3DDevice9__EndScene_t) (IDirect3DDevice9 *pDevice);
	IDirect3DDevice9__EndScene_t IDirect3DDevice9__EndScene;

	typedef HRESULT (CALLBACK *IDirect3DDevice9__Reset_t) (IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pParams);
	IDirect3DDevice9__Reset_t IDirect3DDevice9__Reset;

	typedef HRESULT (CALLBACK *IDirect3DDevice9Ex__ResetEx_t) (IDirect3DDevice9Ex *pDeviceEx, D3DPRESENT_PARAMETERS *pParams, D3DDISPLAYMODEEX *pDisplayMode);
	IDirect3DDevice9Ex__ResetEx_t IDirect3DDevice9Ex__ResetEx;

	typedef HRESULT (CALLBACK *IDirect3D9__CreateDevice_t) (IDirect3D9 *pD3D9, UINT Adapter, D3DDEVTYPE DeviceType,
		HWND hFocusWindow, DWORD dwBehaviorFlags, D3DPRESENT_PARAMETERS *pParams, IDirect3DDevice9 **ppReturnedDevice);
	IDirect3D9__CreateDevice_t IDirect3D9__CreateDevice;

	typedef HRESULT (CALLBACK *IDirect3D9Ex__CreateDeviceEx_t) (IDirect3D9Ex *pD3D9Ex, UINT Adapter,
		D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD dwBehaviorFlags, D3DPRESENT_PARAMETERS *pParams,
		D3DDISPLAYMODEEX *pDisplayMode, IDirect3DDevice9Ex **ppReturnedDevice);
	IDirect3D9Ex__CreateDeviceEx_t IDirect3D9Ex__CreateDeviceEx;

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

#define sDelegates ::Delegates::Instance()
