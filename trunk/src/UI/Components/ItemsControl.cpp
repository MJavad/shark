/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Misc/stdafx.h"
#include "ItemsControl.h"
#include "Base/WndProc.h"
#include "IFocusable.h"

namespace UI {
namespace Components {
	boost::shared_ptr<ItemsControl> ItemsControl::Create() {
		return boost::make_shared<ItemsControl>();
	}

	bool ItemsControl::PushChild(boost::shared_ptr<IComponent> pControl) {
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

		const auto pClientInterface = pControl->GetLocalInterface();
		if (pClientInterface != nullptr)
			pClientInterface->PopControl(pControl);

		const auto pItemParent = boost::dynamic_pointer_cast<ItemsControl>(pControl->GetUIParent());
		if (pItemParent != nullptr)
			pItemParent->m_children.remove(pControl);

		pControl->SetUIParent(pThis);
		m_children.push_front(std::move(pControl));
		return true;
	}

	bool ItemsControl::PopChild(const boost::shared_ptr<IComponent> &pControl) {
		if (!HasChild(pControl) || !pControl->OnParentChanged(nullptr))
			return false;

		pControl->SetUIParent(nullptr);
		m_children.remove(pControl);
		return true;
	}

	void ItemsControl::OnRender(uint32 timePassed) {
		// render from back to front
		for (auto itr = m_children.crbegin(),
			end = m_children.crend(); itr != end; ++itr) {
			const auto &pComponent = *itr;
			if (pComponent->GetVisibility())
				pComponent->OnRender(timePassed);
		}
	}

	void ItemsControl::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc.LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!GetVisibility())
			sWndProc.LastMessageHandled = true;

		const auto lstChildren = m_children;
		for (const auto &pChild: lstChildren)
			pChild->OnMessageReceived(uMsg, wParam, lParam);

		// If we are invisible, restore the original handled state
		if (!GetVisibility())
			sWndProc.LastMessageHandled = bHandled;
	}

	bool ItemsControl::OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
		const auto pThis = shared_from_this();
		if (pComponent == pThis)
			return false;

		const auto pParent = GetUIParent();
		bool nextItem = !HasChild(pComponent);
		const auto lstChildren = m_children;

		for (uint32 i = 0; i < 2; ++i) {
			for (auto itr = lstChildren.crbegin(),
					  end = lstChildren.crend(); itr != end; ++itr)
			{
				const auto &pCurrent = *itr;
				if (nextItem || i > 0) {
					if (pCurrent == pComponent)
						return false;

					if (pCurrent->OnTabPressed(pThis))
						return true;
				}
				else if (pCurrent == pComponent)
					nextItem = true;
			}

			// only call it after the first loop
			if (i == 0 && pParent != nullptr &&
				pParent->OnTabPressed(pThis))
				return true;
		}

		return false;
	}

	void ItemsControl::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<ItemsControl, IComponent,
							boost::shared_ptr<IComponent>>("ItemsControl")
				.scope [ luabind::def("Create", &ItemsControl::CreateDefault) ]
				.def("PushChild", &ItemsControl::PushChild)
				.def("PopChild", &ItemsControl::PopChild)
				.def("HasChild", &ItemsControl::HasChild)
		];
	}
}
}
