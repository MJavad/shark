#pragma once
#include "IComponent.h"

namespace UI {
namespace Components {
	class IRectComponent abstract : public virtual IComponent
	{
	public:
		IRectComponent() : m_width(0), m_height(0) {}

		virtual float GetWidth() const {
			return m_width;
		}

		virtual void SetWidth(float fWidth) {
			m_width = fWidth;
		}

		virtual float GetHeight() const {
			return m_height;
		}

		virtual void SetHeight(float fHeight) {
			m_height = fHeight;
		}

		bool PtInBoundingRect(const POINT &vPoint) const {
			RECT dispRect = GetDisplayRect();
			return PtInRect(&dispRect, vPoint) != FALSE;
		}

		RECT GetFullRect() const;
		RECT GetDisplayRect() const;

	protected:
		float m_width, m_height;
	};
}
}
