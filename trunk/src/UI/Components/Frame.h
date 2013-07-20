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

		static std::shared_ptr<Frame> Create(float width, float height);

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
			return ItemsControl::OnTabPressed(pComponent);
		}

		bool GetSizable() const {
			return m_sizable;
		}

		void SetSizable(bool sizable) {
			m_sizable = sizable;

			if (IsSizing()) {
				_notifyResizeEndEvent(nullptr);
				ResetActiveSizer();
			}
		}

		virtual void SetWidth(float width);
		virtual void SetHeight(float height);

		virtual void SetPosition(const Utils::Vector2 &position) {
			ItemsControl::SetPosition(position);
			OnSetPositionEvent(get_this<Frame>(), position);
		}

		// When it handled the most recent event
		Utils::Event<void (const std::shared_ptr<Frame>&)> OnFrameHighlightStartEvent;
		Utils::Event<void (const std::shared_ptr<Frame>&)> OnFrameHighlightEndEvent;
		Utils::Event<void (const std::shared_ptr<Frame>&, const Utils::Vector2&)> OnSetPositionEvent;

	private:
		bool m_sizable;
		bool m_activeFocus;
	};
}
}
