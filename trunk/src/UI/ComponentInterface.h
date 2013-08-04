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

		// wrapper for luabind ctor
		static boost::shared_ptr<ComponentInterface> _createLua() {
			const auto pInterface = Create();
			pInterface->RegisterAsScriptElement();
			return pInterface;
		}
	};
}
