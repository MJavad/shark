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
#include "D3DTextureObject9.h"
#include "D3DSurface9.h"

namespace UI {
	D3DTextureObject9::D3DTextureObject9(IDirect3DDevice9 *pDevice, const TextureDescription &textureDesc) {
		HRESULT result = S_FALSE;
		switch(textureDesc.type)
		{
		case TEXTURE_FROM_FILE:
			result = D3DXCreateTextureFromFileExW(pDevice, textureDesc.filePath.c_str(),
				textureDesc.width, textureDesc.height, textureDesc.mipLevels, textureDesc.usage,
				textureDesc.format, textureDesc.pool, textureDesc.filter, textureDesc.mipFilter,
				textureDesc.colorKey, nullptr, nullptr, &m_d3dTexture9);
			break;

		case TEXTURE_FROM_RESOURCE:
			result = D3DXCreateTextureFromResourceExW(pDevice, textureDesc.resourceModule,
				MAKEINTRESOURCE(textureDesc.resourceId), textureDesc.width, textureDesc.height,
				textureDesc.mipLevels, textureDesc.usage, textureDesc.format, textureDesc.pool,
				textureDesc.filter, textureDesc.mipFilter, textureDesc.colorKey, nullptr,
				nullptr, &m_d3dTexture9);
			break;
		}

		Utils::ThrowIfFailed(result);
	}

	boost::shared_ptr<ID3DSurface> D3DTextureObject9::QuerySurface(uint32 uLevel) const {
		if (m_d3dTexture9 != nullptr) {
			CComPtr<IDirect3DSurface9> pSurface = nullptr;
			HRESULT result = m_d3dTexture9->GetSurfaceLevel(uLevel, &pSurface);

			if (result == D3D_OK)
				return boost::make_shared<D3DSurface9>(pSurface);
		}

		return nullptr;
	}
}
