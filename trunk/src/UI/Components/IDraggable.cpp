#include "Misc/stdafx.h"
#include "IDraggable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	Utils::Vector2 IDraggable::s_dragVector;
	std::weak_ptr<IDraggable> IDraggable::s_activeDrag;

	void IDraggable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsDragged() && !OnDragEndEventNotify(nullptr))
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
					!OnDragStartEventNotify(&vPosition))
					StartDrag(vPosition - GetScreenPosition());

				sWndProc.LastMessageHandled |= IsDragged();
			}
			break;

		case WM_LBUTTONUP:
			if (IsDragged()) {
				if (!OnDragEndEventNotify(&vPosition))
					ClearDrag();

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_MOUSEMOVE:
			if (!sWndProc.LastMessageHandled &&
				IsDragged() && GetInterface()->ClipStack.PtInClipArea(vPosition) &&
				!OnDragMoveEventNotify(&vPosition)) {
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
