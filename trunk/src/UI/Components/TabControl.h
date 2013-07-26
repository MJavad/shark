#pragma once
#include "TabPage.h"
#include "Rectangle.h"

namespace UI {
namespace Components {
	class TabControl : public Rectangle
	{
	public:
		TabControl() : m_activeTab(0) {}

		static boost::shared_ptr<TabControl> CreateDefault() {
			return Create();
		}

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
	};
}
}
