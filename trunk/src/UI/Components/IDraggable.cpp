#include "Misc/stdafx.h"
#include "IDraggable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	Utils::Vector2 IDraggable::s_dragVector;
	std::weak_ptr<IDraggable> IDraggable::s_activeDrag;

	void IDraggable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsDragged() && !_notifyDragEndEvent(nullptr))
				ClearDrag();

			return;
		}

		Utils::Vector2 position(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			if (!sWndProc.LastMessageHandled) {
				if (!IsDragged() && PtInBoundingRect(position) &&
					!_notifyDragStartEvent(&position))
					StartDrag(position - GetScreenPosition());

				sWndProc.LastMessageHandled |= IsDragged();
			}
			break;

		case WM_LBUTTONUP:
			if (IsDragged()) {
				if (!_notifyDragEndEvent(&position))
					ClearDrag();

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_MOUSEMOVE:
			if (IsDragged() && GetInterface()->ClipStack.PtInClipArea(position) &&
				!_notifyDragMoveEvent(&position)) {
				position -= s_dragVector;
				auto pParent = GetUIParent();
				if (pParent != nullptr)
					position -= pParent->GetScreenPosition();

				SetPosition(position);
			}

			sWndProc.LastMessageHandled |= IsDragged();
			break;
		};
	}
}
}
