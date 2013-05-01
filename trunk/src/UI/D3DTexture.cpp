#include "Misc/stdafx.h"
#include "D3DTexture.h"
#include "D3DTextureObject9.h"

namespace UI {
	void D3DTexture::SetDevice(IDirect3DDevice9 *pDevice) {
		if (m_hasDescription)
			m_textureObject = std::make_shared<D3DTextureObject9>(pDevice, m_textureDesc);
	}
}
