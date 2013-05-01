#pragma once
#include "TabPage.h"
#include "Rectangle.h"

namespace UI {
namespace Components {
	class TabControl : public Rectangle
	{
	public:
		TabControl() : m_activeTab(0) {}
		static std::shared_ptr<TabControl> Create(float fWidth, float fHeight, uint32 dwColor = 0x90000000);

		virtual void OnRender(uint32 uTimePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void AddTabPage(std::shared_ptr<TabPage> pTabPage);
		void RemoveTabPage(const std::shared_ptr<TabPage> &pTabPage);

		void SetActiveTab(uint32 uIndex);

		uint32 GetActiveTab() const {
			return m_activeTab;
		}

		std::vector<std::shared_ptr<TabPage>> GetTabPages() const {
			return m_tabPages;
		}

		bool IsValidTabIndex(uint32 uIndex) const {
			return m_tabPages.size() > uIndex;
		}

		uint32 GetPageIndex(const std::shared_ptr<const TabPage> &pTabPage) const;
		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent);

	protected:
		uint32 m_activeTab;
		std::vector<std::shared_ptr<TabPage>> m_tabPages;

		bool _hasPage(const std::shared_ptr<const IComponent> &pComponent) const {
			return std::find(m_tabPages.cbegin(), m_tabPages.cend(), pComponent) != m_tabPages.cend();
		}
	};
}
}
