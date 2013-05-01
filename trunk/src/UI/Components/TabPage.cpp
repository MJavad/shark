#include "Misc/stdafx.h"
#include "TabPage.h"
#include "TabControl.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::shared_ptr<TabPage> TabPage::Create(std::wstring swText, float fWidth, float fHeight) {
		const auto pTabPage = std::make_shared<TabPage>();
		pTabPage->SetContent(std::make_shared<ItemsControl>());
		pTabPage->SetCaption(Label::Create(std::move(swText), DT_CENTER | DT_VCENTER, fWidth, fHeight));

		std::array<Utils::Vector2, 4> dimensions;
		dimensions[0].x = 5.0f;            dimensions[0].y = 0.0f;
		dimensions[1].x = fWidth - 5.0f;   dimensions[1].y = 0.0f;
		dimensions[2].x = fWidth;          dimensions[2].y = fHeight;
		dimensions[3].x = 0.0f;            dimensions[3].y = fHeight;
		pTabPage->SetDimensions(std::move(dimensions));

		float4 verticalRoundings = {0};
		verticalRoundings._1 = 4.0f;
		verticalRoundings._2 = 4.0f;

		float4 horizontalRoundings = {0};
		horizontalRoundings._1 = 4.0f;
		horizontalRoundings._2 = 4.0f;

		pTabPage->SetVerticalRoundings(verticalRoundings);
		pTabPage->SetHorizontalRoundings(horizontalRoundings);

		const auto pBorder = std::make_shared<Rectangle>();
		pBorder->SetColor(0xA02090CC);
		pBorder->SetVerticalRoundings(verticalRoundings);
		pBorder->SetHorizontalRoundings(horizontalRoundings);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		dimensions[0].x = 5.0f;            dimensions[0].y = 0.0f;
		dimensions[1].x = fWidth - 3.0f;   dimensions[1].y = 0.0f;
		dimensions[2].x = fWidth + 2.0f;   dimensions[2].y = fHeight;
		dimensions[3].x = 0.0f;            dimensions[3].y = fHeight;
		pBorder->SetDimensions(std::move(dimensions));
		pTabPage->SetBorder(std::move(pBorder));

		std::array<Utils::Color, 4> gradient;
		gradient[0] = 0xFF404040;
		gradient[1] = 0xFF404040;
		gradient[2] = 0xFF101010;
		gradient[3] = 0xFF101010;
		pTabPage->SetGradientColors(std::move(gradient));
		return pTabPage;
	}

	void TabPage::OnRender(uint32 uTimePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->OnRender(uTimePassed);

		Button::OnRender(uTimePassed);
	}

	void TabPage::RenderChildren(uint32 uTimePassed) {
		const auto pContent = GetContent();
		if (pContent != nullptr && pContent->GetVisibility())
			pContent->OnRender(uTimePassed);
	}

	void TabPage::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Button::OnMessageReceived(uMsg, wParam, lParam);
	}

	void TabPage::SendMessageToChildren(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc->LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!GetVisibility())
			sWndProc->LastMessageHandled = true;

		const auto pContent = GetContent();
		if (pContent != nullptr)
			pContent->OnMessageReceived(uMsg, wParam, lParam);

		// If we are invisible, restore the original handled state
		if (!GetVisibility())
			sWndProc->LastMessageHandled = bHandled;
	}

	bool TabPage::OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
		const auto pThis = shared_from_this();
		const auto pContent = GetContent();
		const auto pParent = std::dynamic_pointer_cast<TabControl>(GetUIParent());

		if (pParent != nullptr) {
			if (pContent != pComponent) {
				if (!IsFocused() && Focus())
					return true;

				if (pContent != nullptr) {
					const auto& vTabPages = pParent->GetTabPages();
					uint32 uActiveTab = pParent->GetActiveTab();

					if (vTabPages.size() > uActiveTab &&
						vTabPages[uActiveTab] == pThis)
						return pContent->OnTabPressed(pThis);
				}
			}

			return pParent->OnTabPressed(pThis);
		}

		return !IsFocused() && Focus();
	}

	void TabPage::OnPushRequest(const Utils::Vector2 &vPosition) {
		const auto pParent = std::dynamic_pointer_cast<TabControl>(GetUIParent());
		if (pParent != nullptr) {
			uint32 uIndex = pParent->GetPageIndex(get_this<TabPage>());
			pParent->SetActiveTab(uIndex);
		}

		Button::OnPushRequest(vPosition);
	}

	void TabPage::OnClickRequest(const Utils::Vector2 &vPosition) {
		const auto pParent = std::dynamic_pointer_cast<TabControl>(GetUIParent());
		if (pParent != nullptr) {
			uint32 uIndex = pParent->GetPageIndex(get_this<TabPage>());
			pParent->SetActiveTab(uIndex);
		}

		Button::OnClickRequest(vPosition);
	}
}
}
