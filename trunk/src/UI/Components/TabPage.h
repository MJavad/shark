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
#include "ItemsControl.h"
#include "Button.h"

namespace UI {
namespace Components {
	class TabPage : public Button
	{
	public:
		TabPage() {}

		static boost::shared_ptr<TabPage> CreateDefault() {
			return Create();
		}

		static boost::shared_ptr<TabPage> Create(
			std::wstring textString = L"Default Page",
			float width = 100.0f,
			float height = 20.0f);

		virtual void OnRender(uint32 timePassed);
		void RenderChildren(uint32 timePassed);

		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void SendMessageToChildren(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent);

		boost::shared_ptr<ItemsControl> GetContent() const {
			return m_content;
		}

		void SetContent(boost::shared_ptr<ItemsControl> pContent) {
			m_content = std::move(pContent);
			if (m_content != nullptr)
				m_content->SetUIParent(shared_from_this());
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		virtual void _notifyPushEvent(Utils::Vector2 *pPosition);
		virtual void _notifyClickEvent(Utils::Vector2 *pPosition);

		virtual bool _notifyFocusBeginEvent() {
			FadeTo(200, GetHoverColor());
			return IFocusable::_notifyFocusBeginEvent();
		}

		virtual void _notifyFocusEndEvent() {
			FadeTo(200, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return IFocusable::_notifyFocusEndEvent();
		}

	private:
		boost::shared_ptr<ItemsControl> m_content;
	};
}
}
