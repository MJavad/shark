#include "Misc/stdafx.h"
#include "D3DFontObject9.h"
#include "D3DSprite9.h"

namespace UI {
	D3DFontObject9::D3DFontObject9(IDirect3DDevice9 *pDevice, const FontDescription &fontDesc) {
		HRESULT result = D3DXCreateFontW(pDevice, fontDesc.height,
			fontDesc.width, fontDesc.weight, fontDesc.mipLevels, fontDesc.italic,
			fontDesc.charSet, fontDesc.outputPrecision, fontDesc.quality,
			fontDesc.pitchAndFamily, fontDesc.faceName.c_str(), &m_d3dxFont);

		assert(result == D3D_OK);
	}

	void D3DFontObject9::DrawText(const std::shared_ptr<const ID3DSprite> &pSprite, std::wstring textString,
								  RECT &screenRect, uint32 flags, const D3DXCOLOR &color) const {
		if (m_d3dxFont != nullptr) {
			CComPtr<ID3DXSprite> pD3DXSprite = nullptr;
			const auto pSprite9 = std::dynamic_pointer_cast<const D3DSprite9>(pSprite);
			if (pSprite9 != nullptr)
				pD3DXSprite = pSprite9->GetD3DXSprite();

			// Replace whitespaces with a non-breaking space to fix calculation issues.
			uint32 length = textString.length();
			if (length != 0 && textString.back() == 0x20)
				textString.back() = 0xA0;

			m_d3dxFont->DrawTextW(pD3DXSprite, textString.data(), length, &screenRect, flags, color);
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
}
