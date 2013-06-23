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

		Utils::Vector2 vPosition(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			if (!sWndProc.LastMessageHandled) {
				if (!IsDragged() && PtInBoundingRect(vPosition) &&
					!_notifyDragStartEvent(&vPosition))
					StartDrag(vPosition - GetScreenPosition());

				sWndProc.LastMessageHandled |= IsDragged();
			}
			break;

		case WM_LBUTTONUP:
			if (IsDragged()) {
				if (!_notifyDragEndEvent(&vPosition))
					ClearDrag();

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_MOUSEMOVE:
			if (IsDragged() && GetInterface()->ClipStack.PtInClipArea(vPosition) &&
				!_notifyDragMoveEvent(&vPosition)) {
				vPosition -= s_dragVector;
				auto pParent = GetUIParent();
				if (pParent != nullptr)
					vPosition -= pParent->GetScreenPosition();

				SetPosition(vPosition);
			}

			sWndProc.LastMessageHandled |= IsDragged();
			break;
		};
	}
}
}
