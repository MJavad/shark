#pragma once
#include "ID3DTextureObject.h"

namespace UI {
	class D3DTexture
	{
	public:
		D3DTexture(std::shared_ptr<ID3DTextureObject> pTextureObject) :
			m_textureObject(std::move(pTextureObject)),
			m_hasDescription(false) {}

		D3DTexture(const STextureDesc &textureInfo):
			m_textureDesc(textureInfo),
			m_hasDescription(true) {}

		void SetDevice(IDirect3DDevice9 *pDevice);

		const STextureDesc& GetDescription() const { return m_textureDesc; }
		std::shared_ptr<ID3DTextureObject> GetObject() const { return m_textureObject; }
		bool IsCreated() const { return m_textureObject != nullptr; }

	protected:
		bool m_hasDescription;
		STextureDesc m_textureDesc;
		std::shared_ptr<ID3DTextureObject> m_textureObject;
	};
}
