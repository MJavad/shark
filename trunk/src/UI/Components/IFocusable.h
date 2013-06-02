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

			const auto pActiveFocus = GetActiveFocus();
			if (pActiveFocus != nullptr)
				pActiveFocus->OnFocusEndEventNotify();

			s_activeFocus = get_this<IFocusable>();
			return !OnFocusStartEventNotify();
		}

		virtual void Unfocus() {
			if (!IsFocused())
				return;

			s_activeFocus.reset();
			OnFocusEndEventNotify();
		}

		static std::shared_ptr<IFocusable> GetActiveFocus() {
			return s_activeFocus.lock();
		}

		bool IsFocused() const {
			return GetActiveFocus().get() == this;
		}

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
			if (!IsFocused() && Focus())
				return true;

			return IComponent::OnTabPressed(pComponent);
		}

		Utils::Event<bool (const std::shared_ptr<IFocusable>&)> OnFocusStartEvent;
		Utils::Event<void (const std::shared_ptr<IFocusable>&)> OnFocusEndEvent;

	protected:
		virtual bool OnFocusStartEventNotify() {
			return OnFocusStartEvent(get_this<IFocusable>());
		}

		virtual void OnFocusEndEventNotify() {
			OnFocusEndEvent(get_this<IFocusable>());
		}

		static std::weak_ptr<IFocusable> s_activeFocus;
	};
}
}
