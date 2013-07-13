#include "Misc/stdafx.h"
#include "ListBoxEntry.h"
#include "ListBox.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::shared_ptr<ListBoxEntry> ListBoxEntry::Create(std::wstring textString) {
		const auto pListBoxEntry = std::make_shared<ListBoxEntry>();
		const auto pBorder = Rectangle::Create(0, 0);
		const auto pCaption = Label::Create(std::move(textString), DT_VCENTER);

		pBorder->SetColor(0xFF40B0FF);
		pBorder->SetColorMod(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		pBorder->SetVisibility(false);

		pCaption->SetPosition(Utils::Vector2(8.0f, 0.0f));

		pListBoxEntry->SetHeight(20);
		pListBoxEntry->SetSelectedColor(0x902060A0);
		pListBoxEntry->SetDeselectedColor(0x90202020);
		pListBoxEntry->SetBorder(std::move(pBorder));
		pListBoxEntry->SetCaption(std::move(pCaption));
		return pListBoxEntry;
	}

	void ListBoxEntry::OnRender(uint32 timePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(timePassed);

		Rectangle::OnRender(timePassed);
		
		const auto pCaption = GetCaption();
		if (pCaption != nullptr && !pCaption->IsCached())
			pCaption->CreateCachedFontBatch();
	}

	void ListBoxEntry::RenderCachedFontBatch(const std::shared_ptr<const ID3DSprite> &pSprite) const {
		const auto pCaption = GetCaption();
		if (pCaption != nullptr && pCaption->GetVisibility())
			pCaption->RenderCachedFontBatch(pSprite);
	}
	
	void ListBoxEntry::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		IHoverable::OnMessageReceived(uMsg, wParam, lParam);
		
		const auto pParent = std::dynamic_pointer_cast<ListBox>(GetUIParent());
		if (sWndProc.LastMessageHandled ||
			pParent == nullptr || !pParent->IsSelecting())
			return;

		Utils::Vector2 position(lParam);
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
			if (!GetSelected() && PtInBoundingRect(position))
				pParent->SelectItem(get_this<ListBoxEntry>());
			break;
		};
	}

	void ListBoxEntry::SetWidth(float width) {
		Rectangle::SetWidth(width);
			
		const auto pCaption = GetCaption();
		if (pCaption != nullptr) {
			float fCaption = width - pCaption->GetPosition().x;
			pCaption->SetWidth(fCaption > 1.0f ? fCaption : 1.0f);
		}

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(width + 2.0f);
	}

	void ListBoxEntry::SetHeight(float height) {
		Rectangle::SetHeight(height);
			
		const auto pCaption = GetCaption();
		if (pCaption != nullptr)
			pCaption->SetHeight(height);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(height + 2.0f);
	}
}
}
