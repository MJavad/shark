#include "Misc/stdafx.h"
#include "D3DTexture.h"
#include "D3DTextureObject9.h"
#include "Base/D3DManager.h"

namespace UI {
	void D3DTexture::SetDevice(IDirect3DDevice9 *pDevice) {
		if (m_hasDescription)
			m_textureObject = std::make_shared<D3DTextureObject9>(pDevice, m_textureDesc);
	}

	void D3DTexture::OnResetDevice() {
		if (m_hasDescription && m_textureDesc.Pool == D3DPOOL_DEFAULT)
			m_textureObject = std::make_shared<D3DTextureObject9>(sD3DMgr->GetDevice9(), m_textureDesc);
	}
}
