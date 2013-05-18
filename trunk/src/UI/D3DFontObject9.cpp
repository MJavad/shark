#include "Misc/stdafx.h"
#include "D3DFontObject9.h"
#include "D3DSprite9.h"

namespace UI {
	D3DFontObject9::D3DFontObject9(IDirect3DDevice9 *pDevice, const SFontDesc &fontDesc) {
		HRESULT hResult = D3DXCreateFontW(pDevice, fontDesc.Height,
			fontDesc.Width, fontDesc.Weight, fontDesc.MipLevels, fontDesc.Italic,
			fontDesc.CharSet, fontDesc.OutputPrecision, fontDesc.Quality,
			fontDesc.PitchAndFamily, fontDesc.FaceName.c_str(), &m_d3dxFont);
		assert(hResult == D3D_OK);
	}

	void D3DFontObject9::DrawText(const std::shared_ptr<const ID3DSprite> &pSprite, std::wstring swText,
								  RECT &screenRect, uint32 dwFlags, const Utils::Color &color) const {
		if (m_d3dxFont != nullptr) {
			CComPtr<ID3DXSprite> pD3DXSprite = nullptr;
			const auto pSprite9 = std::dynamic_pointer_cast<const D3DSprite9>(pSprite);
			if (pSprite9 != nullptr)
				pD3DXSprite = pSprite9->GetD3DXSprite();

			// Replace whitespaces with a non-breaking space to fix calculation issues.
			uint32 length = swText.length();
			if (length != 0 && swText.back() == 0x20)
				swText.back() = 0xA0;

			m_d3dxFont->DrawTextW(pD3DXSprite, swText.data(), length, &screenRect, dwFlags, color);
		}
	}

	POINT D3DFontObject9::GetGlyphCellInc(wchar_t c) const {
		POINT cellInc = {0};
		if (m_d3dxFont == nullptr)
			return cellInc;

		// replace space with non-breaking space...
		if (c == 0x20)
			c = 0xA0;

		RECT glyphRect = {0};
		m_d3dxFont->DrawTextW(nullptr, &c, 1, &glyphRect, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0);
		cellInc.x = glyphRect.right - glyphRect.left;
		cellInc.y = glyphRect.bottom - glyphRect.top;
		return cellInc;
	}
	
	RECT D3DFontObject9::CalcTextRect(std::wstring swText, uint32 begin, uint32 end, RECT rect) const {
		if (m_d3dxFont == nullptr)
			return rect;
		
		uint32 length = swText.length();
		if (begin >= length || end >= length || begin == end)
			return rect;

		// Replace whitespaces with a non-breaking space to fix calculation issues.
		std::transform(swText.begin(), swText.end(), swText.begin(), [] (wchar_t c) {
			return c != 0x20 ? c : 0xA0;
		});

		m_d3dxFont->DrawTextW(nullptr, swText.data(), max(begin, end),
			&rect, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0);

		RECT tempRect = rect;
		m_d3dxFont->DrawTextW(nullptr, swText.data(), min(begin, end),
			&tempRect, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0);

		rect.left = tempRect.right;
		return rect;
	}
}
