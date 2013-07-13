#include "Misc/stdafx.h"
#include "D3DSprite9.h"
#include "D3DTextureObject9.h"

namespace UI {
	D3DSprite9::D3DSprite9(IDirect3DDevice9 *pDevice) {
		HRESULT result = D3DXCreateSprite(pDevice, &m_d3dxSprite);
		if (result != D3D_OK)
			throw std::runtime_error("Could not create sprite object!");
	}

	void D3DSprite9::Draw(const std::shared_ptr<const D3DTexture> &pTexture, const RECT *pScreenRect,
		const Utils::Vector3 *pCenter, const Utils::Vector3 *pPosition, const D3DXCOLOR &color) const {
		if (m_d3dxSprite != nullptr) {
			CComPtr<IDirect3DTexture9> pD3DTexture9 = nullptr;
			const auto pTexture9 = std::dynamic_pointer_cast<const D3DTextureObject9>(pTexture->GetObject());
			if (pTexture9 != nullptr)
				pD3DTexture9 = pTexture9->GetD3DTexture9();

			if (pD3DTexture9 != nullptr)
				m_d3dxSprite->Draw(pD3DTexture9, pScreenRect, reinterpret_cast<const D3DXVECTOR3*>(pCenter),
								   reinterpret_cast<const D3DXVECTOR3*>(pPosition), color);
		}
	}
}
