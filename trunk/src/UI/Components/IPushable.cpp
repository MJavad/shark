#include "Misc/stdafx.h"
#include "IPushable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	void IPushable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Utils::Vector2 vPosition(lParam);

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (!m_isClicking && !sWndProc.LastMessageHandled &&
				PtInBoundingRect(vPosition)) {
				_notifyPushEvent(&vPosition);

				uint32 time_now = timeGetTime();
				if (m_lastClick + GetDoubleClickTime() > time_now) {
					m_lastClick = 0;
					_notifyDblClickEvent(&vPosition);
				}
				else
					m_lastClick = time_now;

				m_isClicking = true;
				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_LBUTTONUP:
			if (m_isClicking) {
				_notifyReleaseEvent(&vPosition);
				if (!sWndProc.LastMessageHandled &&
					PtInBoundingRect(vPosition))
					_notifyClickEvent(&vPosition);

				m_isClicking = false;
				sWndProc.LastMessageHandled = true;
			}
			break;
		}
	}
}
}
