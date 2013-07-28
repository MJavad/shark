/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
