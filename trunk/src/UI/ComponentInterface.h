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

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool PushControl(std::shared_ptr<Components::IComponent> pControl);
		void PopControl(const std::shared_ptr<Components::IComponent> &pControl);

		bool Visible;
		ClipStack ClipStack;

	protected:
		std::list<std::shared_ptr<Components::IComponent>> m_components;

		bool _hasControl(const std::shared_ptr<Components::IComponent> &pControl) const;
	};
}
