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
#include "IDraggable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	Utils::Vector2 IDraggable::s_dragVector;
	boost::weak_ptr<IDraggable> IDraggable::s_activeDrag;

	void IDraggable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsDragged() && !_notifyDragEndEvent(nullptr))
				ClearDrag();

			return;
		}

		Utils::Vector2 position(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			if (!sWndProc.LastMessageHandled) {
				if (!IsDragged() && PtInBoundingRect(position) &&
					!_notifyDragBeginEvent(&position))
					StartDrag(position - GetScreenPosition());

				sWndProc.LastMessageHandled |= IsDragged();
			}
			break;

		case WM_LBUTTONUP:
			if (IsDragged()) {
				if (!_notifyDragEndEvent(&position))
					ClearDrag();

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_MOUSEMOVE:
			if (IsDragged() && GetGlobalInterface()->ClipStack.PtInClipArea(position) &&
				!_notifyDragMoveEvent(&position)) {
				position -= s_dragVector;
				auto pParent = GetUIParent();
				if (pParent != nullptr)
					position -= pParent->GetScreenPosition();

				SetPosition(position);
			}

			sWndProc.LastMessageHandled |= IsDragged();
			break;
		};
	}

	void IDraggable::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<IDraggable, IRectComponent,
							boost::shared_ptr<IComponent>>("IDraggable")
				.def_readonly("dragBeginEvent", &IDraggable::OnDragBeginLuaWrap)
				.def_readonly("dragEndEvent", &IDraggable::OnDragEndLuaWrap)
				.def_readonly("dragMoveEvent", &IDraggable::OnDragMoveLuaWrap)
				.property("isDragged", &IDraggable::IsDragged)
		];
	}
}
}
