#pragma once
#include "IComponent.h"

namespace UI {
namespace Components {
	class IRectComponent abstract : public virtual IComponent
	{
	public:
		virtual float GetWidth() const {
			return m_width;
		}

		virtual void SetWidth(float width) {
			m_width = width;
		}

		virtual float GetHeight() const {
			return m_height;
		}

		virtual void SetHeight(float height) {
			m_height = height;
		}

		bool PtInBoundingRect(const POINT &vPoint) const {
			RECT dispRect = GetDisplayRect();
			return PtInRect(&dispRect, vPoint) != FALSE;
		}

		RECT GetFullRect() const;
		RECT GetDisplayRect() const;

	protected:
		IRectComponent() : m_width(0), m_height(0) {}

	private:
		float m_width, m_height;
	};
}
}
