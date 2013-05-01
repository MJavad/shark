#pragma once
#include "ID3DSurface.h"

namespace UI {
	class D3DSurface9 : public ID3DSurface
	{
	public:
		D3DSurface9(CComPtr<IDirect3DSurface9> pSurface) : m_surface9(pSurface) {}

		CComPtr<IDirect3DSurface9> GetSurface9() const {
			return m_surface9;
		}

	protected:
		CComPtr<IDirect3DSurface9> m_surface9;
	};
}
