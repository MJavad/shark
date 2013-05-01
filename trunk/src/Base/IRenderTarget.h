#pragma once
#include "UI/D3DTexture.h"
#include "UI/ID3DSurface.h"

class IRenderTarget abstract
{
public:
	virtual ~IRenderTarget() {}
	virtual void BeginUI() {}
	virtual void EndUI() {}
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}
	virtual bool GetSurfaceRect(RECT *pRect) const = 0;
	virtual bool GetClippingArea(RECT *pRect) const = 0;
	virtual void SetClippingArea(const RECT *pRect) const = 0;

	static float GetDimensionWidth(const std::array<Utils::Vector2, 4> &dimensions) {
		return __max(dimensions[1].x, dimensions[2].x) -
			   __min(dimensions[0].x, dimensions[3].x);
	}

	static float GetDimensionHeight(const std::array<Utils::Vector2, 4> &dimensions) {
		return __max(dimensions[2].y, dimensions[3].y) -
			   __min(dimensions[0].y, dimensions[1].y);
	}

	static std::array<Utils::Vector2, 4> MakeDimension(float fWidth, float fHeight) {
		std::array<Utils::Vector2, 4> dimension;
		dimension[0].x = 0;      dimension[0].y = 0;
		dimension[1].x = fWidth; dimension[1].y = 0;
		dimension[2].x = fWidth; dimension[2].y = fHeight;
		dimension[3].x = 0;      dimension[3].y = fHeight;
		return dimension;
	}
	
	virtual std::shared_ptr<UI::D3DTexture> CreateRenderTargetTexture(uint32 uWidth, uint32 uHeight) const = 0;
	virtual std::shared_ptr<UI::ID3DSurface> CreateRenderTargetSurface(uint32 uWidth, uint32 uHeight) const = 0;
	virtual void SetRenderTargetSurface(const std::shared_ptr<const UI::ID3DSurface> &pSurface, uint32 uIndex = 0, bool bClear = false) = 0;
	virtual std::shared_ptr<UI::ID3DSurface> GetRenderTargetSurface(uint32 uIndex = 0) const = 0;

	// Normal
	virtual void DrawRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor, float fStroke) const = 0;
	virtual void FillRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor) const = 0;

	virtual void DrawRoundedRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius, const float4 &fVertRadius, uint32 dwColor, float fStroke) const = 0;

	virtual void FillRoundedRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius, const float4 &fVertRadius, uint32 dwColor) const = 0;

	virtual void DrawBlurredSprite(const Utils::Vector2 &vPosition, std::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor) = 0;

	virtual void DrawSprite(const Utils::Vector2 &vPosition, std::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor) = 0;

	// Gradient
	virtual void FillRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions, const std::array<Utils::Color, 4> &gradient) const = 0;
	virtual void FillRoundedRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius, const float4 &fVertRadius, const std::array<Utils::Color, 4> &gradient) const = 0;
};
