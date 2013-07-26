#pragma once
#include "Base/ID3DInterface.h"
#include "ClipStack.h"
#include "Components/IComponent.h"

namespace UI {
namespace Components { class IComponent; }

	class ComponentInterface : public ID3DInterface
	{
	public:
		ComponentInterface() : Visible(true) {}

		static boost::shared_ptr<ComponentInterface> CreateDefault() {
			return Create();
		}

		static boost::shared_ptr<ComponentInterface> Create();

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool PushControl(boost::shared_ptr<Components::IComponent> pControl);
		void PopControl(const boost::shared_ptr<Components::IComponent> &pControl);

		bool Visible;
		ClipStack ClipStack;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		std::list<boost::shared_ptr<Components::IComponent>> m_components;

		bool _hasControl(const boost::shared_ptr<Components::IComponent> &pControl) const;
	};
}
