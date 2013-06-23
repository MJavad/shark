#pragma once
#include "D3DTexture.h"

namespace UI {
	class ID3DSprite abstract
	{
	public:
		ID3DSprite() {
			D3DXMatrixIdentity(&m_transform);
		}

		virtual ~ID3DSprite() {}
		virtual void OnLostDevice() const {}
		virtual void OnResetDevice() const {}
		virtual void Begin(uint32 dwFlags) {}
		virtual void Draw(const std::shared_ptr<const D3DTexture> &pTexture, const RECT *pScreenRect,
			const Utils::Vector3 *pCenter, const Utils::Vector3 *pPosition, const D3DXCOLOR &color) const = 0;
		virtual void End() {}

		virtual void SetTransform(const D3DXMATRIX *pMatrix) {
			if (pMatrix != nullptr)
				m_transform = *pMatrix;
			else
				D3DXMatrixIdentity(&m_transform);
		}

		virtual void GetTransform(D3DXMATRIX *pMatrix) const {
			*pMatrix = m_transform;
		}

	protected:
		D3DXMATRIX m_transform;
	};
}
