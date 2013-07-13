#pragma once
#include "ID3DFontObject.h"

namespace UI {
	class D3DFont
	{
	public:
		D3DFont(const FontDescription &fontDesc): m_fontDesc(fontDesc) {}

		void SetDevice(IDirect3DDevice9 *pDevice);

		void OnLostDevice() const {
			if (m_fontObject != nullptr)
				m_fontObject->OnLostDevice();
		}

		void OnResetDevice() const {
			if (m_fontObject != nullptr)
				m_fontObject->OnResetDevice();
		}

		void Release() {
			m_fontObject.reset();
		}

		const FontDescription& GetDescription() const { return m_fontDesc; }
		std::shared_ptr<ID3DFontObject> GetObject() const { return m_fontObject; }
		virtual bool IsCreated() const { return m_fontObject != nullptr; }

	protected:
		FontDescription m_fontDesc;
		std::shared_ptr<ID3DFontObject> m_fontObject;
	};
}
