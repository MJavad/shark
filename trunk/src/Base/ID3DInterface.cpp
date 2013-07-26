#include "Misc/stdafx.h"
#include "ID3DInterface.h"

void ID3DInterface::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
	// proxy class
	luabind::module(luaState.get()) [
		luabind::class_<ID3DInterface, boost::shared_ptr<ID3DInterface>>("ID3DInterface")
	];
}
