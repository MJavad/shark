#pragma once
#include "ID3DFontObject.h"

namespace UI {
	class D3DFont
	{
	public:
		D3DFont(const SFontDesc &fontDesc): m_fontDesc(fontDesc) {}

		void SetDevice(IDirect3DDevice9 *pDevice);

		const SFontDesc& GetDescription() const { return m_fontDesc; }
		std::shared_ptr<ID3DFontObject> GetObject() const { return m_fontObject; }
		virtual bool IsCreated() const { return m_fontObject != nullptr; }

	protected:
		SFontDesc m_fontDesc;
		std::shared_ptr<ID3DFontObject> m_fontObject;
	};
}
