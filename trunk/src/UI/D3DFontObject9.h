#pragma once
#include "ID3DFontObject.h"

namespace UI {
	class D3DFontObject9 : public ID3DFontObject
	{
	public:
		D3DFontObject9(IDirect3DDevice9 *pDevice, const SFontDesc &fontDesc);

		virtual void OnLostDevice() const {
			if (m_d3dxFont != nullptr)
				m_d3dxFont->OnLostDevice();
		}

		virtual void OnResetDevice() const {
			if (m_d3dxFont != nullptr)
				m_d3dxFont->OnResetDevice();
		}

		virtual void DrawText(const std::shared_ptr<const ID3DSprite> &pSprite, std::wstring swText,
			RECT &screenRect, uint32 dwFlags, const Utils::Color &color) const;

		virtual RECT GetTextExtent(std::wstring swText, RECT rect, uint32 dwFlags) const {
			DrawText(nullptr, std::move(swText), rect, dwFlags | DT_CALCRECT, 0);
			return rect;
		}

		virtual RECT CalcTextRect(std::wstring swText, uint32 begin, uint32 end, RECT rect) const;
		virtual POINT GetGlyphCellInc(wchar_t c) const;

		CComPtr<ID3DXFont> GetD3DXFont() const { return m_d3dxFont; }

	protected:
		CComPtr<ID3DXFont> m_d3dxFont;
	};
}
