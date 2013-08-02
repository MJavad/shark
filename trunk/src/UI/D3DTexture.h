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
	class D3DTexture : public virtual Utils::IDynamicObject
	{
	public:
		D3DTexture(boost::shared_ptr<ID3DTextureObject> pTextureObject) :
			m_textureObject(std::move(pTextureObject)),
			m_hasDescription(false) {}

		D3DTexture(const TextureDescription &textureInfo):
			m_textureDesc(textureInfo),
			m_hasDescription(true) {}

		void SetDevice(IDirect3DDevice9 *pDevice);

		void OnLostDevice() {
			if (m_textureDesc.pool == D3DPOOL_DEFAULT)
				m_textureObject.reset();
		}

		void OnResetDevice();

		void Release() {
			m_textureObject.reset();
		}

		const TextureDescription& GetDescription() const { return m_textureDesc; }
		boost::shared_ptr<ID3DTextureObject> GetObject() const { return m_textureObject; }
		bool IsCreated() const { return m_textureObject != nullptr; }

	protected:
		bool m_hasDescription;
		TextureDescription m_textureDesc;
		boost::shared_ptr<ID3DTextureObject> m_textureObject;
	};
}
