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

#include "Misc/stdafx.h"
#include "TabPage.h"
#include "TabControl.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::shared_ptr<TabPage> TabPage::Create(std::wstring textString, float width, float height) {
		const auto pTabPage = boost::make_shared<TabPage>();
		pTabPage->SetContent(boost::make_shared<ItemsControl>());
		pTabPage->SetCaption(Label::Create(std::move(textString), DT_CENTER | DT_VCENTER, width, height));

		std::array<Utils::Vector2, 4> dimensions;
		dimensions[0].x = 5.0f;            dimensions[0].y = 0.0f;
		dimensions[1].x = width - 5.0f;   dimensions[1].y = 0.0f;
		dimensions[2].x = width;          dimensions[2].y = height;
		dimensions[3].x = 0.0f;            dimensions[3].y = height;
		pTabPage->SetDimensions(std::move(dimensions));

		float4 verticalRoundings = {0};
		verticalRoundings._1 = 4.0f;
		verticalRoundings._2 = 4.0f;

		float4 horizontalRoundings = {0};
		horizontalRoundings._1 = 4.0f;
		horizontalRoundings._2 = 4.0f;

		pTabPage->SetVerticalRoundings(verticalRoundings);
		pTabPage->SetHorizontalRoundings(horizontalRoundings);

		const auto pBorder = boost::make_shared<Rectangle>();
		pBorder->SetColor(0xA02090CC);
		pBorder->SetVerticalRoundings(verticalRoundings);
		pBorder->SetHorizontalRoundings(horizontalRoundings);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		dimensions[0].x = 5.0f;            dimensions[0].y = 0.0f;
		dimensions[1].x = width - 3.0f;   dimensions[1].y = 0.0f;
		dimensions[2].x = width + 2.0f;   dimensions[2].y = height;
		dimensions[3].x = 0.0f;            dimensions[3].y = height;
		pBorder->SetDimensions(std::move(dimensions));
		pTabPage->SetBorder(std::move(pBorder));

		std::array<D3DXCOLOR, 4> gradient;
		gradient[0] = 0xFF404040;
		gradient[1] = 0xFF404040;
		gradient[2] = 0xFF101010;
		gradient[3] = 0xFF101010;
		pTabPage->SetGradientColors(std::move(gradient));
		return pTabPage;
	}

	void TabPage::OnRender(uint32 timePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->OnRender(timePassed);

		Button::OnRender(timePassed);
	}

	void TabPage::RenderChildren(uint32 timePassed) {
		const auto pContent = GetContent();
		if (pContent != nullptr && pContent->GetVisibility())
			pContent->OnRender(timePassed);
	}

	void TabPage::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Button::OnMessageReceived(uMsg, wParam, lParam);
	}

	void TabPage::SendMessageToChildren(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Save old handled state
		bool bHandled = sWndProc.LastMessageHandled;

		// If we are invisible, pretend that the message was already handled
		if (!GetVisibility())
			sWndProc.LastMessageHandled = true;

		const auto pContent = GetContent();
		if (pContent != nullptr)
			pContent->OnMessageReceived(uMsg, wParam, lParam);

		// If we are invisible, restore the original handled state
		if (!GetVisibility())
			sWndProc.LastMessageHandled = bHandled;
	}

	bool TabPage::OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
		const auto pThis = shared_from_this();
		const auto pContent = GetContent();
		const auto pParent = boost::dynamic_pointer_cast<TabControl>(GetUIParent());

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

	void TabPage::_notifyPushEvent(Utils::Vector2 *pPosition) {
		const auto pParent = boost::dynamic_pointer_cast<TabControl>(GetUIParent());
		if (pParent != nullptr) {
			uint32 index = pParent->GetPageIndex(get_this<TabPage>());
			pParent->SetActiveTab(index);
		}

		Button::_notifyPushEvent(pPosition);
	}

	void TabPage::_notifyClickEvent(Utils::Vector2 *pPosition) {
		const auto pParent = boost::dynamic_pointer_cast<TabControl>(GetUIParent());
		if (pParent != nullptr) {
			uint32 index = pParent->GetPageIndex(get_this<TabPage>());
			pParent->SetActiveTab(index);
		}

		Button::_notifyClickEvent(pPosition);
	}

	void TabPage::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<TabPage, Button,
							boost::shared_ptr<IComponent>>("TabPage")
				.scope [ luabind::def("Create", &TabPage::CreateDefault) ]
				.property("content", &TabPage::GetContent, &TabPage::SetContent)
		];
	}
}
}
