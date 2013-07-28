#pragma once
#include "ScriptObject.h"

class LuaHandler {
	SINGLETON_CLASS(LuaHandler) {}

public:
	void Initialize();

	bool ScriptLoaded(std::wstring fileName) const;
	boost::shared_ptr<ScriptObject> CreateNewObject() const;
	boost::shared_ptr<ScriptObject> LoadFromFile(const std::wstring &fileName);

	std::string PopError(lua_State *L) const;

private:
	std::list<boost::shared_ptr<ScriptObject>> m_scriptObjects;
};

#define sLuaHandler ::LuaHandler::Instance()
