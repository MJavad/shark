#include "Misc/stdafx.h"
#include "ComponentInterface.h"
#include "Components/ItemsControl.h"
#include "Base/WndProc.h"

namespace UI {
	void ComponentInterface::OnRender(uint32 uTimePassed) {
		if (!Visible)
			return;

		// render from back to front...
		for (auto itr = m_components.crbegin(),
			end = m_components.crend(); itr != end; ++itr) {
			const auto &pComponent = *itr;
			if (pComponent->GetVisibility())
				pComponent->OnRender(uTimePassed);
		}
	}

	void ComponentInterface::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc.LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!Visible)
			sWndProc.LastMessageHandled = true;

		const auto lstChildren = m_components;
		for (const auto &pChild: lstChildren)
			pChild->OnMessageReceived(uMsg, wParam, lParam);

		// If we are invisible, restore the original handled state
		if (!Visible)
			sWndProc.LastMessageHandled = bHandled;
	}

	bool ComponentInterface::PushControl(std::shared_ptr<Components::IComponent> pControl) {
		// If it had a parent...
		const auto pParent = std::dynamic_pointer_cast<Components::ItemsControl>(pControl->GetUIParent());
		if (pParent != nullptr && !pParent->PopChild(pControl))
			return false;

		// If it had a previous interface...
		const auto pInterface = pControl->GetClientInterface();
		if (pInterface != nullptr)
			pInterface->m_components.remove(pControl);

		pControl->SetClientInterface(get_this<ComponentInterface>());
		m_components.push_front(std::move(pControl));
		return true;
	}

	void ComponentInterface::PopControl(const std::shared_ptr<Components::IComponent> &pControl) {
		if (!_hasControl(pControl))
			return;

		pControl->SetClientInterface(nullptr);
		m_components.remove(pControl);
	}

	bool ComponentInterface::_hasControl(const std::shared_ptr<Components::IComponent> &pControl) const {
		return pControl->GetClientInterface().get() == this;
	}
}
