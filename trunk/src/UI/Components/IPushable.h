#pragma once
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IPushable abstract : public virtual IRectComponent
	{
	public:
		IPushable() : m_isClicking(false) {}
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		Utils::Event<void (const Utils::Vector2&)> OnClickEvent;
		Utils::Event<void (const Utils::Vector2&)> OnPushEvent;
		Utils::Event<void (const Utils::Vector2&)> OnReleaseEvent;

	protected:
		virtual void OnClickRequest(const Utils::Vector2 &vPosition) {
			OnClickEvent(vPosition);
		}

		virtual void OnPushRequest(const Utils::Vector2 &vPosition) {
			OnPushEvent(vPosition);
		}

		virtual void OnReleaseRequest(const Utils::Vector2 &vPosition) {
			OnReleaseEvent(vPosition);
		}

		bool m_isClicking;
	};
}
}
