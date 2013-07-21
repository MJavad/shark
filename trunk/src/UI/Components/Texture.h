#pragma once
#include "IRectComponent.h"
#include "UI/D3DTexture.h"

namespace UI {
namespace Components {
	class Texture : public virtual IRectComponent
	{
	public:
		Texture() : m_color(0xFFFFFFFF) {
			D3DXMatrixIdentity(&m_transform);
		}

		static boost::shared_ptr<Texture> Create(boost::shared_ptr<D3DTexture> pTexture);
		virtual void OnRender(uint32 timePassed);

		virtual boost::shared_ptr<D3DTexture> GetTexture() const {
			return m_texture;
		}

		virtual void SetTexture(boost::shared_ptr<D3DTexture> pTexture) {
			m_texture = std::move(pTexture);
		}

		virtual D3DXMATRIX GetTransform() const {
			return m_transform;
		}

		virtual void SetTransform(D3DXMATRIX *pMatrix) {
			if (pMatrix != nullptr)
				m_transform = *pMatrix;
			else
				D3DXMatrixIdentity(&m_transform);
		}

		virtual D3DXCOLOR GetColor() const {
			return m_color;
		}

		virtual void SetColor(const D3DXCOLOR &color) {
			m_color = color;
		}

	private:
		D3DXMATRIX m_transform;
		boost::shared_ptr<D3DTexture> m_texture;
		D3DXCOLOR m_color;
	};
}
}
