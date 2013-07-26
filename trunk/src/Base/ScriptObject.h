#pragma once

class ScriptObject {
public:
	ScriptObject();

	const boost::shared_ptr<lua_State>& GetLuaState() const {
		return m_luaState;
	}

private:
	boost::shared_ptr<lua_State> m_luaState;
};
