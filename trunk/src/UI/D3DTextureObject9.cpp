#include "Misc/stdafx.h"
#include "D3DTextureObject9.h"
#include "D3DSurface9.h"

namespace UI {
	D3DTextureObject9::D3DTextureObject9(IDirect3DDevice9 *pDevice, const STextureDesc &textureDesc) {
		HRESULT hResult = S_FALSE;
		switch(textureDesc.TextureType)
		{
		case TEXTURE_FROM_FILE:
			hResult = D3DXCreateTextureFromFileExW(pDevice, textureDesc.FilePathOrResource.c_str(),
				textureDesc.Width, textureDesc.Height, textureDesc.MipLevels, textureDesc.Usage,
				textureDesc.Format, textureDesc.Pool, textureDesc.Filter, textureDesc.MipFilter,
				textureDesc.ColorKey, nullptr, nullptr, &m_d3dTexture9);
			break;

		case TEXTURE_FROM_RESOURCE:
			hResult = D3DXCreateTextureFromResourceExW(pDevice, textureDesc.ResourceModule,
				textureDesc.FilePathOrResource.c_str(), textureDesc.Width, textureDesc.Height,
				textureDesc.MipLevels, textureDesc.Usage, textureDesc.Format, textureDesc.Pool,
				textureDesc.Filter, textureDesc.MipFilter, textureDesc.ColorKey, nullptr,
				nullptr, &m_d3dTexture9);
			break;
		}

		assert(hResult == D3D_OK);
	}

	std::shared_ptr<ID3DSurface> D3DTextureObject9::QuerySurface(uint32 uLevel) const {
		if (m_d3dTexture9 != nullptr) {
			CComPtr<IDirect3DSurface9> pSurface = nullptr;
			HRESULT hResult = m_d3dTexture9->GetSurfaceLevel(uLevel, &pSurface);

			if (hResult == D3D_OK)
				return std::make_shared<D3DSurface9>(pSurface);
		}

		return nullptr;
	}
}
