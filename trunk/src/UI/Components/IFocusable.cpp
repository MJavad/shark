#include "Misc/stdafx.h"
#include "IFocusable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::weak_ptr<IFocusable> IFocusable::s_activeFocus;

	void IFocusable::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<IFocusable, IComponent,
							boost::shared_ptr<IComponent>>("IFocusable")
				.def_readonly("focusBeginEvent", &IFocusable::OnFocusBeginLuaWrap)
				.def_readonly("focusEndEvent", &IFocusable::OnFocusEndLuaWrap)
				.property("focused", &IFocusable::GetFocus, &IFocusable::SetFocus)
		];
	}
}
}
