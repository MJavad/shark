#include "Misc/stdafx.h"
#include "Delegates.h"
#include "Offsets.h"

void Delegates::Initialize() {
	IDirect3DDevice9__EndScene = reinterpret_cast<IDirect3DDevice9__EndScene_t>
		(sOffsets.DirectX.IDirect3DDevice9__EndScene);

	IDirect3DDevice9__Reset = reinterpret_cast<IDirect3DDevice9__Reset_t>
		(sOffsets.DirectX.IDirect3DDevice9__Reset);

	IDirect3D9__CreateDevice = reinterpret_cast<IDirect3D9__CreateDevice_t>
		(sOffsets.DirectX.IDirect3D9__CreateDevice);

	IDXGISwapChain__Present = reinterpret_cast<IDXGISwapChain__Present_t>
		(sOffsets.DirectX.IDXGISwapChain__Present);

	ID3D11DeviceContext__ClearRenderTargetView = reinterpret_cast<ID3D11DeviceContext__ClearRenderTargetView_t>
		(sOffsets.DirectX.ID3D11DeviceContext__ClearRenderTargetView);

	D3D11CreateDeviceAndSwapChain = reinterpret_cast<D3D11CreateDeviceAndSwapChain_t>
		(sOffsets.DirectX.D3D11CreateDeviceAndSwapChain);
}
