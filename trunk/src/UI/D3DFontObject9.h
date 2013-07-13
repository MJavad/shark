#pragma once
#include "ID3DFontObject.h"

namespace UI {
	class D3DFontObject9 : public ID3DFontObject
	{
	public:
		D3DFontObject9(IDirect3DDevice9 *pDevice, const FontDescription &fontDesc);

		virtual void OnLostDevice() const {
			if (m_d3dxFont != nullptr)
				m_d3dxFont->OnLostDevice();
		}

		virtual void OnResetDevice() const {
			if (m_d3dxFont != nullptr)
				m_d3dxFont->OnResetDevice();
		}

		virtual void DrawText(const std::shared_ptr<const ID3DSprite> &pSprite, std::wstring textString,
			RECT &screenRect, uint32 flags, const D3DXCOLOR &color) const;

		virtual RECT GetTextExtent(std::wstring textString, RECT rect, uint32 flags) const {
			DrawText(nullptr, std::move(textString), rect, flags | DT_CALCRECT, D3DXCOLOR());
			return rect;
		}

		virtual POINT GetGlyphCellInc(wchar_t c) const;

		CComPtr<ID3DXFont> GetD3DXFont() const { return m_d3dxFont; }

	protected:
		CComPtr<ID3DXFont> m_d3dxFont;
	};
}
