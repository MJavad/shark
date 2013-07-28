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

#pragma once

class ScriptObject {
public:
	ScriptObject();

	const boost::shared_ptr<lua_State>& GetLuaState() const {
		return m_luaState;
	}

	const std::wstring& GetScriptName() const {
		return m_scriptName;
	}

	void SetScriptName(std::wstring name) {
		m_scriptName = std::move(name);
	}

private:
	std::wstring m_scriptName;
	boost::shared_ptr<lua_State> m_luaState;
};
