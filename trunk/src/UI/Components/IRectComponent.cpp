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

		std::shared_ptr<ComponentInterface> pInterface = GetInterface();
		if (pInterface != nullptr && !pInterface->ClipStack.IsEmpty())
			IntersectRect(&dispRect, &fullRect, pInterface->ClipStack.Top());

		return dispRect;
	}
}
}
