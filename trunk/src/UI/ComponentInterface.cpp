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
#include "ComponentInterface.h"
#include "Components/ItemsControl.h"
#include "Base/WndProc.h"

namespace UI {
	boost::shared_ptr<ComponentInterface> ComponentInterface::Create() {
		return boost::make_shared<ComponentInterface>();
	}

	void ComponentInterface::OnRender(uint32 timePassed) {
		if (!Visible)
			return;

		// render from back to front...
		for (auto itr = m_components.crbegin(),
			end = m_components.crend(); itr != end; ++itr) {
			const auto &pComponent = *itr;
			if (pComponent->GetVisibility())
				pComponent->OnRender(timePassed);
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

	bool ComponentInterface::PushControl(boost::shared_ptr<Components::IComponent> pControl) {
		// If it had a parent...
		const auto pParent = boost::dynamic_pointer_cast<Components::ItemsControl>(pControl->GetUIParent());
		if (pParent != nullptr && !pParent->PopChild(pControl))
			return false;

		// If it had a previous interface...
		const auto pInterface = pControl->GetLocalInterface();
		if (pInterface != nullptr)
			pInterface->m_components.remove(pControl);

		pControl->SetLocalInterface(getThis<ComponentInterface>());
		m_components.push_front(std::move(pControl));
		return true;
	}

	void ComponentInterface::PopControl(const boost::shared_ptr<Components::IComponent> &pControl) {
		if (!_hasControl(pControl))
			return;

		pControl->SetLocalInterface(nullptr);
		m_components.remove(pControl);
	}

	bool ComponentInterface::_hasControl(const boost::shared_ptr<Components::IComponent> &pControl) const {
		return pControl->GetLocalInterface().get() == this;
	}

	void ComponentInterface::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<ComponentInterface, ID3DInterface,
							boost::shared_ptr<ID3DInterface>>("ComponentInterface")
				.scope [ luabind::def("Create", &ComponentInterface::_createLua) ]
				.def_readwrite("visible", &ComponentInterface::Visible)
				.def_readonly("clipStack", &ComponentInterface::ClipStack)
				.def("PushControl", &ComponentInterface::PushControl)
				.def("PopControl", &ComponentInterface::PopControl)
		];
	}
}
