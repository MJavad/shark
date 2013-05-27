#include "Misc/stdafx.h"
#include "Rectangle.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	Rectangle::Rectangle() : m_dropShadow(false), m_shadowDirection(7.0f, 7.0f) {
		memset(&m_horizRoundings, 0, sizeof(m_horizRoundings));
		memset(&m_vertRoundings, 0, sizeof(m_vertRoundings));

		const auto callback = [this] { m_shadowTexture = nullptr; };
		m_lostDevice = sD3DMgr.OnDeviceLostEvent += callback;
		m_changeDevice = sD3DMgr.OnDeviceChangedEvent += callback;
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
		m_shadowTexture = nullptr;
		m_dimensions = std::move(dimensions);
		IRectComponent::SetWidth(IRenderTarget::GetDimensionWidth(m_dimensions));
		IRectComponent::SetHeight(IRenderTarget::GetDimensionHeight(m_dimensions));
	}

	void Rectangle::OnRender(uint32 uTimePassed) {
		const auto dimensions = GetDimensions();
		Utils::Vector2 vPosition = GetScreenPosition();
		float4 fHorizontalRounding = GetHorizontalRoundings();
		float4 fVerticalRounding = GetVerticalRoundings();
		const auto gradient = GetModifiedColor(GetGradientColors());
		const auto pRenderTarget = sD3DMgr.GetRenderTarget();

		bool bDrawRoundings = ((fHorizontalRounding._1 != 0.0f && fVerticalRounding._1 != 0.0f) ||
							   (fHorizontalRounding._2 != 0.0f && fVerticalRounding._2 != 0.0f) ||
							   (fHorizontalRounding._3 != 0.0f && fVerticalRounding._3 != 0.0f) ||
							   (fHorizontalRounding._4 != 0.0f && fVerticalRounding._4 != 0.0f));

		if (GetDropShadow()) {
			const RECT *pOldClipRect = GetInterface()->ClipStack.top();
			const auto pOldSurface = pRenderTarget->GetRenderTargetSurface();

			if (m_shadowTexture == nullptr) {
				m_shadowTexture = pRenderTarget->CreateRenderTargetTexture((uint32) GetWidth(), (uint32) GetHeight());

				if (m_shadowTexture == nullptr) {
					LOG_DEBUG("Could not create shadow texture!");
					return;
				}
			}

			const auto pSurface = m_shadowTexture->GetObject()->QuerySurface(0);
			pRenderTarget->SetRenderTargetSurface(pSurface, 0, true);

			if (bDrawRoundings)
				pRenderTarget->FillRoundedRectangle(Utils::Vector2(0.0f, 0.0f), dimensions,
					fHorizontalRounding, fVerticalRounding, gradient);
			else
				pRenderTarget->FillRectangle(Utils::Vector2(0.0f, 0.0f), dimensions, gradient);
			
			pRenderTarget->SetRenderTargetSurface(pOldSurface);

			if (pOldClipRect != nullptr) {
				RECT newClipRect = *pOldClipRect;
				if (m_shadowDirection.x < 0.0f)
					newClipRect.left += (long) m_shadowDirection.x;
				else
					newClipRect.right += (long) m_shadowDirection.x;

				if (m_shadowDirection.y < 0.0f)
					newClipRect.top += (long) m_shadowDirection.y;
				else
					newClipRect.bottom += (long) m_shadowDirection.y;

				pRenderTarget->SetClippingArea(&newClipRect);
			}

			pRenderTarget->DrawBlurredSprite(vPosition + m_shadowDirection, m_shadowTexture, dimensions, 0xAA000000);

			if (pOldClipRect != nullptr)
				pRenderTarget->SetClippingArea(pOldClipRect);

			const auto pSprite = sD3DMgr.GetSprite();
			if (pSprite != nullptr) {
				pSprite->Begin(D3DXSPRITE_ALPHABLEND);
				Utils::Vector3 vPosition3 = vPosition;
				pSprite->Draw(m_shadowTexture, nullptr, nullptr, &vPosition3, 0xFFFFFFFF);
				pSprite->End();
			}
		}
		else {
			if (bDrawRoundings)
				pRenderTarget->FillRoundedRectangle(vPosition, dimensions, fHorizontalRounding, fVerticalRounding, gradient);
			else
				pRenderTarget->FillRectangle(vPosition, dimensions, gradient);
		}
	}
}
}
