#include "Misc/stdafx.h"
#include "Label.h"

namespace UI {
namespace Components {
	Label::Label() : m_color(0xFFE0E0E0), m_formatFlags(0),
		m_shouldCache(true), m_dropShadow(true), m_shadowDirection(2.0f, 2.0f) {
		const auto callback = std::bind(&Label::FlushFontCache, this);
		m_lostDevice = sD3DMgr.OnDeviceLostEvent += callback;
		m_changeDevice = sD3DMgr.OnDeviceChangedEvent += callback;
	}

	Label::~Label() {
		sD3DMgr.OnDeviceLostEvent -= m_lostDevice;
		sD3DMgr.OnDeviceChangedEvent -= m_changeDevice;
	}

	std::shared_ptr<Label> Label::Create(std::wstring swText,
		uint32 dwFormatFlags, float fWidth, float fHeight) {
		const auto pLabel = std::make_shared<Label>();
		pLabel->SetText(std::move(swText));
		pLabel->SetWidth(fWidth);
		pLabel->SetHeight(fHeight);
		pLabel->SetFormatFlags(dwFormatFlags);
		pLabel->SetFont(sD3DMgr.GetFont(L"Segoe UI", 15));
		return pLabel;
	}

	void Label::OnRender(uint32 uTimePassed) {
		if (!GetUseCache()) {
			const auto pFont = GetFont();
			const auto pSprite = sD3DMgr.GetSprite();

			if (pFont != nullptr && pFont->GetObject() != nullptr) {
				RECT screenRect = GetFullRect();
				std::wstring swText = GetText();
				uint32 dwFormatFlags = GetFormatFlags();
				const auto pObject = pFont->GetObject();

				if (pSprite != nullptr)
					pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);

				if (GetDropShadow()) {
					RECT shadowRect = screenRect;
					Utils::Vector2 vShadow = GetShadowDirection();
					OffsetRect(&shadowRect, (int) vShadow.x, (int) vShadow.y);
					pObject->DrawText(pSprite, swText, shadowRect, dwFormatFlags, GetModifiedColor(0x70000000));
				}

				pObject->DrawText(pSprite, swText, screenRect,
					dwFormatFlags, GetModifiedColor(GetColor()));

				if (pSprite != nullptr)
					pSprite->End();
			}

			return;
		}

		if (!IsCached())
			CreateCachedFontBatch();

		const auto pSprite = sD3DMgr.GetSprite();
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

		const auto swText = GetText();
		const auto pFontObject = pFont->GetObject();

		RECT textExtent = {0};
		textExtent.right = static_cast<uint32>(GetWidth());
		textExtent.bottom = static_cast<uint32>(GetHeight());
		textExtent = pFontObject->GetTextExtent(swText, textExtent, GetFormatFlags());

		m_textIndent.x = static_cast<float>(textExtent.left);
		m_textIndent.y = static_cast<float>(textExtent.top);

		uint32 uWidth = textExtent.right - textExtent.left;
		uint32 uHeight = textExtent.bottom - textExtent.top;

		const auto pRenderTarget = sD3DMgr.GetRenderTarget();
		const auto pOldSurface = pRenderTarget->GetRenderTargetSurface();

		m_fontCache = pRenderTarget->CreateRenderTargetTexture(uWidth, uHeight);
		if (m_fontCache == nullptr)
			throw std::runtime_error("Could not create font cache!");

		const auto pSurface = m_fontCache->GetObject()->QuerySurface(0);
		pRenderTarget->SetRenderTargetSurface(pSurface, 0, true);

		RECT fontRect = {0};
		fontRect.right = uWidth;
		fontRect.bottom = uHeight;

		const auto pSprite = sD3DMgr.GetSprite();
		if (pSprite != nullptr)
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);

		pFontObject->DrawText(pSprite, swText, fontRect, DT_NOCLIP, 0xFFFFFFFF);

		if (pSprite != nullptr)
			pSprite->End();

		pRenderTarget->SetRenderTargetSurface(pOldSurface);
		GetInterface()->ClipStack.apply();
	}

	void Label::RenderCachedFontBatch(const std::shared_ptr<const ID3DSprite> &pSprite) const {
		if (m_fontCache != nullptr) {
			Utils::Vector2 vScreen = GetScreenPosition() + m_textIndent;
			if (GetDropShadow()) {
				Utils::Vector3 vShadow = vScreen + GetShadowDirection();
				pSprite->Draw(m_fontCache, nullptr, nullptr, &vShadow, GetModifiedColor(0x70000000));
			}

			Utils::Vector3 vPosition = vScreen;
			pSprite->Draw(m_fontCache, nullptr, nullptr, &vPosition, GetModifiedColor(GetColor()));
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

			RECT textExtent = pObject->GetTextExtent(text, GetFullRect(), flags);
			int32 distFromStart = textExtent.left;

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
		int32 currentPos = 0;
		std::wstring text = GetText();
		uint32 textLength = text.length();
		uint32 flags = GetFormatFlags();
		const auto pFont = GetFont();

		if (pFont != nullptr && pFont->GetObject() != nullptr) {
			const auto pObject = pFont->GetObject();

			RECT textExtent = pObject->GetTextExtent(text, GetFullRect(), flags);
			currentPos = textExtent.left;

			for (uint32 i = 0; i < textLength && i < cp; ++i) {
				POINT cellInc = pObject->GetGlyphCellInc(text[i]);
				currentPos += cellInc.x;
			}
		}

		return currentPos;
	}
}
}
