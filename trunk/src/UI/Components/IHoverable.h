#pragma once
#include "IComponent.h"
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IHoverable abstract : public virtual IRectComponent
	{
	public:
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
				FadeTo(400, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		
		D3DXCOLOR GetHoverColor() const {
			return m_hoverColor;
		}

		void SetHoverColor(const D3DXCOLOR &hoverColor) {
			m_hoverColor = hoverColor;
		}

		// returning true = no hover
		Utils::Event<bool (const boost::shared_ptr<IHoverable>&)> OnHoverStartEvent;
		Utils::Event<bool (const boost::shared_ptr<IHoverable>&)> OnHoverEndEvent;

	protected:
		IHoverable() :
			m_isHovered(false),
			m_hoverColor(1.5f, 1.5f, 1.5f, 1.4f) {}

		virtual bool _notifyHoverStartEvent() {
			LOG_DEBUG("%08X: HoverStart triggered.", this);
			return OnHoverStartEvent(get_this<IHoverable>());
		}

		virtual bool _notifyHoverEndEvent() {
			LOG_DEBUG("%08X: HoverEnd triggered.", this);
			return OnHoverEndEvent(get_this<IHoverable>());
		}

	private:
		bool m_isHovered;
		D3DXCOLOR m_hoverColor;
	};
}
}
