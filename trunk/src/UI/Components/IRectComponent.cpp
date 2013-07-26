#include "Misc/stdafx.h"
#include "IRectComponent.h"

namespace UI {
namespace Components {
	RECT IRectComponent::GetFullRect() const {
		RECT fullRect = {0};
		Utils::Vector2 position = GetScreenPosition();
		fullRect.left = static_cast<LONG>(position.x);
		fullRect.right = static_cast<LONG>(position.x + GetWidth());
		fullRect.top = static_cast<LONG>(position.y);
		fullRect.bottom = static_cast<LONG>(position.y + GetHeight());
		return fullRect;
	}

	RECT IRectComponent::GetDisplayRect() const {
		RECT fullRect = GetFullRect();
		RECT dispRect = fullRect;

		boost::shared_ptr<ComponentInterface> pInterface = GetGlobalInterface();
		if (pInterface != nullptr && !pInterface->ClipStack.IsEmpty())
			IntersectRect(&dispRect, &fullRect, pInterface->ClipStack.Top());

		return dispRect;
	}

	void IRectComponent::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<IRectComponent, IComponent,
							boost::shared_ptr<IComponent>>("IRectComponent")
				.def("PtInBoundingRect", &IRectComponent::PtInBoundingRect)
				.property("width", &IRectComponent::GetWidth, &IRectComponent::SetWidth)
				.property("height", &IRectComponent::GetHeight, &IRectComponent::SetHeight)
				.property("fullRect", &IRectComponent::GetFullRect)
				.property("displayRect", &IRectComponent::GetDisplayRect)
		];
	}
}
}
