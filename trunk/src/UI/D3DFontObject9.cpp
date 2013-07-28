#include "Misc/stdafx.h"
#include "D3DFontObject9.h"
#include "D3DSprite9.h"

namespace UI {
	D3DFontObject9::D3DFontObject9(IDirect3DDevice9 *pDevice, const FontDescription &fontDesc) {
		Utils::ThrowIfFailed(D3DXCreateFontW(pDevice, fontDesc.height,
			fontDesc.width, fontDesc.weight, fontDesc.mipLevels, fontDesc.italic,
			fontDesc.charSet, fontDesc.outputPrecision, fontDesc.quality,
			fontDesc.pitchAndFamily, fontDesc.faceName.c_str(), &m_d3dxFont));

		RECT spaceRect = {0};
		m_d3dxFont->DrawTextW(nullptr, L" .", 2, &spaceRect,
			DT_LEFT | DT_TOP | DT_SINGLELINE | DT_CALCRECT, 0);

		RECT pointRect = {0};
		m_d3dxFont->DrawTextW(nullptr, L".", 1, &pointRect,
			DT_LEFT | DT_TOP | DT_SINGLELINE | DT_CALCRECT, 0);

		m_pointExtent.x = (pointRect.right - pointRect.left);
		m_pointExtent.y = (pointRect.bottom - pointRect.top);

		m_spaceExtent.x = (spaceRect.right - spaceRect.left) - m_pointExtent.x;
		m_spaceExtent.y = (spaceRect.bottom - spaceRect.top);
	}

	void D3DFontObject9::DrawText(const boost::shared_ptr<const ID3DSprite> &pSprite,
		std::wstring textString, RECT &screenRect, uint32 flags, const D3DXCOLOR &color) const {
		if (m_d3dxFont != nullptr) {
			CComPtr<ID3DXSprite> pD3DXSprite = nullptr;
			const auto pSprite9 = boost::dynamic_pointer_cast<const D3DSprite9>(pSprite);
			if (pSprite9 != nullptr)
				pD3DXSprite = pSprite9->GetD3DXSprite();

			// has trailing whitespace?
			// todo: proper fix for multiline + hcentered
			uint32 length = textString.length();
			if (length != 0 && textString.back() == L' ')
			{
				// this hack only works for single line align
				if ((flags & DT_SINGLELINE) == DT_SINGLELINE) {
					flags &= ~(DT_CENTER | DT_VCENTER | DT_RIGHT | DT_BOTTOM);
					screenRect = GetTextExtent(textString, screenRect, flags);
				}
				else
					textString.back() = 0xA0; // "non breaking space"
			}

			m_d3dxFont->DrawTextW(pD3DXSprite, textString.data(), length, &screenRect, flags, color);
		}
	}

	RECT D3DFontObject9::GetTextExtent(std::wstring textString, RECT rect, uint32 flags) const {
		if (m_d3dxFont != nullptr) {
			uint32 length = textString.length();
			bool trailingSpace = (length != 0 && textString.back() == L' ');
			if (trailingSpace)
				textString.back() = L'.';

			m_d3dxFont->DrawTextW(nullptr, textString.data(), length, &rect, flags | DT_CALCRECT, 0);

			// adjust the rectangle...
			if (trailingSpace) {
				rect.right += m_spaceExtent.x;
				rect.right -= m_pointExtent.x;
			}
		}

		return rect;
	}

	POINT D3DFontObject9::GetGlyphCellInc(wchar_t c) const {
		POINT cellInc = {0};
		if (m_d3dxFont == nullptr)
			return cellInc;

		// fix calculation for space characters
		// we can also return the point extent since we already calculated it
		switch (c) {
		case L' ':
			return m_spaceExtent;

		case L'.':
			return m_pointExtent;
		}

		RECT glyphRect = {0};
		m_d3dxFont->DrawTextW(nullptr, &c, 1, &glyphRect, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_CALCRECT, 0);
		cellInc.x = (glyphRect.right - glyphRect.left);
		cellInc.y = (glyphRect.bottom - glyphRect.top);
		return cellInc;
	}
}
