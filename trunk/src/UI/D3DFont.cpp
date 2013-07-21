#include "Misc/stdafx.h"
#include "D3DFont.h"
#include "D3DFontObject9.h"

namespace UI {
	void D3DFont::SetDevice(IDirect3DDevice9 *pDevice) {
		m_fontObject = boost::make_shared<D3DFontObject9>(pDevice, m_fontDesc);
	}
}
