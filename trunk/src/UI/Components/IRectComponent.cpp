#include "Misc/stdafx.h"
#include "IRectComponent.h"

namespace UI {
namespace Components {
	RECT IRectComponent::GetFullRect() const {
		RECT fullRect = {0};
		Utils::Vector2 vPosition = GetScreenPosition();
		fullRect.left = static_cast<LONG>(vPosition.x);
		fullRect.right = static_cast<LONG>(vPosition.x + GetWidth());
		fullRect.top = static_cast<LONG>(vPosition.y);
		fullRect.bottom = static_cast<LONG>(vPosition.y + GetHeight());
		return fullRect;
	}

	RECT IRectComponent::GetDisplayRect() const {
		RECT fullRect = GetFullRect();
		RECT dispRect = fullRect;

		std::shared_ptr<ComponentInterface> pInterface = GetInterface();
		if (pInterface != nullptr && !pInterface->ClipStack.empty())
			IntersectRect(&dispRect, &fullRect, pInterface->ClipStack.top());

		return dispRect;
	}
}
}
