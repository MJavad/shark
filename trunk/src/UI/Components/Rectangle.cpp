#include "Misc/stdafx.h"
#include "Rectangle.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	Rectangle::Rectangle() : m_dropShadow(false), m_shadowDirection(7.0f, 7.0f) {
		memset(&m_horizRoundings, 0, sizeof(m_horizRoundings));
		memset(&m_vertRoundings, 0, sizeof(m_vertRoundings));

		const auto callback = std::bind(&Rectangle::FlushShadowTexture, this);
		m_lostDevice = sD3DMgr.OnDeviceLostEvent.connect(callback);
		m_changeDevice = sD3DMgr.OnDeviceChangedEvent.connect(callback);
	}

	Rectangle::~Rectangle() {
		sD3DMgr.OnDeviceLostEvent -= m_lostDevice;
		sD3DMgr.OnDeviceChangedEvent -= m_changeDevice;
	}

	std::shared_ptr<Rectangle> Rectangle::Create(float fWidth, float fHeight) {
		const auto pRectangle = std::make_shared<Rectangle>();
		pRectangle->SetWidth(fWidth);
		pRectangle->SetHeight(fHeight);
		return pRectangle;
	}

	void Rectangle::SetDimensions(std::array<Utils::Vector2, 4> dimensions) {
		FlushShadowTexture();
		m_dimensions = std::move(dimensions);
		IRectComponent::SetWidth(IRenderTarget::GetDimensionWidth(m_dimensions));
		IRectComponent::SetHeight(IRenderTarget::GetDimensionHeight(m_dimensions));
	}

	void Rectangle::OnRender(uint32 uTimePassed) {
		const auto dimensions = GetDimensions();
		Utils::Vector2 vPosition = GetScreenPosition();
		const auto pRenderTarget = sD3DMgr.GetRenderTarget();

		if (GetDropShadow()) {
			if (m_shadowTexture == nullptr)
				CreateShadowTexture();

			const RECT *pClipRect = GetInterface()->ClipStack.Top();
			if (pClipRect != nullptr) {
				RECT newClipRect = *pClipRect;
				if (m_shadowDirection.x < 0.0f)
					newClipRect.left += static_cast<LONG>(m_shadowDirection.x);
				else
					newClipRect.right += static_cast<LONG>(m_shadowDirection.x);

				if (m_shadowDirection.y < 0.0f)
					newClipRect.top += static_cast<LONG>(m_shadowDirection.y);
				else
					newClipRect.bottom += static_cast<LONG>(m_shadowDirection.y);

				pRenderTarget->SetClippingArea(&newClipRect);
			}

			std::array<Utils::Color, 4> gradient;
			gradient.fill(GetModifiedColor(Utils::Color(0xAA, 0x00, 0x00, 0x00)));

			pRenderTarget->DrawBlurredSprite(
				vPosition + m_shadowDirection,
				m_shadowTexture,
				dimensions,
				gradient);

			if (pClipRect != nullptr)
				pRenderTarget->SetClippingArea(pClipRect);

			const auto pSprite = sD3DMgr.GetSprite();
			if (pSprite != nullptr) {
				Utils::Vector3 vPosition3 = vPosition;
				pSprite->Begin(D3DXSPRITE_ALPHABLEND);
				pSprite->Draw(m_shadowTexture, nullptr, nullptr, &vPosition3,
					GetModifiedColor(Utils::Color(0xFF, 0xFF, 0xFF, 0xFF)));
				pSprite->End();
			}
		}
		else {
			float4 horizRounding = GetHorizontalRoundings();
			float4 vertRounding = GetVerticalRoundings();
			const auto gradient = GetModifiedColor(GetGradientColors());

			if (((horizRounding._1 != 0.0f && vertRounding._1 != 0.0f) ||
				 (horizRounding._2 != 0.0f && vertRounding._2 != 0.0f) ||
				 (horizRounding._3 != 0.0f && vertRounding._3 != 0.0f) ||
				 (horizRounding._4 != 0.0f && vertRounding._4 != 0.0f)))
			{
				pRenderTarget->FillRoundedRectangle(
					vPosition,
					dimensions,
					horizRounding,
					vertRounding,
					gradient);
			}
			else
				pRenderTarget->FillRectangle(vPosition, dimensions, gradient);
		}
	}

	void Rectangle::CreateShadowTexture() {
		const auto pRenderTarget = sD3DMgr.GetRenderTarget();
		const auto pOldSurface = pRenderTarget->GetRenderTargetSurface();

		uint32 uWidth = static_cast<uint32>(GetWidth());
		uint32 uHeight = static_cast<uint32>(GetHeight());
		m_shadowTexture = pRenderTarget->CreateRenderTargetTexture(uWidth, uHeight);

		if (m_shadowTexture == nullptr)
			throw std::runtime_error("Could not create shadow texture!");

		const auto pSurface = m_shadowTexture->GetObject()->QuerySurface(0);
		pRenderTarget->SetRenderTargetSurface(pSurface, 0, true);

		const auto dimensions = GetDimensions();
		const auto gradientColors = GetGradientColors();
		float4 horizRounding = GetHorizontalRoundings();
		float4 vertRounding = GetVerticalRoundings();

		if (((horizRounding._1 != 0.0f && vertRounding._1 != 0.0f) ||
			 (horizRounding._2 != 0.0f && vertRounding._2 != 0.0f) ||
			 (horizRounding._3 != 0.0f && vertRounding._3 != 0.0f) ||
			 (horizRounding._4 != 0.0f && vertRounding._4 != 0.0f)))
		{
			pRenderTarget->FillRoundedRectangle(
				Utils::Vector2(0.0f, 0.0f),
				dimensions,
				horizRounding,
				vertRounding,
				gradientColors);
		}
		else {
			pRenderTarget->FillRectangle(
				Utils::Vector2(0.0f, 0.0f),
				dimensions,
				gradientColors);
		}

		pRenderTarget->SetRenderTargetSurface(pOldSurface);
		GetInterface()->ClipStack.Apply();
	}
}
}
