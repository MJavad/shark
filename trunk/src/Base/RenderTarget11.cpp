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

std::shared_ptr<UI::D3DTexture> RenderTarget11::CreateRenderTargetTexture(uint32 width, uint32 height) const {
	// unimplemented...
	return nullptr;
}

std::shared_ptr<UI::ID3DSurface> RenderTarget11::CreateRenderTargetSurface(uint32 width, uint32 height) const {
	// unimplemented...
	return nullptr;
}

void RenderTarget11::SetRenderTargetSurface(const std::shared_ptr<const UI::ID3DSurface> &pSurface,
											uint32 index, bool shouldClear)
{
	// unimplemented...
}

std::shared_ptr<UI::ID3DSurface> RenderTarget11::GetRenderTargetSurface(uint32 index) const {
	// unimplemented...
	return nullptr;
}

void RenderTarget11::DrawRectangle(const Utils::Vector2 &position,
								   const std::array<Utils::Vector2, 4> &dimensions,
								   const std::array<D3DXCOLOR, 4> &gradient,
								   float fStroke) const
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
										  float fStroke) const
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
									   std::shared_ptr<const UI::D3DTexture> pTexture,
									   const std::array<Utils::Vector2, 4> &dimensions,
									   const std::array<D3DXCOLOR, 4> &gradient) const
{
	// unimplemented...
}

void RenderTarget11::DrawSprite(const Utils::Vector2 &position,
								std::shared_ptr<const UI::D3DTexture> pTexture,
								const std::array<Utils::Vector2, 4> &dimensions,
								const std::array<D3DXCOLOR, 4> &gradient) const
{
	// unimplemented...
}
