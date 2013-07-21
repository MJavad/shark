#pragma once
#include "ID3DTextureObject.h"

namespace UI {
	class D3DTextureObject9 : public ID3DTextureObject
	{
	public:
		D3DTextureObject9(CComPtr<IDirect3DTexture9> pTexture) : m_d3dTexture9(pTexture) {}
		D3DTextureObject9(IDirect3DDevice9 *pDevice, const TextureDescription &textureDesc);
		CComPtr<IDirect3DTexture9> GetD3DTexture9() const { return m_d3dTexture9; }
		virtual boost::shared_ptr<ID3DSurface> QuerySurface(uint32 uLevel) const;

	protected:
		CComPtr<IDirect3DTexture9> m_d3dTexture9;
	};
}
