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
#include "ID3DSurface.h"

namespace UI {
	class D3DSurface9 : public ID3DSurface
	{
	public:
		D3DSurface9(CComPtr<IDirect3DSurface9> pSurface) : m_surface9(pSurface) {}

		CComPtr<IDirect3DSurface9> GetSurface9() const {
			return m_surface9;
		}

		virtual HDC GetDC() const {
			HDC hDC = nullptr;
			m_surface9->GetDC(&hDC);
			return hDC;
		}

	protected:
		CComPtr<IDirect3DSurface9> m_surface9;
	};
}
