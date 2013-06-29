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
		Utils::Event<void (const std::shared_ptr<IPushable>&, Utils::Vector2*)> OnDblClickEvent;
		Utils::Event<void (const std::shared_ptr<IPushable>&, Utils::Vector2*)> OnPushEvent;
		Utils::Event<void (const std::shared_ptr<IPushable>&, Utils::Vector2*)> OnReleaseEvent;

	protected:
		IPushable() : m_isClicking(false), m_lastClick(0) {}

		virtual void _notifyClickEvent(Utils::Vector2 *pvPosition) {
			LOG_DEBUG("%08X: Click triggered.", this);
			OnClickEvent(get_this<IPushable>(), pvPosition);
		}

		virtual void _notifyDblClickEvent(Utils::Vector2 *pvPosition) {
			LOG_DEBUG("%08X: DblClick triggered.", this);
			OnDblClickEvent(get_this<IPushable>(), pvPosition);
		}

		virtual void _notifyPushEvent(Utils::Vector2 *pvPosition) {
			LOG_DEBUG("%08X: Push triggered.", this);
			OnPushEvent(get_this<IPushable>(), pvPosition);
		}

		virtual void _notifyReleaseEvent(Utils::Vector2 *pvPosition) {
			LOG_DEBUG("%08X: Release triggered.", this);
			OnReleaseEvent(get_this<IPushable>(), pvPosition);
		}

	private:
		bool m_isClicking;
		uint32 m_lastClick;
	};
}
}
