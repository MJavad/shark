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
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IFocusable abstract : public virtual IComponent
	{
	public:
		virtual ~IFocusable() {
			if (IsFocused())
				s_activeFocus.reset();
		}

		virtual bool Focus() {
			if (IsFocused())
				return true;

			const auto activeFocus = GetActiveFocus();
			if (activeFocus != nullptr)
				activeFocus->_notifyFocusEndEvent();

			s_activeFocus = get_this<IFocusable>();
			return !_notifyFocusBeginEvent();
		}

		virtual void Unfocus() {
			if (!IsFocused())
				return;

			s_activeFocus.reset();
			_notifyFocusEndEvent();
		}

		void SetFocus(bool focus) {
			if (focus)
				Focus();
			else
				Unfocus();
		}

		bool GetFocus() const {
			return IsFocused();
		}

		static boost::shared_ptr<IFocusable> GetActiveFocus() {
			return s_activeFocus.lock();
		}

		bool IsFocused() const {
			return GetActiveFocus().get() == this;
		}

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
			if (!IsFocused() && Focus())
				return true;

			return IComponent::OnTabPressed(pComponent);
		}

		// lua wrapper
		Utils::Event<void ()> OnFocusBeginLuaWrap;
		Utils::Event<void ()> OnFocusEndLuaWrap;

		Utils::Event<bool (const boost::shared_ptr<IFocusable>&)> OnFocusBeginEvent;
		Utils::Event<void (const boost::shared_ptr<IFocusable>&)> OnFocusEndEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IFocusable() {}

		virtual bool _notifyFocusBeginEvent() {
			OnFocusBeginLuaWrap();
			return OnFocusBeginEvent(get_this<IFocusable>());
		}

		virtual void _notifyFocusEndEvent() {
			OnFocusEndLuaWrap();
			OnFocusEndEvent(get_this<IFocusable>());
		}

	private:
		static boost::weak_ptr<IFocusable> s_activeFocus;
	};
}
}
