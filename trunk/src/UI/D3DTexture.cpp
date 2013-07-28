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
#include "D3DTexture.h"
#include "D3DTextureObject9.h"
#include "Base/D3DManager.h"

namespace UI {
	void D3DTexture::SetDevice(IDirect3DDevice9 *pDevice) {
		if (m_hasDescription)
			m_textureObject = boost::make_shared<D3DTextureObject9>(pDevice, m_textureDesc);
	}

	void D3DTexture::OnResetDevice() {
		if (m_textureDesc.pool == D3DPOOL_DEFAULT) {
			if (m_hasDescription)
				m_textureObject = boost::make_shared<D3DTextureObject9>(sD3DMgr.GetDevice9(), m_textureDesc);
			else
				LOG_DEBUG("Device Reset failed: "
					"Could not recreate texture - missing description for D3DPOOL_DEFAULT");
		}
	}
}
