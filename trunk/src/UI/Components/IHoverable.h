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
#include "Misc/LuaEvent.h"

namespace UI {
namespace Components {
	class IHoverable abstract : public virtual IRectComponent
	{
	public:
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		bool IsHovered() const {
			return m_isHovered;
		}

		void StartHover() {
			if (!m_isHovered) {
				m_isHovered = true;
				FadeTo(75, m_hoverColor);
			}
		}

		void ClearHover() {
			if (m_isHovered) {
				m_isHovered = false;
				FadeTo(400, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

		void SetHovered(bool hovered) {
			if (hovered)
				StartHover();
			else
				ClearHover();
		}

		bool GetHovered() const {
			return IsHovered();
		}
		
		D3DXCOLOR GetHoverColor() const {
			return m_hoverColor;
		}

		void SetHoverColor(const D3DXCOLOR &hoverColor) {
			m_hoverColor = hoverColor;
		}

		// lua wrapper
		Utils::LuaEvent<void ()> OnHoverBeginLuaWrap;
		Utils::LuaEvent<void ()> OnHoverEndLuaWrap;

		// returning true = no hover
		Utils::Event<bool (const boost::shared_ptr<IHoverable>&)> OnHoverBeginEvent;
		Utils::Event<bool (const boost::shared_ptr<IHoverable>&)> OnHoverEndEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IHoverable() :
			m_isHovered(false),
			m_hoverColor(1.5f, 1.5f, 1.5f, 1.4f) {}

		virtual bool _notifyHoverBeginEvent() {
			OnHoverBeginLuaWrap();
			return OnHoverBeginEvent(getThis<IHoverable>());
		}

		virtual bool _notifyHoverEndEvent() {
			OnHoverEndLuaWrap();
			return OnHoverEndEvent(getThis<IHoverable>());
		}

	private:
		bool m_isHovered;
		D3DXCOLOR m_hoverColor;
	};
}
}
