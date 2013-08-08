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
#include "ScriptObject.h"

class LuaHandler {
	SINGLETON_CLASS(LuaHandler) {}

public:
	void Initialize();

	bool ScriptLoaded(std::wstring fileName) const;
	boost::shared_ptr<ScriptObject> CreateNewObject() const;
	boost::shared_ptr<ScriptObject> LoadFromFile(const std::wstring &fileName);
	boost::shared_ptr<ScriptObject> GetObjectByInterpreter(lua_State *L) const;
	void InvalidateScript(const boost::shared_ptr<ScriptObject> &pScript);

	boost::shared_ptr<ScriptObject> GetActiveScript() const {
		return m_activeScript.lock();
	}

	std::string PopError(lua_State *L) const;


	template <typename R /*, typename... Args*/>
	R ExecuteFunction(const boost::shared_ptr<ScriptObject> &scriptObject,
					  const std::string &functionName
					  /*, const Args&... args*/) {
		try {
			m_activeScript = scriptObject;
			R value = luabind::call_function<R>(
				scriptObject->GetLuaState().get(),
				functionName.c_str() /*, args...*/);

			return value;
		}
		catch (luabind::error &e) {
			std::string luaError = PopError(e.state());
			throw std::runtime_error(luaError.c_str());
		}
	}

	template </*typename... Args*/>
	void ExecuteFunction<void /*, Args...*/>(const boost::shared_ptr<ScriptObject> &scriptObject,
											 const std::string &functionName
											 /*, const Args&... args*/) {
		try {
			m_activeScript = scriptObject;
			luabind::call_function<void>(
				scriptObject->GetLuaState().get(),
				functionName.c_str() /*, args...*/);
		}
		catch (luabind::error &e) {
			std::string luaError = PopError(e.state());
			throw std::runtime_error(luaError.c_str());
		}
	}

	template <typename R /*, typename... Args*/>
	R ExecuteFunction(const luabind::object &function
					  /*, const Args&... args*/) {
		try {
			m_activeScript = GetObjectByInterpreter(function.interpreter());
			R value = luabind::call_function<R>(function /*, args...*/);
			return value;
		}
		catch (luabind::error &e) {
			std::string luaError = PopError(e.state());
			throw std::runtime_error(luaError.c_str());
		}
	}

	template </*typename... Args*/>
	void ExecuteFunction<void /*, Args...*/>(const luabind::object &function
											 /*, const Args&... args*/) {
		try {
			m_activeScript = GetObjectByInterpreter(function.interpreter());
			luabind::call_function<void>(function /*, args...*/);
		}
		catch (luabind::error &e) {
			std::string luaError = PopError(e.state());
			throw std::runtime_error(luaError.c_str());
		}
	}

private:
	boost::weak_ptr<ScriptObject> m_activeScript;
	std::list<boost::shared_ptr<ScriptObject>> m_activeObjects;
};

#define sLuaHandler ::LuaHandler::Instance()
