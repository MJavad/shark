#pragma once
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IPushable abstract : public virtual IRectComponent
	{
	public:
		IPushable() : m_isClicking(false) {}
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool IsPressed() const {
			return m_isClicking;
		}

		Utils::Event<void (const std::shared_ptr<IPushable>&, const Utils::Vector2&)> OnClickEvent;
		Utils::Event<void (const std::shared_ptr<IPushable>&, const Utils::Vector2&)> OnPushEvent;
		Utils::Event<void (const std::shared_ptr<IPushable>&, const Utils::Vector2&)> OnReleaseEvent;

	protected:
		virtual void OnClickEventNotify(const Utils::Vector2 &vPosition) {
			OnClickEvent(get_this<IPushable>(), vPosition);
		}

		virtual void OnPushEventNotify(const Utils::Vector2 &vPosition) {
			OnPushEvent(get_this<IPushable>(), vPosition);
		}

		virtual void OnReleaseEventNotify(const Utils::Vector2 &vPosition) {
			OnReleaseEvent(get_this<IPushable>(), vPosition);
		}

		bool m_isClicking;
	};
}
}
