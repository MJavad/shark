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
#include "ScriptObject.h"
#include "D3DManager.h"

ScriptObject::ScriptObject() : m_scriptName(L"<local>") {
	m_luaState = boost::shared_ptr<lua_State>(luaL_newstate(), lua_close);
	luaL_openlibs(m_luaState.get());
	luabind::open(m_luaState.get());
}

void ScriptObject::DestroyLuaReferences() {
	for (const auto& scriptInterface: m_scriptInterfaces) {
		if (!scriptInterface.expired())
			sD3DMgr.PopInterface(scriptInterface.lock());
	}

	for (const auto& scriptComponent: m_scriptComponents) {
		const auto pComponent = scriptComponent.lock();
		if (pComponent != nullptr) {
			const auto pInterface = pComponent->GetLocalInterface();
			if (pInterface != nullptr)
				pInterface->PopControl(pComponent);
		}
	}

	m_scriptInterfaces.clear();
	m_scriptComponents.clear();
}
