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
#include "IComponent.h"

namespace UI {
namespace Components {
	class ItemsControl : public virtual IComponent
	{
	public:
		ItemsControl() {}

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

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		std::list<boost::shared_ptr<IComponent>> m_children;

		// wrapper for luabind ctor
		static boost::shared_ptr<ItemsControl> _createLua() {
			return Create();
		}
	};
}
}
