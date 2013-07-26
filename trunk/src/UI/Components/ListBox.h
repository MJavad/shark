#pragma once
#include "Rectangle.h"
#include "IHoverable.h"
#include "ListBoxEntry.h"

namespace UI {
namespace Components {
	class ListBox : public Rectangle
	{
	public:
		ListBox() : m_isSelecting(false), m_isMultiSelect(false) {}

		static boost::shared_ptr<ListBox> CreateDefault() {
			return Create();
		}

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
			pItem->SetUIParent(shared_from_this());
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
				m_border->SetUIParent(shared_from_this());
			}
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		bool m_isSelecting, m_isMultiSelect;
		boost::shared_ptr<Rectangle> m_border;
		std::list<boost::shared_ptr<ListBoxEntry>> m_entries;
	};
}
}
