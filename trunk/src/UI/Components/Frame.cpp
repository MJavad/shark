#include "Misc/stdafx.h"
#include "Frame.h"
#include "Rectangle.h"
#include "Base/WndProc.h"
#include "IFocusable.h"

namespace UI {
namespace Components {
	boost::shared_ptr<Frame> Frame::Create(float width, float height) {
		const auto pFrame = boost::make_shared<Frame>();
		pFrame->SetWidth(width);
		pFrame->SetHeight(height);
		return pFrame;
	}

	void Frame::OnRender(uint32 timePassed) {
		GetGlobalInterface()->ClipStack.SetRect(GetFullRect(), [&]() {
			ItemsControl::OnRender(timePassed);
		});

		if (m_sizable)
			ISizable::OnRender(timePassed);
	}

	void Frame::SetWidth(float width) {
		ISizable::SetWidth(width);
		float height = GetHeight();
		for (const auto &pChild: GetChildren())
			pChild->OnParentSizeChanged(width, height);
	}

	void Frame::SetHeight(float height) {
		ISizable::SetHeight(height);
		float width = GetWidth();
		for (const auto &pChild: GetChildren())
			pChild->OnParentSizeChanged(width, height);
	}

	void Frame::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc.LastMessageHandled;

		if (m_sizable)
			ISizable::OnMessageReceived(uMsg, wParam, lParam);

		// Send message to all children and set clip area...
		auto pInterface = GetGlobalInterface();
		pInterface->ClipStack.Push(GetFullRect());
		ItemsControl::OnMessageReceived(uMsg, wParam, lParam);
		pInterface->ClipStack.Pop();
		const auto pThis = get_this<Frame>();

		IDraggable::OnMessageReceived(uMsg, wParam, lParam);

		switch (uMsg)
		{
			case WM_CHAR:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN: {
				if (!bHandled && GetVisibility()) {
					Utils::Vector2 position = Utils::Vector2(lParam);

					if (sWndProc.LastMessageHandled ||
						(uMsg != WM_CHAR && PtInBoundingRect(position))) {
						const auto pParent = GetUIParent();
						const auto pClientInterface = GetLocalInterface();

						if (pParent != nullptr) {
							auto pItemControl = boost::dynamic_pointer_cast<ItemsControl>(pParent);
							if (pItemControl != nullptr)
								pItemControl->PushChild(pThis); // push to front
						}
						else if (pClientInterface != nullptr)
							pClientInterface->PushControl(pThis); // push to front

						if (!m_activeFocus) {
							m_activeFocus = true;
							OnFrameHighlightBeginEvent(pThis);
						}

						sWndProc.LastMessageHandled = true;
					}
				}
				
				if (m_activeFocus && uMsg != WM_CHAR &&
					(bHandled || !sWndProc.LastMessageHandled)) {
					OnFrameHighlightEndEvent(pThis);
					m_activeFocus = false;
				}
			}
			break;

			case WM_MOUSEMOVE: {
				Utils::Vector2 position = Utils::Vector2(lParam);
				if (GetVisibility() && PtInBoundingRect(position))
					sWndProc.LastMessageHandled = true;
			}
			break;
		}
	}

	void Frame::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<Frame,
							luabind::bases<ItemsControl, IDraggable, ISizable>,
							boost::shared_ptr<IComponent>>("Frame")
				.scope [ luabind::def("Create", &Frame::CreateDefault) ]
				.property("sizable", &Frame::GetSizable, &Frame::SetSizable)
		];
	}
}
}
