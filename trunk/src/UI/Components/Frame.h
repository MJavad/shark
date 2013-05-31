#pragma once
#include "ItemsControl.h"
#include "IDraggable.h"
#include "ISizable.h"

namespace UI {
namespace Components {
	class Frame : public ItemsControl, public IDraggable, public ISizable
	{
	public:
		Frame() : m_sizable(true),
			m_activeFocus(false) {}

		static std::shared_ptr<Frame> Create(float fWidth, float fHeight);

		virtual void OnRender(uint32 uTimePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
			return ItemsControl::OnTabPressed(pComponent);
		}

		bool GetSizable() const {
			return m_sizable;
		}

		void SetSizable(bool bSizable) {
			m_sizable = bSizable;

			if (IsSizing()) {
				OnResizeEndRequest(nullptr);
				ResetActiveSizer();
			}
		}

		virtual void SetWidth(float fWidth);
		virtual void SetHeight(float fHeight);

		virtual void SetPosition(const Utils::Vector2 &vPosition) {
			ItemsControl::SetPosition(vPosition);
			OnSetPositionEvent(get_this<Frame>(), vPosition);
		}

		// When it handled the most recent event
		Utils::Event<void (const std::shared_ptr<Frame>&)> OnFrameHighlightStartEvent;
		Utils::Event<void (const std::shared_ptr<Frame>&)> OnFrameHighlightStopEvent;
		Utils::Event<void (const std::shared_ptr<Frame>&, const Utils::Vector2&)> OnSetPositionEvent;

	protected:
		bool m_sizable;
		bool m_activeFocus;
	};
}
}
