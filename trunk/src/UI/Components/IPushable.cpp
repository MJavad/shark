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
#include "IPushable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::weak_ptr<IPushable> IPushable::s_activeClick;

	void IPushable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Utils::Vector2 position(lParam);

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (!sWndProc.LastMessageHandled && IsVisible() &&
				!IsPressed() && PtInBoundingRect(position)) {
				const auto activeClick = GetActiveClick();
				s_activeClick = get_this<IPushable>();

				// tell old control that it's no longer pressed...
				if (activeClick != nullptr)
					activeClick->_notifyReleaseEvent(&position);

				// tell new control that it's now pressed...
				_notifyPushEvent(&position);

				// double click?
				uint32 time_now = timeGetTime();
				if (m_lastClick + GetDoubleClickTime() > time_now) {
					m_lastClick = 0;
					_notifyDblClickEvent(&position);
				}
				else
					m_lastClick = time_now;

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_LBUTTONUP:
			if (IsPressed()) {
				s_activeClick.reset();
				_notifyReleaseEvent(&position);

				if (!sWndProc.LastMessageHandled &&
					PtInBoundingRect(position))
					_notifyClickEvent(&position);

				sWndProc.LastMessageHandled = true;
			}
			break;
		}
	}

	void IPushable::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<IPushable, IComponent,
							boost::shared_ptr<IComponent>>("IPushable")
				.def_readonly("clickEvent", &IPushable::OnClickLuaWrap)
				.def_readonly("dblClickEvent", &IPushable::OnDblClickLuaWrap)
				.def_readonly("pushEvent", &IPushable::OnPushLuaWrap)
				.def_readonly("releaseEvent", &IPushable::OnReleaseLuaWrap)
				.property("pressed", &IPushable::IsPressed)
		];
	}
}
}
