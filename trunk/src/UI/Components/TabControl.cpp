#include "Misc/stdafx.h"
#include "TabControl.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::shared_ptr<TabControl> TabControl::Create(float width,
												   float height,
												   D3DXCOLOR dwColor)
	{
		const auto pTabControl = boost::make_shared<TabControl>();
		pTabControl->SetWidth(width);
		pTabControl->SetHeight(height);
		pTabControl->SetColor(dwColor);
		return pTabControl;
	}

	void TabControl::OnRender(uint32 timePassed) {
		bool validActiveTab = IsValidTabIndex(m_activeTab);
		Utils::Vector2 position = GetPosition();
		SetPosition(position + Utils::Vector2(0.0f, 20.0f));
		SetChildOffset(Utils::Vector2(0.0f, 0.0f));
		Rectangle::OnRender(timePassed);

		const auto pInterface = GetInterface();
		pInterface->ClipStack.Push(GetFullRect());
		pInterface->ClipStack.Apply();

		for (const auto& pTabPage : m_tabPages) {
			if (pTabPage->GetVisibility()) {
				D3DXCOLOR previousColor = pTabPage->GetColorMod();
				pTabPage->SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				pTabPage->RenderChildren(timePassed);
				pTabPage->SetColorMod(previousColor);
			}
		}

		pInterface->ClipStack.Pop();
		pInterface->ClipStack.Apply();

		SetPosition(position);
		float renderOffset = 0.0f;
		float activeTabOffset = 0.0f;
		for (const auto &pTabPage: m_tabPages)
			renderOffset += (pTabPage->GetWidth() - 2.0f);

		for (auto itr = m_tabPages.crbegin(),
			end = m_tabPages.crend(); itr != end; ++itr) {
			const auto &pTabPage = *itr;
			if (!pTabPage->GetVisibility())
				continue;

			renderOffset -= (pTabPage->GetWidth() - 2);
			if (!validActiveTab || pTabPage != m_tabPages[m_activeTab]) {
				SetChildOffset(Utils::Vector2(renderOffset + 2.0f, 0.0f));
				pTabPage->OnRender(timePassed);
			}
			else
				activeTabOffset = (renderOffset + 2);
		}

		if (!validActiveTab ||
			!m_tabPages[m_activeTab]->GetVisibility())
			return;

		const auto &pActiveTab = m_tabPages[m_activeTab];
		D3DXCOLOR previousColor = pActiveTab->GetColorMod();
		D3DXCOLOR highlightColor = previousColor * 1.5f;

		pActiveTab->SetColorMod(highlightColor);
		SetChildOffset(Utils::Vector2(activeTabOffset, 0.0f));
		pActiveTab->OnRender(timePassed);
		pActiveTab->SetColorMod(previousColor);
	}

	void TabControl::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc.LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!GetVisibility())
			sWndProc.LastMessageHandled = true;

		float renderOffset = 0.0f;
		const auto vTabPages = m_tabPages;
		for (const auto &pTabPage: vTabPages) {
			SetChildOffset(Utils::Vector2(renderOffset, 0.0f));
			renderOffset += (pTabPage->GetWidth() + 2.0f);

			pTabPage->OnMessageReceived(uMsg, wParam, lParam);
		}

		Utils::Vector2 position = GetPosition();
		SetPosition(position + Utils::Vector2(0.0f, 20.0f));
		SetChildOffset(Utils::Vector2(0.0f, 0.0f));

		if (IsValidTabIndex(m_activeTab)) {
			const auto pInterface = GetInterface();
			pInterface->ClipStack.Push(GetFullRect());
			m_tabPages[m_activeTab]->SendMessageToChildren(uMsg, wParam, lParam);
			pInterface->ClipStack.Pop();
		}

		SetPosition(position);

		// If we are invisible, restore the original handled state
		if (!GetVisibility())
			sWndProc.LastMessageHandled = bHandled;
	}

	void TabControl::AddTabPage(boost::shared_ptr<TabPage> pTabPage) {
		const auto pParent = boost::dynamic_pointer_cast<TabControl>(pTabPage->GetUIParent());
		if (pParent != nullptr)
			pParent->RemoveTabPage(pTabPage);

		const auto pContent = pTabPage->GetContent();
		if (pContent != nullptr && !m_tabPages.empty()) {
			pContent->SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			pContent->SetVisibility(false);
		}

		pTabPage->SetUIParent(shared_from_this());
		m_tabPages.push_back(std::move(pTabPage));
	}

	void TabControl::RemoveTabPage(const boost::shared_ptr<TabPage> &pTabPage) {
		const auto pThis = shared_from_this();
		if (pTabPage->GetUIParent() != pThis)
			return;

		pTabPage->SetUIParent(nullptr);
		std::remove(m_tabPages.begin(), m_tabPages.end(), pTabPage);

		size_t numTabs = m_tabPages.size();
		if (m_activeTab >= numTabs)
			m_activeTab = (numTabs < 1 ? 0 : numTabs - 1);
	}

	void TabControl::SetActiveTab(uint32 index) {
		if (m_activeTab == index ||
			!IsValidTabIndex(index))
			return;

		if (IsValidTabIndex(m_activeTab)) {
			const auto pContent = m_tabPages[m_activeTab]->GetContent();
			if (pContent != nullptr)
				pContent->Hide(150);
		}

		m_activeTab = index;
		const auto pContent = m_tabPages[m_activeTab]->GetContent();
		if (pContent != nullptr)
			pContent->Show(150);
	}

	uint32 TabControl::GetPageIndex(const boost::shared_ptr<const TabPage> &pTabPage) const {
		uint32 index = 0;
		for (const auto &pCurrentPage: m_tabPages) {
			if (pCurrentPage == pTabPage)
				return index;

			++index;
		}

		return UINT_MAX;
	}

	bool TabControl::OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
		const auto pThis = shared_from_this();
		if (pComponent == pThis)
			return false;

		const auto pParent = GetUIParent();
		bool nextItem = !_hasPage(pComponent);
		const auto vTabPages = m_tabPages;

		for (uint32 i = 0; i < 2; ++i) {
			for (auto itr = vTabPages.cbegin(),
					  end = vTabPages.cend(); itr != end; ++itr)
			{
				const auto &pCurrent = *itr;
				if (nextItem || i > 0) {
					if (pCurrent == pComponent)
						return false;

					if (pCurrent->OnTabPressed(pThis))
						return true;
				}
				else if (pCurrent == pComponent)
					nextItem = true;
			}

			// only call it after the first loop
			if (i == 0 && pParent != nullptr &&
				pParent->OnTabPressed(pThis))
				return true;
		}

		return false;
	}
}
}
