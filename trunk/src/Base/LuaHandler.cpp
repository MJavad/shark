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
#include "LuaHandler.h"
#include "Misc/LuaEvent.h"

#include "UI/GUIManager.h"
#include "D3DManager.h"
#include "FileManager.h"

/* IMPORTANT:
	All classes which are exposed to lua using luabind MUST define a constructor, otherwise it will lead to heap corruption!

	-- Master674
*/

void LuaHandler::Initialize() {

}

boost::shared_ptr<ScriptObject> LuaHandler::CreateNewObject() const {
	const auto scriptObject = boost::make_shared<ScriptObject>();
	const auto& luaState = scriptObject->GetLuaState();

	luabind::module(luaState.get()) [
		// bind float2
		luabind::class_<float2>("float2")
			.def(luabind::constructor<>())
			.def_readwrite("_1", &float2::_1)
			.def_readwrite("_2", &float2::_2),

		// bind float3
		luabind::class_<float3>("float3")
			.def(luabind::constructor<>())
			.def_readwrite("_1", &float3::_1)
			.def_readwrite("_2", &float3::_2)
			.def_readwrite("_3", &float3::_3),

		// bind float4
		luabind::class_<float4>("float4")
			.def(luabind::constructor<>())
			.def_readwrite("_1", &float4::_1)
			.def_readwrite("_2", &float4::_2)
			.def_readwrite("_3", &float4::_3)
			.def_readwrite("_4", &float4::_4),

		// bind d3dxcolor
		luabind::class_<D3DXCOLOR>("ColorValue")
			.def(luabind::constructor<>())
			.def(luabind::constructor<D3DXCOLOR>())
			.def(luabind::constructor<DWORD>())
			.def(luabind::constructor<FLOAT, FLOAT, FLOAT, FLOAT>())
			.def_readwrite("r", &D3DXCOLOR::r)
			.def_readwrite("g", &D3DXCOLOR::g)
			.def_readwrite("b", &D3DXCOLOR::b)
			.def_readwrite("a", &D3DXCOLOR::a)
	];

	// bind utils
	Utils::LuaEvent<void ()>::BindToLua(luaState);
	Utils::Vector2::BindToLua(luaState);
	Utils::Vector3::BindToLua(luaState);

	D3DManager::BindToLua(luaState);
	UI::GUIManager::BindToLua(luaState);

	return scriptObject;
}

bool LuaHandler::ScriptLoaded(std::wstring fileName) const {
	// do a lowercase comparision
	std::transform(fileName.begin(), fileName.end(),
				   fileName.begin(), std::tolower);

	for (const auto& scriptObject: m_activeObjects) {
		std::wstring currentFile = scriptObject->GetScriptName();
		std::transform(currentFile.begin(), currentFile.end(),
					   currentFile.begin(), std::tolower);

		if (fileName == currentFile)
			return true;
	}

	return false;
}

boost::shared_ptr<ScriptObject> LuaHandler::LoadFromFile(const std::wstring &fileName) {
	if (ScriptLoaded(fileName))
		throw std::runtime_error("The script you tried to load is already loaded!");

	const auto scriptObject = CreateNewObject();
	const auto& luaState = scriptObject->GetLuaState();
	scriptObject->SetScriptName(fileName);
	m_activeScript = scriptObject;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string fileName_utf8 = conv.to_bytes(sFileMgr.GetScriptsDirectory() + L'\\' + fileName);

	if (!luaL_dofile(luaState.get(), fileName_utf8.c_str()))
		ExecuteFunction<void>(scriptObject, "Load");
	else
		throw std::runtime_error(PopError(luaState.get()).c_str());

	m_activeObjects.push_back(scriptObject);
	return scriptObject;
}

boost::shared_ptr<ScriptObject> LuaHandler::GetObjectByInterpreter(lua_State *L) const {
	for (const auto& object: m_activeObjects) {
		if (object->GetLuaState().get() == L)
			return object;
	}

	return nullptr;
}

std::string LuaHandler::PopError(lua_State *L) const {
	const char *msg = lua_tostring(L, -1);
	if (msg != nullptr) {
		std::string result(msg);
		lua_pop(L, 1);
		return std::string("lua> runtime error: ") + result;
	}

	return "lua> unknown error";
}
