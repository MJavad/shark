#pragma once
#include "ItemsControl.h"
#include "Button.h"

namespace UI {
namespace Components {
	class TabPage : public Button
	{
	public:
		static std::shared_ptr<TabPage> Create(std::wstring swText, float fWidth, float fHeight);
		virtual void OnRender(uint32 uTimePassed);
		void RenderChildren(uint32 uTimePassed);

		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void SendMessageToChildren(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent);

		std::shared_ptr<ItemsControl> GetContent() const {
			return m_content;
		}

		void SetContent(std::shared_ptr<ItemsControl> pContent) {
			m_content = std::move(pContent);
			if (m_content != nullptr)
				m_content->SetUIParent(shared_from_this());
		}

	protected:
		virtual void _notifyPushEvent(Utils::Vector2 *pvPosition);
		virtual void _notifyClickEvent(Utils::Vector2 *pvPosition);

		virtual bool _notifyFocusStartEvent() {
			FadeTo(200, GetHoverColor());
			return IFocusable::_notifyFocusStartEvent();
		}

		virtual void _notifyFocusEndEvent() {
			FadeTo(200, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return IFocusable::_notifyFocusEndEvent();
		}

	private:
		std::shared_ptr<ItemsControl> m_content;
	};
}
}
