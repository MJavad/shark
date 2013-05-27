#include "Misc/stdafx.h"
#include "TabControl.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::shared_ptr<TabControl> TabControl::Create(float fWidth, float fHeight, uint32 dwColor) {
		const auto pTabControl = std::make_shared<TabControl>();
		pTabControl->SetWidth(fWidth);
		pTabControl->SetHeight(fHeight);
		pTabControl->SetColor(dwColor);
		return pTabControl;
	}

	void TabControl::OnRender(uint32 uTimePassed) {
		bool bValidActiveTab = IsValidTabIndex(m_activeTab);
		Utils::Vector2 vPosition = GetPosition();
		SetPosition(vPosition + Utils::Vector2(0.0f, 20.0f));
		SetChildOffset(Utils::Vector2(0.0f, 0.0f));
		Rectangle::OnRender(uTimePassed);
		float4 fDefault = {1.0f, 1.0f, 1.0f, 1.0f};

		GetInterface()->ClipStack.SetRect(GetFullRect(), [&] () {
			for (const auto& pTabPage : m_tabPages) {
				if (pTabPage->GetVisibility()) {
					float4 fCurrent = pTabPage->GetColorMod();
					pTabPage->SetColorMod(fDefault);
					pTabPage->RenderChildren(uTimePassed);
					pTabPage->SetColorMod(fCurrent);
				}
			}
		});

		SetPosition(vPosition);
		float fCurrentOffset = 0.0f;
		float fActiveTabOffset = 0.0f;
		for (const auto &pTabPage: m_tabPages)
			fCurrentOffset += (pTabPage->GetWidth() - 2.0f);

		for (auto itr = m_tabPages.crbegin(),
			end = m_tabPages.crend(); itr != end; ++itr) {
			const auto &pTabPage = *itr;
			if (!pTabPage->GetVisibility())
				continue;

			fCurrentOffset -= (pTabPage->GetWidth() - 2);
			if (!bValidActiveTab || pTabPage != m_tabPages[m_activeTab]) {
				SetChildOffset(Utils::Vector2(fCurrentOffset + 2, 0.0f));
				pTabPage->OnRender(uTimePassed);
			}
			else
				fActiveTabOffset = (fCurrentOffset + 2);
		}

		if (!bValidActiveTab ||
			!m_tabPages[m_activeTab]->GetVisibility())
			return;

		const auto &pActiveTab = m_tabPages[m_activeTab];
		float4 fPrevious = pActiveTab->GetColorMod();
		float4 fLight = fPrevious;
		fLight._2 *= 1.5f; fLight._3 *= 1.5f; fLight._4 *= 1.5f;

		pActiveTab->SetColorMod(fLight);
		SetChildOffset(Utils::Vector2(fActiveTabOffset, 0.0f));
		pActiveTab->OnRender(uTimePassed);
		pActiveTab->SetColorMod(fPrevious);
	}

	void TabControl::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc.LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!GetVisibility())
			sWndProc.LastMessageHandled = true;

		float fCurrentOffset = 0.0f;
		const auto vTabPages = m_tabPages;
		for (const auto &pTabPage: vTabPages) {
			SetChildOffset(Utils::Vector2(fCurrentOffset, 0.0f));
			pTabPage->OnMessageReceived(uMsg, wParam, lParam);
			fCurrentOffset += (pTabPage->GetWidth() + 2);
		}

		Utils::Vector2 vPosition = GetPosition();
		SetPosition(vPosition + Utils::Vector2(0.0f, 20.0f));
		SetChildOffset(Utils::Vector2(0.0f, 0.0f));

		if (IsValidTabIndex(m_activeTab)) {
			const auto pInterface = GetInterface();
			pInterface->ClipStack.push(GetFullRect());
			m_tabPages[m_activeTab]->SendMessageToChildren(uMsg, wParam, lParam);
			pInterface->ClipStack.pop();
		}

		SetPosition(vPosition);

		// If we are invisible, restore the original handled state
		if (!GetVisibility())
			sWndProc.LastMessageHandled = bHandled;
	}

	void TabControl::AddTabPage(std::shared_ptr<TabPage> pTabPage) {
		const auto pParent = std::dynamic_pointer_cast<TabControl>(pTabPage->GetUIParent());
		if (pParent != nullptr)
			pParent->RemoveTabPage(pTabPage);

		const auto pContent = pTabPage->GetContent();
		if (pContent != nullptr && !m_tabPages.empty()) {
			float4 fInvis = {0.0f, 1.0f, 1.0f, 1.0f};
			pContent->SetColorMod(fInvis);
			pContent->SetVisibility(false);
		}

		pTabPage->SetUIParent(shared_from_this());
		m_tabPages.push_back(std::move(pTabPage));
	}

	void TabControl::RemoveTabPage(const std::shared_ptr<TabPage> &pTabPage) {
		const auto pThis = shared_from_this();
		if (pTabPage->GetUIParent() != pThis)
			return;

		pTabPage->SetUIParent(nullptr);
		std::remove(m_tabPages.begin(), m_tabPages.end(), pTabPage);

		size_t numTabs = m_tabPages.size();
		if (m_activeTab >= numTabs)
			m_activeTab = (numTabs < 1 ? 0 : numTabs - 1);
	}

	void TabControl::SetActiveTab(uint32 uIndex) {
		if (m_activeTab == uIndex ||
			!IsValidTabIndex(uIndex))
			return;

		if (IsValidTabIndex(m_activeTab)) {
			const auto pContent = m_tabPages[m_activeTab]->GetContent();
			if (pContent != nullptr)
				pContent->Hide(150);
		}

		m_activeTab = uIndex;
		const auto pContent = m_tabPages[m_activeTab]->GetContent();
		if (pContent != nullptr)
			pContent->Show(150);
	}

	uint32 TabControl::GetPageIndex(const std::shared_ptr<const TabPage> &pTabPage) const {
		uint32 uIndex = 0;
		for (const auto &pCurrentPage: m_tabPages) {
			if (pCurrentPage == pTabPage)
				return uIndex;

			++uIndex;
		}

		return UINT_MAX;
	}

	bool TabControl::OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
		const auto pThis = shared_from_this();
		if (pComponent == pThis)
			return false;

		const auto pParent = GetUIParent();
		bool bNext = !_hasPage(pComponent);
		const auto vTabPages = m_tabPages;

		for (uint32 i = 0; i < 2; ++i) {
			for (auto itr = vTabPages.cbegin(),
					  end = vTabPages.cend(); itr != end; ++itr)
			{
				const auto &pCurrent = *itr;
				if (bNext || i > 0) {
					if (pCurrent == pComponent)
						return false;

					if (pCurrent->OnTabPressed(pThis))
						return true;
				}
				else if (pCurrent == pComponent)
					bNext = true;
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
