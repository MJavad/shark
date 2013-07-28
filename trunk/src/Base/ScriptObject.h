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
