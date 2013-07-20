#pragma once
#include "IComponent.h"

namespace UI {
namespace Components {
	class ItemsControl : public virtual IComponent
	{
	public:
		static std::shared_ptr<ItemsControl> Create();

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool PushChild(std::shared_ptr<IComponent> pControl);
		bool PopChild(const std::shared_ptr<IComponent> &pControl);
		std::list<std::shared_ptr<IComponent>> GetChildren() const { return m_children; }

		bool HasChild(const std::shared_ptr<IComponent> pControl) const {
			return pControl->GetUIParent().get() == this;
		}

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent);

	private:
		std::list<std::shared_ptr<IComponent>> m_children;
	};
}
}
