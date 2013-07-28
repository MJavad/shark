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
#include "ID3DTextureObject.h"

namespace UI {
	class D3DTextureObject9 : public ID3DTextureObject
	{
	public:
		D3DTextureObject9(CComPtr<IDirect3DTexture9> pTexture) : m_d3dTexture9(pTexture) {}
		D3DTextureObject9(IDirect3DDevice9 *pDevice, const TextureDescription &textureDesc);
		CComPtr<IDirect3DTexture9> GetD3DTexture9() const { return m_d3dTexture9; }
		virtual boost::shared_ptr<ID3DSurface> QuerySurface(uint32 uLevel) const;

	protected:
		CComPtr<IDirect3DTexture9> m_d3dTexture9;
	};
}
