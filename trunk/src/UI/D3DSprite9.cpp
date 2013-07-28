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

#include "Misc/stdafx.h"
#include "D3DSprite9.h"
#include "D3DTextureObject9.h"

namespace UI {
	D3DSprite9::D3DSprite9(IDirect3DDevice9 *pDevice) {
		HRESULT result = D3DXCreateSprite(pDevice, &m_d3dxSprite);
		if (result != D3D_OK)
			throw std::runtime_error("Could not create sprite object!");
	}

	void D3DSprite9::Draw(const boost::shared_ptr<const D3DTexture> &pTexture, const RECT *pScreenRect,
		const Utils::Vector3 *pCenter, const Utils::Vector3 *pPosition, const D3DXCOLOR &color) const {
		if (m_d3dxSprite != nullptr) {
			CComPtr<IDirect3DTexture9> pD3DTexture9 = nullptr;
			const auto pTexture9 = boost::dynamic_pointer_cast<const D3DTextureObject9>(pTexture->GetObject());
			if (pTexture9 != nullptr)
				pD3DTexture9 = pTexture9->GetD3DTexture9();

			if (pD3DTexture9 != nullptr)
				m_d3dxSprite->Draw(pD3DTexture9, pScreenRect, reinterpret_cast<const D3DXVECTOR3*>(pCenter),
								   reinterpret_cast<const D3DXVECTOR3*>(pPosition), color);
		}
	}
}
