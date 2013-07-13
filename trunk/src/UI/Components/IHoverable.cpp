#include "Misc/stdafx.h"
#include "IHoverable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	void IHoverable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsHovered() && !_notifyHoverEndEvent())
				ClearHover();

			return;
		}

		Utils::Vector2 position(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
			case WM_MOUSEMOVE: {
				bool bInRect = PtInBoundingRect(position);
				if (!m_isHovered && (!sWndProc.LastMessageHandled && bInRect) &&
					!_notifyHoverStartEvent())
					StartHover();

				else if (m_isHovered && (sWndProc.LastMessageHandled || !bInRect) &&
					!_notifyHoverEndEvent())
					ClearHover();
			}
			break;
		}
	}
}
}
