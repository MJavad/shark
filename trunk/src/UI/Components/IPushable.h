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
#include "IRectComponent.h"
#include "Misc/LuaEvent.h"

namespace UI {
namespace Components {
	class IPushable abstract : public virtual IRectComponent
	{
	public:
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		static boost::shared_ptr<IPushable> GetActiveClick() {
			return s_activeClick.lock();
		}

		bool IsPressed() const {
			return GetActiveClick().get() == this;
		}

		// lua wrapper
		Utils::LuaEvent<void ()> OnClickLuaWrap;
		Utils::LuaEvent<void ()> OnDblClickLuaWrap;
		Utils::LuaEvent<void ()> OnPushLuaWrap;
		Utils::LuaEvent<void ()> OnReleaseLuaWrap;

		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnClickEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnDblClickEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnPushEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnReleaseEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IPushable() : m_lastClick(0) {}

		virtual void _notifyClickEvent(Utils::Vector2 *pPosition) {
			OnClickLuaWrap();
			OnClickEvent(getThis<IPushable>(), pPosition);
		}

		virtual void _notifyDblClickEvent(Utils::Vector2 *pPosition) {
			OnDblClickLuaWrap();
			OnDblClickEvent(getThis<IPushable>(), pPosition);
		}

		virtual void _notifyPushEvent(Utils::Vector2 *pPosition) {
			OnPushLuaWrap();
			OnPushEvent(getThis<IPushable>(), pPosition);
		}

		virtual void _notifyReleaseEvent(Utils::Vector2 *pPosition) {
			OnReleaseLuaWrap();
			OnReleaseEvent(getThis<IPushable>(), pPosition);
		}

	private:
		uint32 m_lastClick;
		static boost::weak_ptr<IPushable> s_activeClick;
	};
}
}
