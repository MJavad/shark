#include "Misc/stdafx.h"
#include "IPushable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::weak_ptr<IPushable> IPushable::s_activeClick;

	void IPushable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Utils::Vector2 vPosition(lParam);

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (!sWndProc.LastMessageHandled &&
				!IsPressed() && PtInBoundingRect(vPosition)) {
				const auto activeClick = GetActiveClick();
				s_activeClick = get_this<IPushable>();

				// tell old control that it's no longer pressed...
				if (activeClick != nullptr)
					activeClick->_notifyReleaseEvent(&vPosition);

				// tell new control that it's now pressed...
				_notifyPushEvent(&vPosition);

				// double click?
				uint32 time_now = timeGetTime();
				if (m_lastClick + GetDoubleClickTime() > time_now) {
					m_lastClick = 0;
					_notifyDblClickEvent(&vPosition);
				}
				else
					m_lastClick = time_now;

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_LBUTTONUP:
			if (IsPressed()) {
				s_activeClick.reset();
				_notifyReleaseEvent(&vPosition);

				if (!sWndProc.LastMessageHandled &&
					PtInBoundingRect(vPosition))
					_notifyClickEvent(&vPosition);

				sWndProc.LastMessageHandled = true;
			}
			break;
		}
	}
}
}
