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
		virtual void OnPushEventNotify(Utils::Vector2 *pvPosition);
		virtual void OnClickEventNotify(Utils::Vector2 *pvPosition);

		virtual bool OnFocusStartEventNotify() {
			FadeTo(200, GetHoverColor());
			return IFocusable::OnFocusStartEventNotify();
		}

		virtual void OnFocusEndEventNotify() {
			float4 fDefault = {1.0f, 1.0f, 1.0f, 1.0f};
			FadeTo(200, fDefault);
			return IFocusable::OnFocusEndEventNotify();
		}

	private:
		std::shared_ptr<ItemsControl> m_content;
	};
}
}
