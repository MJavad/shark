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
#include "ID3DSprite.h"

namespace UI {
	class D3DSprite9 : public ID3DSprite
	{
	public:
		D3DSprite9(IDirect3DDevice9 *pDevice);

		virtual void OnLostDevice() const {
			if (m_d3dxSprite != nullptr)
				m_d3dxSprite->OnLostDevice();
		}

		virtual void OnResetDevice() const {
			if (m_d3dxSprite != nullptr)
				m_d3dxSprite->OnResetDevice();
		}

		virtual void Begin(uint32 flags) {
			if (m_d3dxSprite != nullptr)
				m_d3dxSprite->Begin(flags);
		}

		virtual void Draw(const boost::shared_ptr<const D3DTexture> &pTexture, const RECT *pScreenRect,
			const Utils::Vector3 *pCenter, const Utils::Vector3 *pPosition, const D3DXCOLOR &color) const;

		virtual void End() {
			if (m_d3dxSprite != nullptr)
				m_d3dxSprite->End();
		}

		virtual void SetTransform(const D3DXMATRIX *pMatrix) {
			ID3DSprite::SetTransform(pMatrix);
			D3DXMATRIX matTransform;
			GetTransform(&matTransform);

			if (m_d3dxSprite != nullptr)
				m_d3dxSprite->SetTransform(&matTransform);
		}

		CComPtr<ID3DXSprite> GetD3DXSprite() const { return m_d3dxSprite; }

	protected:
		CComPtr<ID3DXSprite> m_d3dxSprite;
	};
}
