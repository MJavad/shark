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

	std::shared_ptr<ID3DSurface> D3DTextureObject9::QuerySurface(uint32 uLevel) const {
		if (m_d3dTexture9 != nullptr) {
			CComPtr<IDirect3DSurface9> pSurface = nullptr;
			HRESULT result = m_d3dTexture9->GetSurfaceLevel(uLevel, &pSurface);

			if (result == D3D_OK)
				return std::make_shared<D3DSurface9>(pSurface);
		}

		return nullptr;
	}
}
