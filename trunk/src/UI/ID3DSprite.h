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
#include "D3DTexture.h"

namespace UI {
	class ID3DSprite abstract
	{
	public:
		ID3DSprite() {
			D3DXMatrixIdentity(&m_transform);
		}

		virtual ~ID3DSprite() {}
		virtual void OnLostDevice() const {}
		virtual void OnResetDevice() const {}
		virtual void Begin(uint32 flags) {}
		virtual void Draw(const boost::shared_ptr<const D3DTexture> &pTexture, const RECT *pScreenRect,
			const Utils::Vector3 *pCenter, const Utils::Vector3 *pPosition, const D3DXCOLOR &color) const = 0;
		virtual void End() {}

		virtual void SetTransform(const D3DXMATRIX *pMatrix) {
			if (pMatrix != nullptr)
				m_transform = *pMatrix;
			else
				D3DXMatrixIdentity(&m_transform);
		}

		virtual void GetTransform(D3DXMATRIX *pMatrix) const {
			*pMatrix = m_transform;
		}

	protected:
		D3DXMATRIX m_transform;
	};
}
