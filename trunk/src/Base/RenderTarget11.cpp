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
#include "RenderTarget11.h"

RenderTarget11::RenderTarget11(IDXGISwapChain *pSwapChain, ID3D11Device *pDevice, ID3D11DeviceContext *pContext) :
	m_swapChain(pSwapChain), m_device11(pDevice), m_context11(pContext) {
}

bool RenderTarget11::GetSurfaceRect(RECT *pRect) const {
	if (pRect != nullptr) {
		ZeroMemory(pRect, sizeof(RECT));
		DXGI_SWAP_CHAIN_DESC swapDesc = {0};
		HRESULT result = m_swapChain->GetDesc(&swapDesc);

		if (result == S_OK) {
			pRect->right = swapDesc.BufferDesc.Width;
			pRect->bottom = swapDesc.BufferDesc.Height;
			return true;
		}
	}

	return false;
}

bool RenderTarget11::GetClippingArea(RECT *pRect) const {
	// unimplemented...
	return false;
}

void RenderTarget11::SetClippingArea(const RECT *pRect) const {
	// unimplemented...
}

boost::shared_ptr<UI::D3DTexture> RenderTarget11::CreateRenderTargetTexture(uint32 width, uint32 height) const {
	// unimplemented...
	return nullptr;
}

boost::shared_ptr<UI::ID3DSurface> RenderTarget11::CreateRenderTargetSurface(uint32 width, uint32 height) const {
	// unimplemented...
	return nullptr;
}

void RenderTarget11::SetRenderTargetSurface(const boost::shared_ptr<const UI::ID3DSurface> &pSurface,
											uint32 index, bool shouldClear)
{
	// unimplemented...
}

boost::shared_ptr<UI::ID3DSurface> RenderTarget11::GetRenderTargetSurface(uint32 index) const {
	// unimplemented...
	return nullptr;
}

void RenderTarget11::DrawRectangle(const Utils::Vector2 &position,
								   const std::array<Utils::Vector2, 4> &dimensions,
								   const std::array<D3DXCOLOR, 4> &gradient,
								   float stroke) const
{
	// unimplemented...
}

void RenderTarget11::FillRectangle(const Utils::Vector2 &position,
								   const std::array<Utils::Vector2, 4> &dimensions,
								   const std::array<D3DXCOLOR, 4> &gradient) const
{
	// unimplemented...
}

void RenderTarget11::DrawRoundedRectangle(const Utils::Vector2 &position,
										  const std::array<Utils::Vector2, 4> &dimensions,
										  const float4 &horizontalRadius,
										  const float4 &verticalRadius,
										  const std::array<D3DXCOLOR, 4> &gradient,
										  float stroke) const
{
	// unimplemented...
}

void RenderTarget11::FillRoundedRectangle(const Utils::Vector2 &position,
										  const std::array<Utils::Vector2, 4> &dimensions,
										  const float4 &horizontalRadius,
										  const float4 &verticalRadius,
										  const std::array<D3DXCOLOR, 4> &gradient) const
{
	// unimplemented...
}

void RenderTarget11::DrawBlurredSprite(const Utils::Vector2 &position,
									   boost::shared_ptr<const UI::D3DTexture> pTexture,
									   const std::array<Utils::Vector2, 4> &dimensions,
									   const std::array<D3DXCOLOR, 4> &gradient) const
{
	// unimplemented...
}

void RenderTarget11::DrawSprite(const Utils::Vector2 &position,
								boost::shared_ptr<const UI::D3DTexture> pTexture,
								const std::array<Utils::Vector2, 4> &dimensions,
								const std::array<D3DXCOLOR, 4> &gradient) const
{
	// unimplemented...
}
