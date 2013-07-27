#include "Misc/stdafx.h"
#include "LuaHandler.h"

#include "UI/GUIManager.h"
#include "Base/D3DManager.h"

/* IMPORTANT:
	All classes which are exposed to lua using luabind MUST define a constructor, otherwise it will lead to heap corruption!

	-- Master674
*/

void LuaHandler::Initialize() {
	luabind::set_pcall_callback([] (lua_State *L) {
		luabind::object o(luabind::from_stack(L, -1));
		std::ostringstream msg;
		msg << "lua> runtime error: " << o;
		sLog.OutDebug_UTF8(msg.str());

		luabind::object trace(luabind::globals(L) ["debug"] ["traceback"]);
		std::string stacktrace = luabind::call_function<std::string>(trace);
		sLog.OutDebug_UTF8(std::string("lua> ") + stacktrace);
		return 1;
	});
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
	Utils::Event<void ()>::BindToLua(luaState);
	Utils::Vector2::BindToLua(luaState);
	Utils::Vector3::BindToLua(luaState);

	D3DManager::BindToLua(luaState);
	UI::GUIManager::BindToLua(luaState);

	return scriptObject;
}

boost::shared_ptr<ScriptObject> LuaHandler::LoadFromFile(const std::wstring &fileName) {
	const auto scriptObject = CreateNewObject();
	const auto& luaState = scriptObject->GetLuaState();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string fileName_utf8 = conv.to_bytes(fileName);

	if (!luaL_dofile(luaState.get(), fileName_utf8.c_str()))
		luabind::call_function<void>(luaState.get(), "Load");
	else
		throw std::runtime_error(PopError(luaState.get()).c_str());

	m_scriptObjects.push_back(scriptObject);
	return scriptObject;
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
