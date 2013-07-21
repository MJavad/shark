#include "Misc/stdafx.h"
#include "ListBox.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::shared_ptr<ListBox> ListBox::Create(float width, float height) {
		const auto pListBox = boost::make_shared<ListBox>();
		const auto pBorder = Rectangle::Create(width, height);
		
		pBorder->SetColor(0x90404040);
		pListBox->SetBorder(std::move(pBorder));

		pListBox->SetWidth(width);
		pListBox->SetHeight(height);
		pListBox->SetColor(0x90101010);
		return pListBox;
	}

	void ListBox::OnRender(uint32 timePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(timePassed);

		Rectangle::OnRender(timePassed);

		float renderOffset = 0.0f;
		for (const auto &pListBoxEntry: m_entries) {
			SetChildOffset(Utils::Vector2(0.0f, renderOffset));
			renderOffset += pListBoxEntry->GetHeight() + 1.0f;

			if (pListBoxEntry->GetVisibility())
				pListBoxEntry->OnRender(timePassed);
		}
		
		const auto pSprite = sD3DMgr.GetSprite();
		if (pSprite != nullptr) {
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);

			renderOffset = 0.0f;
			for (const auto &pListBoxEntry: m_entries) {
				SetChildOffset(Utils::Vector2(0.0f, renderOffset));
				renderOffset += pListBoxEntry->GetHeight() + 1.0f;

				if (pListBoxEntry->GetVisibility())
					pListBoxEntry->RenderCachedFontBatch(pSprite);
			}

			SetChildOffset(Utils::Vector2(0.0f, 0.0f));
			pSprite->End();
		}
	}

	void ListBox::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Utils::Vector2 position(lParam);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->OnMessageReceived(uMsg, wParam, lParam);
		
		if (uMsg == WM_LBUTTONDOWN && !sWndProc.LastMessageHandled &&
			PtInBoundingRect(position))
			SetSelecting(true);

		float renderOffset = 0.0f;
		for (const auto &pListBoxEntry: m_entries) {
			SetChildOffset(Utils::Vector2(0.0f, renderOffset));
			renderOffset += pListBoxEntry->GetHeight() + 1.0f;
			pListBoxEntry->OnMessageReceived(uMsg, wParam, lParam);
		}

		SetChildOffset(Utils::Vector2(0.0f, 0.0f));

		if (IsSelecting()) {
			if (uMsg == WM_LBUTTONDOWN)
				sWndProc.LastMessageHandled = true;

			if (uMsg == WM_MOUSEMOVE)
				sWndProc.LastMessageHandled = true;

			if (uMsg == WM_LBUTTONUP) {
				SetSelecting(false);
				sWndProc.LastMessageHandled = true;
			}
		}
	}

	void ListBox::SetWidth(float width) {
		Rectangle::SetWidth(width);
		
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(width + 2.0f);

		for (const auto &pListBoxEntry: m_entries)
			pListBoxEntry->SetWidth(width);
	}

	void ListBox::SetHeight(float height) {
		Rectangle::SetHeight(height);
		
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(height + 2.0f);
	}

	void ListBox::SelectItem(const boost::shared_ptr<ListBoxEntry> &pItem) {
		if (!IsMultiSelect())
			DeselectAll();

		pItem->SetSelected(true);
	}

	void ListBox::DeselectItem(const boost::shared_ptr<ListBoxEntry> &pItem) {
		pItem->SetSelected(false);
	}

	void ListBox::DeselectAll() {
		for (const auto &pListBoxEntry: m_entries)
			pListBoxEntry->SetSelected(false);
	}
}
}
