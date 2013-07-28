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

class Offsets {
	SINGLETON_CLASS(Offsets) {}

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
		DWORD_PTR IDirect3D9Ex__CreateDeviceEx;
		DWORD_PTR IDirect3DDevice9__EndScene;
		DWORD_PTR IDirect3DDevice9__Reset;
		DWORD_PTR IDirect3DDevice9Ex__ResetEx;

		DWORD_PTR D3D11CreateDeviceAndSwapChain;
		DWORD_PTR IDXGISwapChain__Present;
		DWORD_PTR ID3D11DeviceContext__ClearRenderTargetView;
	};
	SDirectX DirectX;
};

#define sOffsets ::Offsets::Instance()
