#include "Misc/stdafx.h"
#include "Frame.h"
#include "Rectangle.h"
#include "Base/WndProc.h"
#include "IFocusable.h"

namespace UI {
namespace Components {
	std::shared_ptr<Frame> Frame::Create(float fWidth, float fHeight) {
		const auto pFrame = std::make_shared<Frame>();
		pFrame->SetWidth(fWidth);
		pFrame->SetHeight(fHeight);
		return pFrame;
	}

	void Frame::OnRender(uint32 uTimePassed) {
		GetInterface()->ClipStack.SetRect(GetFullRect(), [&]() {
			ItemsControl::OnRender(uTimePassed);
		});

		if (m_sizable)
			ISizable::OnRender(uTimePassed);
	}

	void Frame::SetWidth(float fWidth) {
		ISizable::SetWidth(fWidth);
		float fHeight = GetHeight();
		for (const auto &pChild: GetChildren())
			pChild->OnParentSizeChanged(fWidth, fHeight);
	}

	void Frame::SetHeight(float fHeight) {
		ISizable::SetHeight(fHeight);
		float fWidth = GetWidth();
		for (const auto &pChild: GetChildren())
			pChild->OnParentSizeChanged(fWidth, fHeight);
	}

	void Frame::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc->LastMessageHandled;

		if (m_sizable)
			ISizable::OnMessageReceived(uMsg, wParam, lParam);

		// Send message to all children and set clip area...
		auto pInterface = GetInterface();
		pInterface->ClipStack.push(GetFullRect());
		ItemsControl::OnMessageReceived(uMsg, wParam, lParam);
		pInterface->ClipStack.pop();
		const auto pThis = get_this<Frame>();

		IDraggable::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
			case WM_CHAR:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN: {
				if (!bHandled && GetVisibility()) {
					Utils::Vector2 vPosition = Utils::Vector2(lParam);

					if (sWndProc->LastMessageHandled ||
						(uMsg != WM_CHAR && PtInBoundingRect(vPosition))) {
						const auto pParent = GetUIParent();
						const auto pClientInterface = GetClientInterface();

						if (pParent != nullptr) {
							auto pItemControl = std::dynamic_pointer_cast<ItemsControl>(pParent);
							if (pItemControl != nullptr)
								pItemControl->PushChild(pThis); // push to front
						}
						else if (pClientInterface != nullptr)
							pClientInterface->PushControl(pThis); // push to front

						if (!m_activeFocus) {
							m_activeFocus = true;
							OnFocusStartEvent();
						}

						sWndProc->LastMessageHandled = true;
					}
				}
				
				if (m_activeFocus && uMsg != WM_CHAR &&
					(bHandled || !sWndProc->LastMessageHandled)) {
					OnFocusStopEvent();
					m_activeFocus = false;
				}
			}
			break;

			case WM_MOUSEMOVE: {
				Utils::Vector2 vPosition = Utils::Vector2(lParam);
				if (GetVisibility() && PtInBoundingRect(vPosition))
					sWndProc->LastMessageHandled = true;
			}
			break;
		}
	}
}
}
