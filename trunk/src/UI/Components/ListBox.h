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
#include "Rectangle.h"
#include "IHoverable.h"
#include "ListBoxEntry.h"

namespace UI {
namespace Components {
	class ListBox : public virtual Rectangle
	{
	public:
		ListBox() : m_isSelecting(false), m_isMultiSelect(false) {}

		static boost::shared_ptr<ListBox> Create(float width = 200.0f, float height = 160.0f);

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		virtual void SetWidth(float width);
		virtual void SetHeight(float height);

		virtual void SelectItem(const boost::shared_ptr<ListBoxEntry> &pItem);
		virtual void DeselectItem(const boost::shared_ptr<ListBoxEntry> &pItem);
		virtual void DeselectAll();
		
		virtual void AddItem(boost::shared_ptr<ListBoxEntry> pItem) {
			pItem->SetWidth(GetWidth());
			pItem->SetUIParent(getThis<IComponent>());
			m_entries.push_back(std::move(pItem));
		}

		virtual void RemoveItem(const boost::shared_ptr<ListBoxEntry> &pItem) {
			pItem->SetUIParent(nullptr);
			m_entries.remove(pItem);
		}

		virtual void RemoveAll() {
			for (const auto &pListBoxEntry: m_entries)
				pListBoxEntry->SetUIParent(nullptr);
			m_entries.clear();
		}
		
		virtual bool GetMultiSelect() const {
			return m_isMultiSelect;
		}

		virtual void SetMultiSelect(bool enabled) {
			m_isMultiSelect = enabled;
		}
		
		virtual bool IsSelecting() const {
			return m_isSelecting;
		}

		virtual void SetSelecting(bool selecting) {
			m_isSelecting = selecting;
		}

		virtual boost::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(boost::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr) {
				m_border->SetPosition(Utils::Vector2(-1.0f, -1.0f));
				m_border->SetUIParent(getThis<IComponent>());
			}
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		bool m_isSelecting, m_isMultiSelect;
		boost::shared_ptr<Rectangle> m_border;
		std::list<boost::shared_ptr<ListBoxEntry>> m_entries;

		// wrapper for luabind ctor
		static boost::shared_ptr<ListBox> _createLua() {
			const auto pListBox = Create();
			pListBox->_registerAsScriptElement();
			return pListBox;
		}
	};
}
}
