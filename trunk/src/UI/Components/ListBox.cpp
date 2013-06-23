#include "Misc/stdafx.h"
#include "ListBox.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::shared_ptr<ListBox> ListBox::Create(float fWidth, float fHeight) {
		const auto pListBox = std::make_shared<ListBox>();
		const auto pBorder = Rectangle::Create(fWidth, fHeight);
		
		pBorder->SetColor(0x90404040);
		pListBox->SetBorder(std::move(pBorder));

		pListBox->SetWidth(fWidth);
		pListBox->SetHeight(fHeight);
		pListBox->SetColor(0x90101010);
		return pListBox;
	}

	void ListBox::OnRender(uint32 uTimePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(uTimePassed);

		Rectangle::OnRender(uTimePassed);

		float fItemOffset = 0.0f;
		for (const auto &pListBoxEntry: m_entries) {
			SetChildOffset(Utils::Vector2(0.0f, fItemOffset));
			fItemOffset += pListBoxEntry->GetHeight() + 1.0f;

			if (pListBoxEntry->GetVisibility())
				pListBoxEntry->OnRender(uTimePassed);
		}
		
		const auto pSprite = sD3DMgr.GetSprite();
		if (pSprite != nullptr) {
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);

			fItemOffset = 0.0f;
			for (const auto &pListBoxEntry: m_entries) {
				SetChildOffset(Utils::Vector2(0.0f, fItemOffset));
				fItemOffset += pListBoxEntry->GetHeight() + 1.0f;

				if (pListBoxEntry->GetVisibility())
					pListBoxEntry->RenderCachedFontBatch(pSprite);
			}
			SetChildOffset(Utils::Vector2(0.0f, 0.0f));

			pSprite->End();
		}
	}

	void ListBox::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Utils::Vector2 vPosition(lParam);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->OnMessageReceived(uMsg, wParam, lParam);
		
		if (uMsg == WM_LBUTTONDOWN && !sWndProc.LastMessageHandled &&
			PtInBoundingRect(vPosition))
			SetSelecting(true);

		float fItemOffset = 0.0f;
		for (const auto &pListBoxEntry: m_entries) {
			SetChildOffset(Utils::Vector2(0.0f, fItemOffset));
			fItemOffset += pListBoxEntry->GetHeight() + 1.0f;
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
		
	void ListBox::SetWidth(float fWidth) {
		Rectangle::SetWidth(fWidth);
		
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(fWidth + 2.0f);

		for (const auto &pListBoxEntry: m_entries)
			pListBoxEntry->SetWidth(fWidth);
	}

	void ListBox::SetHeight(float fHeight) {
		Rectangle::SetHeight(fHeight);
		
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(fHeight + 2.0f);
	}

	void ListBox::SelectItem(const std::shared_ptr<ListBoxEntry> &pItem) {
		if (!IsMultiSelect())
			DeselectAll();

		pItem->SetSelected(true);
	}

	void ListBox::DeselectItem(const std::shared_ptr<ListBoxEntry> &pItem) {
		pItem->SetSelected(false);
	}

	void ListBox::DeselectAll() {
		for (const auto &pListBoxEntry: m_entries)
			pListBoxEntry->SetSelected(false);
	}
}
}
