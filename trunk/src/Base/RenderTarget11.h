#pragma once
#include "IRenderTarget.h"

class RenderTarget11 : public IRenderTarget
{
public:
	RenderTarget11(IDXGISwapChain *pSwapChain, ID3D11Device *pDevice, ID3D11DeviceContext *pContext);

	virtual bool GetSurfaceRect(RECT *pRect) const;
	virtual bool GetClippingArea(RECT *pRect) const;
	virtual void SetClippingArea(const RECT *pRect) const;

	virtual std::shared_ptr<UI::D3DTexture> CreateRenderTargetTexture(uint32 uWidth, uint32 uHeight) const;
	virtual std::shared_ptr<UI::ID3DSurface> CreateRenderTargetSurface(uint32 uWidth, uint32 uHeight) const;
	virtual void SetRenderTargetSurface(const std::shared_ptr<const UI::ID3DSurface> &pSurface, uint32 uIndex = 0, bool bClear = false);
	virtual std::shared_ptr<UI::ID3DSurface> GetRenderTargetSurface(uint32 uIndex = 0) const;

	virtual void DrawRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient,
		float fStroke) const;

	virtual void FillRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawRoundedRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius,
		const float4 &fVertRadius,
		const std::array<D3DXCOLOR, 4> &gradient,
		float fStroke) const;

	virtual void FillRoundedRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius,
		const float4 &fVertRadius,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawBlurredSprite(const Utils::Vector2 &vPosition,
		std::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawSprite(const Utils::Vector2 &vPosition,
		std::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

protected:
	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device11;
	ID3D11DeviceContext *m_context11;
};
