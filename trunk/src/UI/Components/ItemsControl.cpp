#include "Misc/stdafx.h"
#include "ItemsControl.h"
#include "Base/WndProc.h"
#include "IFocusable.h"

namespace UI {
namespace Components {
	bool ItemsControl::PushChild(std::shared_ptr<IComponent> pControl) {
		if (HasChild(pControl)) {
			m_children.remove(pControl);
			m_children.push_front(std::move(pControl));
			return true;
		}

		const auto pThis = shared_from_this();
		for (auto pParent = pControl; pParent != nullptr; pParent = pParent->GetUIParent()) {
			if (pParent == pThis)
				return false;
		}

		for (auto pParent = pThis; pParent != nullptr; pParent = pParent->GetUIParent()) {
			if (pParent == pControl)
				return false;
		}

		if (!pControl->OnParentChanged(pThis))
			return false;

		const auto pClientInterface = pControl->GetClientInterface();
		if (pClientInterface != nullptr)
			pClientInterface->PopControl(pControl);

		const auto pItemParent = std::dynamic_pointer_cast<ItemsControl>(pControl->GetUIParent());
		if (pItemParent != nullptr)
			pItemParent->m_children.remove(pControl);

		pControl->SetUIParent(pThis);
		m_children.push_front(std::move(pControl));
		return true;
	}

	bool ItemsControl::PopChild(const std::shared_ptr<IComponent> &pControl) {
		if (!HasChild(pControl) || !pControl->OnParentChanged(nullptr))
			return false;

		pControl->SetUIParent(nullptr);
		m_children.remove(pControl);
		return true;
	}

	void ItemsControl::OnRender(uint32 uTimePassed) {
		// render from back to front
		for (auto itr = m_children.crbegin(),
			end = m_children.crend(); itr != end; ++itr) {
			const auto &pComponent = *itr;
			if (pComponent->GetVisibility())
				pComponent->OnRender(uTimePassed);
		}
	}

	void ItemsControl::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc->LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!GetVisibility())
			sWndProc->LastMessageHandled = true;

		const auto lstChildren = m_children;
		for (const auto &pChild: lstChildren)
			pChild->OnMessageReceived(uMsg, wParam, lParam);

		// If we are invisible, restore the original handled state
		if (!GetVisibility())
			sWndProc->LastMessageHandled = bHandled;
	}

	bool ItemsControl::OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
		const auto pThis = shared_from_this();
		if (pComponent == pThis)
			return false;

		const auto pParent = GetUIParent();
		bool bNext = !HasChild(pComponent);
		const auto lstChildren = m_children;

		for (uint32 i = 0; i < 2; ++i) {
			for (auto itr = lstChildren.crbegin(),
					  end = lstChildren.crend(); itr != end; ++itr)
			{
				const auto &pCurrent = *itr;
				if (bNext || i > 0) {
					if (pCurrent == pComponent)
						return false;

					if (pCurrent->OnTabPressed(pThis))
						return true;
				}
				else if (pCurrent == pComponent)
					bNext = true;
			}

			// only call it after the first loop
			if (i == 0 && pParent != nullptr &&
				pParent->OnTabPressed(pThis))
				return true;
		}

		return false;
	}
}
}
