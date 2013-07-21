#pragma once
#include "IComponent.h"
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IFocusable abstract : public virtual IComponent
	{
	public:
		virtual ~IFocusable() {
			if (IsFocused())
				s_activeFocus.reset();
		}

		virtual bool Focus() {
			if (IsFocused())
				return true;

			const auto activeFocus = GetActiveFocus();
			if (activeFocus != nullptr)
				activeFocus->_notifyFocusEndEvent();

			s_activeFocus = get_this<IFocusable>();
			return !_notifyFocusStartEvent();
		}

		virtual void Unfocus() {
			if (!IsFocused())
				return;

			s_activeFocus.reset();
			_notifyFocusEndEvent();
		}

		static boost::shared_ptr<IFocusable> GetActiveFocus() {
			return s_activeFocus.lock();
		}

		bool IsFocused() const {
			return GetActiveFocus().get() == this;
		}

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
			if (!IsFocused() && Focus())
				return true;

			return IComponent::OnTabPressed(pComponent);
		}

		Utils::Event<bool (const boost::shared_ptr<IFocusable>&)> OnFocusStartEvent;
		Utils::Event<void (const boost::shared_ptr<IFocusable>&)> OnFocusEndEvent;

	protected:
		virtual bool _notifyFocusStartEvent() {
			LOG_DEBUG("%08X: FocusStart triggered.", this);
			return OnFocusStartEvent(get_this<IFocusable>());
		}

		virtual void _notifyFocusEndEvent() {
			LOG_DEBUG("%08X: FocusEnd triggered.", this);
			OnFocusEndEvent(get_this<IFocusable>());
		}

	private:
		static boost::weak_ptr<IFocusable> s_activeFocus;
	};
}
}
