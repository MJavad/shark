#pragma once
#include "IComponent.h"
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IHoverable abstract : public virtual IRectComponent
	{
	public:
		IHoverable() : m_isHovered(false) {
			m_hoverColor._1 = 1.4f; m_hoverColor._2 = 1.5f;
			m_hoverColor._3 = 1.5f; m_hoverColor._4 = 1.5f;
		}

		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		bool IsHovered() const {
			return m_isHovered;
		}

		void StartHover() {
			if (!m_isHovered) {
				m_isHovered = true;
				FadeTo(75, m_hoverColor);
			}
		}

		void ClearHover() {
			if (m_isHovered) {
				m_isHovered = false;
				float4 fOrig = {1.0f, 1.0f, 1.0f, 1.0f};
				FadeTo(400, fOrig);
			}
		}
		
		float4 GetHoverColor() const {
			return m_hoverColor;
		}

		void SetHoverColor(const float4 &fHoverColor) {
			m_hoverColor = fHoverColor;
		}

		// returning true = no hover
		Utils::Event<bool (const std::shared_ptr<IHoverable>&)> OnHoverStartEvent;
		Utils::Event<bool (const std::shared_ptr<IHoverable>&)> OnHoverEndEvent;

	protected:
		virtual bool OnHoverStartRequest() {
			return OnHoverStartEvent(get_this<IHoverable>());
		}

		virtual bool OnHoverEndRequest() {
			return OnHoverEndEvent(get_this<IHoverable>());
		}

		bool m_isHovered;
		float4 m_hoverColor;
	};
}
}
