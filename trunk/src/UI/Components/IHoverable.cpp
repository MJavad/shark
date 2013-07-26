#include "Misc/stdafx.h"
#include "IHoverable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	void IHoverable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsHovered() && !_notifyHoverEndEvent()) {
				m_isHovered = false;
				SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			return;
		}

		Utils::Vector2 position(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg) {
			case WM_MOUSEMOVE: {
				bool inRectangle = PtInBoundingRect(position);
				if (!m_isHovered &&
					(!sWndProc.LastMessageHandled && inRectangle) &&
					!_notifyHoverBeginEvent())
					StartHover();

				else if (m_isHovered &&
					(sWndProc.LastMessageHandled || !inRectangle) &&
					!_notifyHoverEndEvent())
					ClearHover();
			}
			break;
		}
	}

	void IHoverable::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<IHoverable, IRectComponent,
							boost::shared_ptr<IComponent>>("IHoverable")
				.def_readonly("hoverBeginEvent", &IHoverable::OnHoverBeginLuaWrap)
				.def_readonly("hoverEndEvent", &IHoverable::OnHoverEndLuaWrap)
				.property("hovered", &IHoverable::GetHovered, &IHoverable::SetHovered)
		];
	}
}
}
