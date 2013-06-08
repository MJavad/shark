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

		static std::shared_ptr<ListBox> Create(float fWidth = 200.0f,
			float fHeight = 160.0f);

		virtual void OnRender(uint32 uTimePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		virtual void SetWidth(float fWidth);
		virtual void SetHeight(float fHeight);

		virtual void SelectItem(const std::shared_ptr<ListBoxEntry> &pItem);
		virtual void DeselectItem(const std::shared_ptr<ListBoxEntry> &pItem);
		virtual void DeselectAll();
		
		virtual void AddItem(std::shared_ptr<ListBoxEntry> pItem) {
			pItem->SetWidth(GetWidth());
			pItem->SetUIParent(shared_from_this());
			m_entries.push_back(std::move(pItem));
		}

		virtual void RemoveItem(const std::shared_ptr<ListBoxEntry> &pItem) {
			pItem->SetUIParent(nullptr);
			m_entries.remove(pItem);
		}

		virtual void RemoveAll() {
			for (const auto &pListBoxEntry: m_entries)
				pListBoxEntry->SetUIParent(nullptr);
			m_entries.clear();
		}
		
		virtual bool IsMultiSelect() const {
			return m_isMultiSelect;
		}

		virtual void SetMultiSelect(bool bEnabled) {
			m_isMultiSelect = bEnabled;
		}
		
		virtual bool IsSelecting() const {
			return m_isSelecting;
		}

		virtual void SetSelecting(bool bSelecting) {
			m_isSelecting = bSelecting;
		}

		virtual std::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(std::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr) {
				m_border->SetPosition(Utils::Vector2(-1.0f, -1.0f));
				m_border->SetUIParent(shared_from_this());
			}
		}

	private:
		bool m_isSelecting, m_isMultiSelect;
		std::shared_ptr<Rectangle> m_border;
		std::list<std::shared_ptr<ListBoxEntry>> m_entries;
	};
}
}