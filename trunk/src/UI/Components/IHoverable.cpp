#include "Misc/stdafx.h"
#include "IHoverable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	void IHoverable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsHovered() && !OnHoverEndRequest())
				ClearHover();

			return;
		}

		Utils::Vector2 vPosition(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
			case WM_MOUSEMOVE: {
				bool bInRect = PtInBoundingRect(vPosition);
				if (!m_isHovered && (!sWndProc.LastMessageHandled && bInRect) &&
					!OnHoverStartRequest())
					StartHover();

				else if (m_isHovered && (sWndProc.LastMessageHandled || !bInRect) &&
					!OnHoverEndRequest())
					ClearHover();
			}
			break;
		}
	}
}
}
