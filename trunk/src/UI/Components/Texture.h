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
#include "IRectComponent.h"
#include "UI/D3DTexture.h"

namespace UI {
namespace Components {
	class Texture : public virtual IRectComponent
	{
	public:
		Texture() : m_color(0xFFFFFFFF) {
			D3DXMatrixIdentity(&m_transform);
		}

		static boost::shared_ptr<Texture> CreateDefault() {
			return Create();
		}

		static boost::shared_ptr<Texture> Create(boost::shared_ptr<D3DTexture> pTexture = nullptr);
		virtual void OnRender(uint32 timePassed);

		virtual boost::shared_ptr<D3DTexture> GetTexture() const {
			return m_texture;
		}

		virtual void SetTexture(boost::shared_ptr<D3DTexture> pTexture) {
			m_texture = std::move(pTexture);
		}

		virtual D3DXMATRIX GetTransform() const {
			return m_transform;
		}

		virtual void SetTransform(D3DXMATRIX *pMatrix) {
			if (pMatrix != nullptr)
				m_transform = *pMatrix;
			else
				D3DXMatrixIdentity(&m_transform);
		}

		virtual D3DXCOLOR GetColor() const {
			return m_color;
		}

		virtual void SetColor(const D3DXCOLOR &color) {
			m_color = color;
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		D3DXMATRIX m_transform;
		boost::shared_ptr<D3DTexture> m_texture;
		D3DXCOLOR m_color;
	};
}
}
