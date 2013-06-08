#pragma once
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IPushable abstract : public virtual IRectComponent
	{
	public:
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool IsPressed() const {
			return m_isClicking;
		}

		Utils::Event<void (const std::shared_ptr<IPushable>&, Utils::Vector2*)> OnClickEvent;
		Utils::Event<void (const std::shared_ptr<IPushable>&, Utils::Vector2*)> OnPushEvent;
		Utils::Event<void (const std::shared_ptr<IPushable>&, Utils::Vector2*)> OnReleaseEvent;

	protected:
		IPushable() : m_isClicking(false) {}

		virtual void OnClickEventNotify(Utils::Vector2 *pvPosition) {
			OnClickEvent(get_this<IPushable>(), pvPosition);
		}

		virtual void OnPushEventNotify(Utils::Vector2 *pvPosition) {
			OnPushEvent(get_this<IPushable>(), pvPosition);
		}

		virtual void OnReleaseEventNotify(Utils::Vector2 *pvPosition) {
			OnReleaseEvent(get_this<IPushable>(), pvPosition);
		}

	private:
		bool m_isClicking;
	};
}
}
