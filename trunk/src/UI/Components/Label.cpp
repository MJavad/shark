#include "Misc/stdafx.h"
#include "Label.h"

namespace UI {
namespace Components {
	Label::Label() : m_color(0xFFE0E0E0), m_formatFlags(0),
		m_shouldCache(true), m_dropShadow(true), m_shadowDirection(2.0f, 2.0f) {
		const auto callback = std::bind(&Label::FlushFontCache, this);
		m_lostDevice = sD3DMgr.OnDeviceLostEvent.connect(callback);
		m_changeDevice = sD3DMgr.OnDeviceChangedEvent.connect(callback);
	}

	Label::~Label() {
		sD3DMgr.OnDeviceLostEvent -= m_lostDevice;
		sD3DMgr.OnDeviceChangedEvent -= m_changeDevice;
	}

	std::shared_ptr<Label> Label::Create(std::wstring textString,
										 uint32 dwFormatFlags,
										 float width,
										 float height)
	{
		const auto pLabel = std::make_shared<Label>();
		pLabel->SetText(std::move(textString));
		pLabel->SetWidth(width);
		pLabel->SetHeight(height);
		pLabel->SetFormatFlags(dwFormatFlags);
		pLabel->SetFont(sD3DMgr.GetFont(L"Segoe UI", 15));
		return pLabel;
	}

	void Label::OnRender(uint32 timePassed) {
		const auto pSprite = sD3DMgr.GetSprite();

		if (!GetUseCache()) {
			const auto pFont = GetFont();

			if (pFont != nullptr && pFont->GetObject() != nullptr) {
				RECT screenRect = GetFullRect();
				std::wstring textString = GetText();
				uint32 dwFormatFlags = GetFormatFlags();
				const auto pObject = pFont->GetObject();

				if (pSprite != nullptr)
					pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);

				if (GetDropShadow()) {
					RECT shadowRect = screenRect;
					Utils::Vector2 vShadow = GetShadowDirection();
					OffsetRect(&shadowRect, static_cast<int32>(vShadow.x), static_cast<int32>(vShadow.y));

					pObject->DrawText(pSprite, textString, shadowRect, dwFormatFlags,
						CalculateAbsoluteColor(0x70000000));
				}

				pObject->DrawText(pSprite, textString, screenRect, dwFormatFlags,
					CalculateAbsoluteColor(GetColor()));

				if (pSprite != nullptr)
					pSprite->End();
			}

			return;
		}

		if (!IsCached())
			CreateCachedFontBatch();

		if (pSprite != nullptr) {
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
			RenderCachedFontBatch(pSprite);
			pSprite->End();
		}
	}

	void Label::CreateCachedFontBatch() {
		const auto pFont = GetFont();
		if (pFont == nullptr || pFont->GetObject() == nullptr)
			return;

		const auto textString = GetText();
		const auto pFontObject = pFont->GetObject();

		RECT textExtent = {0};
		textExtent.right = static_cast<LONG>(GetWidth());
		textExtent.bottom = static_cast<LONG>(GetHeight());
		textExtent = pFontObject->GetTextExtent(textString, textExtent, GetFormatFlags());

		m_textOffset.x = static_cast<float>(textExtent.left);
		m_textOffset.y = static_cast<float>(textExtent.top);

		LONG width = textExtent.right - textExtent.left;
		LONG height = textExtent.bottom - textExtent.top;

		const auto pRenderTarget = sD3DMgr.GetRenderTarget();
		const auto pOldSurface = pRenderTarget->GetRenderTargetSurface();

		m_fontCache = pRenderTarget->CreateRenderTargetTexture(width, height);
		if (m_fontCache == nullptr)
			throw std::runtime_error("Could not create font cache!");

		const auto pSurface = m_fontCache->GetObject()->QuerySurface(0);
		pRenderTarget->SetRenderTargetSurface(pSurface, 0, true);

		RECT fontRect = {0};
		fontRect.right = width;
		fontRect.bottom = height;

		const auto pSprite = sD3DMgr.GetSprite();
		if (pSprite != nullptr)
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);

		pFontObject->DrawText(pSprite, textString, fontRect, DT_NOCLIP, 0xFFFFFFFF);

		if (pSprite != nullptr)
			pSprite->End();

		pRenderTarget->SetRenderTargetSurface(pOldSurface);
		GetInterface()->ClipStack.Apply();
	}

	void Label::RenderCachedFontBatch(const std::shared_ptr<const ID3DSprite> &pSprite) const {
		if (m_fontCache != nullptr) {
			Utils::Vector2 textPosition = (GetScreenPosition() + m_textOffset).floor();
			if (GetDropShadow()) {
				Utils::Vector3 vShadow = textPosition + GetShadowDirection();
				pSprite->Draw(m_fontCache, nullptr, nullptr, &vShadow,
					CalculateAbsoluteColor(0x70000000));
			}

			Utils::Vector3 position3 = textPosition;
			pSprite->Draw(m_fontCache, nullptr, nullptr, &position3,
				CalculateAbsoluteColor(GetColor()));
		}
	}

	uint32 Label::XToCP(int32 x) const {
		uint32 currentPos = 0;
		std::wstring text = GetText();
		uint32 textLength = text.length();
		uint32 flags = GetFormatFlags();
		const auto pFont = GetFont();

		if (pFont != nullptr && pFont->GetObject() != nullptr) {
			const auto pObject = pFont->GetObject();
			int32 distFromStart = static_cast<int32>(GetScreenPosition().x + m_textOffset.x);

			if (!IsCached())
				distFromStart = pObject->GetTextExtent(text, GetFullRect(), flags).left;

			for (; currentPos < textLength; ++currentPos) {
				POINT cellInc = pObject->GetGlyphCellInc(text[currentPos]);

				if (distFromStart + cellInc.x / 2 < x)
					distFromStart += cellInc.x;
				else
					break;
			}
		}

		return currentPos;
	}

	int32 Label::CPToX(uint32 cp) const {
		std::wstring text = GetText();
		uint32 textLength = text.length();
		uint32 flags = GetFormatFlags();
		const auto pFont = GetFont();
		int32 currentPos = static_cast<int32>(GetScreenPosition().x + m_textOffset.x);

		if (pFont != nullptr && pFont->GetObject() != nullptr) {
			const auto pObject = pFont->GetObject();

			if (!IsCached())
				currentPos = pObject->GetTextExtent(text, GetFullRect(), flags).left;

			for (uint32 i = 0; i < textLength && i < cp; ++i) {
				POINT cellInc = pObject->GetGlyphCellInc(text[i]);
				currentPos += cellInc.x;
			}
		}

		return currentPos;
	}
}
}
