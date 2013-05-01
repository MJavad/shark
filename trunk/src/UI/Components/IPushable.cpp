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
			if (!m_isClicking && !sWndProc->LastMessageHandled &&
				PtInBoundingRect(vPosition)) {
				OnPushRequest(vPosition);
				m_isClicking = true;
				sWndProc->LastMessageHandled = true;
			}
			break;

		case WM_LBUTTONUP:
			if (m_isClicking) {
				OnReleaseRequest(vPosition);
				if (!sWndProc->LastMessageHandled &&
					PtInBoundingRect(vPosition))
					OnClickRequest(vPosition);

				m_isClicking = false;
				sWndProc->LastMessageHandled = true;
			}
			break;
		}
	}
}
}
