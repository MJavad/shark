/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

		virtual void DrawText(const boost::shared_ptr<const ID3DSprite> &pSprite,
			std::wstring textString, RECT &screenRect, uint32 flags, const D3DXCOLOR &color) const;

		virtual RECT GetTextExtent(std::wstring textString, RECT rect, uint32 flags) const;

		virtual POINT GetGlyphCellInc(wchar_t c) const;

		CComPtr<ID3DXFont> GetD3DXFont() const { return m_d3dxFont; }

	protected:
		POINT m_spaceExtent, m_pointExtent;
		CComPtr<ID3DXFont> m_d3dxFont;
	};
}
