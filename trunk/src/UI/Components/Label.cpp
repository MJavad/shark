#include "Misc/stdafx.h"
#include "Label.h"

namespace UI {
namespace Components {
	Label::Label() : m_color(0xFFE0E0E0), m_formatFlags(0),
		m_shouldCache(true), m_dropShadow(true), m_shadowDirection(2.0f, 2.0f) {
		const auto callback = boost::bind(&Label::_flushFontCache, this);
		m_lostDevice = sD3DMgr.OnDeviceLostEvent.connect(callback);
		m_changeDevice = sD3DMgr.OnDeviceChangedEvent.connect(callback);
	}

	Label::~Label() {
		sD3DMgr.OnDeviceLostEvent -= m_lostDevice;
		sD3DMgr.OnDeviceChangedEvent -= m_changeDevice;
	}

	boost::shared_ptr<Label> Label::Create(std::wstring textString,
										   uint32 formatFlags,
										   float width,
										   float height)
	{
		const auto pLabel = boost::make_shared<Label>();
		pLabel->SetText(std::move(textString));
		pLabel->SetWidth(width);
		pLabel->SetHeight(height);
		pLabel->SetFormatFlags(formatFlags);
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
		GetGlobalInterface()->ClipStack.Apply();
	}

	void Label::RenderCachedFontBatch(const boost::shared_ptr<const ID3DSprite> &pSprite) const {
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

	void Label::SetWidth(float width) {
		if (GetWidth() != width) {
			IRectComponent::SetWidth(width);

			if (GetUseCache()) {
				// If we use word breaking, we need to render it all over again :(
				uint32 formatFlags = GetFormatFlags();
				if ((formatFlags & DT_WORDBREAK) == DT_WORDBREAK)
					_flushFontCache();

				// ... otherwise we can just calculate the new text metrics...
				else if (((formatFlags & DT_CENTER) == DT_CENTER) ||
						((formatFlags & DT_RIGHT) == DT_RIGHT))
					_recalcTextMetrics();
			}
		}
	}

	void Label::SetHeight(float height) {
		if (GetHeight() != height) {
			IRectComponent::SetHeight(height);

			// If we use caching, we probably need to recalc text metrics...
			if (GetUseCache()) {
				uint32 formatFlags = GetFormatFlags();
				if (((formatFlags & DT_VCENTER) == DT_VCENTER) ||
					((formatFlags & DT_BOTTOM) == DT_BOTTOM))
					_recalcTextMetrics();
			}
		}
	}

	uint32 Label::XToCP(int32 x) const {
		uint32 currentPos = 0;
		int32 distFromStart = 0;
		std::wstring text = GetText();
		uint32 textLength = text.length();
		uint32 flags = GetFormatFlags();
		const auto pFont = GetFont();

		if (pFont != nullptr && pFont->GetObject() != nullptr) {
			const auto pObject = pFont->GetObject();

			if (IsCached())
				distFromStart = static_cast<int32>(GetScreenPosition().x + m_textOffset.x);
			else
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
		int32 currentPos = 0;
		std::wstring text = GetText();
		uint32 textLength = text.length();
		uint32 flags = GetFormatFlags();
		const auto pFont = GetFont();

		if (pFont != nullptr && pFont->GetObject() != nullptr) {
			const auto pObject = pFont->GetObject();

			if (IsCached())
				currentPos = static_cast<int32>(GetScreenPosition().x + m_textOffset.x);
			else
				currentPos = pObject->GetTextExtent(text, GetFullRect(), flags).left;

			for (uint32 i = 0; i < textLength && i < cp; ++i) {
				POINT cellInc = pObject->GetGlyphCellInc(text[i]);
				currentPos += cellInc.x;
			}
		}

		return currentPos;
	}

	void Label::_recalcTextMetrics() {
		const auto pFont = GetFont();
		if (pFont != nullptr && pFont->GetObject() != nullptr) {
			RECT textExtent = {0};
			textExtent.right = static_cast<LONG>(GetWidth());
			textExtent.bottom = static_cast<LONG>(GetHeight());
			textExtent = pFont->GetObject()->GetTextExtent(GetText(), textExtent, GetFormatFlags());

			m_textOffset.x = static_cast<float>(textExtent.left);
			m_textOffset.y = static_cast<float>(textExtent.top);
		}
	}

	void Label::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<Label, IRectComponent,
							boost::shared_ptr<IComponent>>("Label")
				.scope [ luabind::def("Create", &Label::CreateDefault) ]
				.def("IsCached", &Label::IsCached)
				.def("XToCP", &Label::XToCP)
				.def("CPToX", &Label::CPToX)
				.property("text", &Label::GetText_UTF8, &Label::SetText_UTF8)
				.property("color", &Label::GetColor, &Label::SetColor)
				.property("formatFlags", &Label::GetFormatFlags, &Label::SetFormatFlags)
				.property("font", &Label::GetFont, &Label::SetFont)
				.property("dropShadow", &Label::GetDropShadow, &Label::SetDropShadow)
				.property("shadowDirection", &Label::GetShadowDirection, &Label::SetShadowDirection)
				.property("useCache", &Label::GetUseCache, &Label::SetUseCache)
		];
	}
}
}
