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
#include "IFocusable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::weak_ptr<IFocusable> IFocusable::s_activeFocus;

	void IFocusable::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<IFocusable, IComponent,
							boost::shared_ptr<IComponent>>("IFocusable")
				.def_readonly("focusBeginEvent", &IFocusable::OnFocusBeginLuaWrap)
				.def_readonly("focusEndEvent", &IFocusable::OnFocusEndLuaWrap)
				.property("focused", &IFocusable::GetFocus, &IFocusable::SetFocus)
		];
	}
}
}
