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
#include "IRenderTarget.h"
#include "UI/D3DSurface9.h"

template <typename T>
struct SStateBlockRecord {
	T ordinal;
	DWORD value;
};

class RenderTarget9 : public IRenderTarget
{
public:
	RenderTarget9(IDirect3DDevice9 *pDevice);
	virtual void BeginUI();
	virtual void EndUI();
	virtual void OnLostDevice();
	virtual void OnResetDevice();
	virtual bool GetSurfaceRect(RECT *pRect) const;
	virtual bool GetClippingArea(RECT *pRect) const;
	virtual void SetClippingArea(const RECT *pRect) const;

	virtual boost::shared_ptr<UI::D3DTexture> CreateRenderTargetTexture(uint32 width, uint32 height) const;
	virtual boost::shared_ptr<UI::ID3DSurface> CreateRenderTargetSurface(uint32 width, uint32 height) const;
	virtual void SetRenderTargetSurface(const boost::shared_ptr<const UI::ID3DSurface> &pSurface, uint32 index = 0, bool shouldClear = false);
	virtual boost::shared_ptr<UI::ID3DSurface> GetRenderTargetSurface(uint32 index = 0) const;

	virtual void DrawRectangle(const Utils::Vector2 &position,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient,
		float stroke) const;

	virtual void FillRectangle(const Utils::Vector2 &position,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawRoundedRectangle(const Utils::Vector2 &position,
		const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &horizontalRadius,
		const float4 &verticalRadius,
		const std::array<D3DXCOLOR, 4> &gradient,
		float stroke) const;

	virtual void FillRoundedRectangle(const Utils::Vector2 &position,
		const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &horizontalRadius,
		const float4 &verticalRadius,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawBlurredSprite(const Utils::Vector2 &position,
		boost::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawSprite(const Utils::Vector2 &position,
		boost::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

protected:
	IDirect3DDevice9 *m_device9;
	CComPtr<IDirect3DStateBlock9> m_stateBlock;
	CComPtr<IDirect3DStateBlock9> m_defaultStateBlock;
	CComPtr<IDirect3DStateBlock9> _createStateBlock(D3DSTATEBLOCKTYPE type) const;
	CComPtr<IDirect3DStateBlock9> _createDefaultStateBlock() const;

	// Stateblock default data
	static const SStateBlockRecord<D3DRENDERSTATETYPE> m_renderStateTable[];
	static const SStateBlockRecord<D3DSAMPLERSTATETYPE> m_samplerStateTable[];
	static const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> m_textureStage1StateTable[];
	static const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> m_textureStageNStateTable[];

	// UI Shader
	CComPtr<IDirect3DVertexShader9> m_uiVertexShader;
	CComPtr<IDirect3DVertexDeclaration9> m_uiVertexShaderDecl;

	// Rounded Rectangle Shaders
	CComPtr<IDirect3DPixelShader9> m_drawRoundedRectangleShader;
	CComPtr<IDirect3DPixelShader9> m_fillRoundedRectangleShader;

	// Blur Shaders
	CComPtr<IDirect3DPixelShader9> m_blurShader;
};
