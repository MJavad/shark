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

		void SetHovered(bool hovered) {
			if (hovered)
				StartHover();
			else
				ClearHover();
		}

		bool GetHovered() const {
			return IsHovered();
		}
		
		D3DXCOLOR GetHoverColor() const {
			return m_hoverColor;
		}

		void SetHoverColor(const D3DXCOLOR &hoverColor) {
			m_hoverColor = hoverColor;
		}

		// lua wrapper
		Utils::Event<void ()> OnHoverBeginLuaWrap;
		Utils::Event<void ()> OnHoverEndLuaWrap;

		// returning true = no hover
		Utils::Event<bool (const boost::shared_ptr<IHoverable>&)> OnHoverBeginEvent;
		Utils::Event<bool (const boost::shared_ptr<IHoverable>&)> OnHoverEndEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IHoverable() :
			m_isHovered(false),
			m_hoverColor(1.5f, 1.5f, 1.5f, 1.4f) {}

		virtual bool _notifyHoverBeginEvent() {
			LOG_DEBUG("%08X: HoverStart triggered.", this);
			OnHoverBeginLuaWrap();
			return OnHoverBeginEvent(get_this<IHoverable>());
		}

		virtual bool _notifyHoverEndEvent() {
			LOG_DEBUG("%08X: HoverEnd triggered.", this);
			OnHoverEndLuaWrap();
			return OnHoverEndEvent(get_this<IHoverable>());
		}

	private:
		bool m_isHovered;
		D3DXCOLOR m_hoverColor;
	};
}
}
