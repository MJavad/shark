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
		Utils::Event<void ()> OnClickLuaWrap;
		Utils::Event<void ()> OnDblClickLuaWrap;
		Utils::Event<void ()> OnPushLuaWrap;
		Utils::Event<void ()> OnReleaseLuaWrap;

		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnClickEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnDblClickEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnPushEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnReleaseEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IPushable() : m_lastClick(0) {}

		virtual void _notifyClickEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: Click triggered.", this);
			OnClickLuaWrap();
			OnClickEvent(get_this<IPushable>(), pPosition);
		}

		virtual void _notifyDblClickEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: DblClick triggered.", this);
			OnDblClickLuaWrap();
			OnDblClickEvent(get_this<IPushable>(), pPosition);
		}

		virtual void _notifyPushEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: Push triggered.", this);
			OnPushLuaWrap();
			OnPushEvent(get_this<IPushable>(), pPosition);
		}

		virtual void _notifyReleaseEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: Release triggered.", this);
			OnReleaseLuaWrap();
			OnReleaseEvent(get_this<IPushable>(), pPosition);
		}

	private:
		uint32 m_lastClick;
		static boost::weak_ptr<IPushable> s_activeClick;
	};
}
}
