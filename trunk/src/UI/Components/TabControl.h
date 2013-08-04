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
#include "TabPage.h"
#include "Rectangle.h"

namespace UI {
namespace Components {
	class TabControl : public virtual Rectangle
	{
	public:
		TabControl() : m_activeTab(0) {}

		static boost::shared_ptr<TabControl> Create(float width = 100.0f,
			float height = 100.0f, D3DXCOLOR dwColor = 0x90000000);

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void AddTabPage(boost::shared_ptr<TabPage> pTabPage);
		void RemoveTabPage(const boost::shared_ptr<TabPage> &pTabPage);

		void SetActiveTab(uint32 index);

		uint32 GetActiveTab() const {
			return m_activeTab;
		}

		std::vector<boost::shared_ptr<TabPage>> GetTabPages() const {
			return m_tabPages;
		}

		bool IsValidTabIndex(uint32 index) const {
			return m_tabPages.size() > index;
		}

		uint32 GetPageIndex(const boost::shared_ptr<const TabPage> &pTabPage) const;
		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent);

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		uint32 m_activeTab;
		std::vector<boost::shared_ptr<TabPage>> m_tabPages;

		bool _hasPage(const boost::shared_ptr<const IComponent> &pComponent) const {
			return std::find(m_tabPages.cbegin(), m_tabPages.cend(), pComponent) != m_tabPages.cend();
		}

		// wrapper for luabind ctor
		static boost::shared_ptr<TabControl> _createLua() {
			const auto pTabControl = Create();
			pTabControl->_registerAsScriptElement();
			return pTabControl;
		}
	};
}
}
