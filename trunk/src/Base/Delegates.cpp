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

#include "Misc/stdafx.h"
#include "Delegates.h"
#include "Offsets.h"

void Delegates::Initialize() {
	IDirect3DDevice9__EndScene = reinterpret_cast<IDirect3DDevice9__EndScene_t>
		(sOffsets.DirectX.IDirect3DDevice9__EndScene);

	IDirect3DDevice9__Reset = reinterpret_cast<IDirect3DDevice9__Reset_t>
		(sOffsets.DirectX.IDirect3DDevice9__Reset);

	IDirect3DDevice9Ex__ResetEx = reinterpret_cast<IDirect3DDevice9Ex__ResetEx_t>
		(sOffsets.DirectX.IDirect3DDevice9Ex__ResetEx);

	IDirect3D9__CreateDevice = reinterpret_cast<IDirect3D9__CreateDevice_t>
		(sOffsets.DirectX.IDirect3D9__CreateDevice);

	IDirect3D9Ex__CreateDeviceEx = reinterpret_cast<IDirect3D9Ex__CreateDeviceEx_t>
		(sOffsets.DirectX.IDirect3D9Ex__CreateDeviceEx);

	IDXGISwapChain__Present = reinterpret_cast<IDXGISwapChain__Present_t>
		(sOffsets.DirectX.IDXGISwapChain__Present);

	ID3D11DeviceContext__ClearRenderTargetView = reinterpret_cast<ID3D11DeviceContext__ClearRenderTargetView_t>
		(sOffsets.DirectX.ID3D11DeviceContext__ClearRenderTargetView);

	D3D11CreateDeviceAndSwapChain = reinterpret_cast<D3D11CreateDeviceAndSwapChain_t>
		(sOffsets.DirectX.D3D11CreateDeviceAndSwapChain);
}
