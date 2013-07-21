#pragma once
#include "IComponent.h"

namespace UI {
namespace Components {
	class ItemsControl : public virtual IComponent
	{
	public:
		static boost::shared_ptr<ItemsControl> Create();

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool PushChild(boost::shared_ptr<IComponent> pControl);
		bool PopChild(const boost::shared_ptr<IComponent> &pControl);
		std::list<boost::shared_ptr<IComponent>> GetChildren() const { return m_children; }

		bool HasChild(const boost::shared_ptr<IComponent> pControl) const {
			return pControl->GetUIParent().get() == this;
		}

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent);

	private:
		std::list<boost::shared_ptr<IComponent>> m_children;
	};
}
}
