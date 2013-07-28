#include "Misc/stdafx.h"
#include "ScriptObject.h"

ScriptObject::ScriptObject() : m_scriptName(L"<local>") {
	m_luaState = boost::shared_ptr<lua_State>(luaL_newstate(), lua_close);
	luaL_openlibs(m_luaState.get());
	luabind::open(m_luaState.get());
}
