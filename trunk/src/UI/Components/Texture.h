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

		static std::shared_ptr<Texture> Create(std::shared_ptr<D3DTexture> pTexture);
		virtual void OnRender(uint32 uTimePassed);

		virtual std::shared_ptr<D3DTexture> GetTexture() const {
			return m_texture;
		}

		virtual void SetTexture(std::shared_ptr<D3DTexture> pTexture) {
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

		virtual Utils::Color GetColor() const {
			return m_color;
		}

		virtual void SetColor(const Utils::Color &color) {
			m_color = color;
		}

	private:
		D3DXMATRIX m_transform;
		std::shared_ptr<D3DTexture> m_texture;
		Utils::Color m_color;
	};
}
}
