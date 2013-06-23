#include "Misc/stdafx.h"
#include "Button.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::shared_ptr<Button> Button::Create(std::wstring swText,
		float fWidth, float fHeight, float fHorizontalRounding, float fVerticalRounding) {
		const auto pButton = std::make_shared<Button>();
		pButton->SetCaption(Label::Create(std::move(swText), DT_VCENTER | DT_CENTER));

		const auto pBorder = Rectangle::Create(fWidth, fHeight);
		pBorder->SetColor(0xFF40B0FF);
		pBorder->SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		pBorder->SetHorizontalRounding(fHorizontalRounding);
		pBorder->SetVerticalRounding(fVerticalRounding);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		pButton->SetBorder(std::move(pBorder));
		pButton->SetWidth(fWidth);
		pButton->SetHeight(fHeight);
		pButton->SetHorizontalRounding(fHorizontalRounding);
		pButton->SetVerticalRounding(fVerticalRounding);

		std::array<D3DXCOLOR, 4> gradient;
		gradient[0] = 0xFF404040;
		gradient[1] = 0xFF404040;
		gradient[2] = 0xFF101010;
		gradient[3] = 0xFF101010;
		pButton->SetGradientColors(std::move(gradient));
		return pButton;
	}

	void Button::OnRender(uint32 uTimePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(uTimePassed);

		if (GetRenderRect())
			Rectangle::OnRender(uTimePassed);
		
		const auto pCaption = GetCaption();
		if (pCaption != nullptr && pCaption->GetVisibility())
			pCaption->OnRender(uTimePassed);

		for (const auto &pTexture: GetTextures()) {
			if (pTexture->GetVisibility())
				pTexture->OnRender(uTimePassed);
		}
	}

	void Button::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (IsFocused()) {
			switch (uMsg)
			{
			case WM_LBUTTONDOWN:
				if (sWndProc.LastMessageHandled ||
					!PtInBoundingRect(Utils::Vector2(lParam)))
					Unfocus();
				break;

			case WM_CHAR:
				if (wParam == VK_SPACE &&
					!sWndProc.LastMessageHandled) {
					_notifyClickEvent(nullptr);
					sWndProc.LastMessageHandled = true;
				}
				break;

			case WM_KEYDOWN:
				if (wParam == VK_ESCAPE &&
					!sWndProc.LastMessageHandled) {
					Unfocus();
					sWndProc.LastMessageHandled = true;
				}
				break;
			}
		}

		IHoverable::OnMessageReceived(uMsg, wParam, lParam);
		IPushable::OnMessageReceived(uMsg, wParam, lParam);
		IFocusable::OnMessageReceived(uMsg, wParam, lParam);
	}

	void Button::SetWidth(float fWidth) {
		Rectangle::SetWidth(fWidth);
			
		const auto pCaption = GetCaption();
		if (pCaption != nullptr)
			pCaption->SetWidth(fWidth);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(fWidth + 2);
	}

	void Button::SetHeight(float fHeight) {
		Rectangle::SetHeight(fHeight);
			
		const auto pCaption = GetCaption();
		if (pCaption != nullptr)
			pCaption->SetHeight(fHeight);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(fHeight + 2);
	}

	void Button::_notifyPushEvent(Utils::Vector2 *pvPosition) {
		FadeTo(75, D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.4f));
		Focus();

		IPushable::_notifyPushEvent(pvPosition);
	}

	void Button::_notifyClickEvent(Utils::Vector2 *pvPosition) {
		if (IsHovered())
			FadeTo(100, GetHoverColor());

		IPushable::_notifyClickEvent(pvPosition);
	}

	void Button::_notifyReleaseEvent(Utils::Vector2 *pvPosition) {
		FadeTo(100, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		IPushable::_notifyReleaseEvent(pvPosition);
	}

	bool Button::_notifyFocusStartEvent() {
		const auto pBorder = GetBorder();
		bool result = IFocusable::_notifyFocusStartEvent();

		if (!result && pBorder != nullptr)
			pBorder->FadeTo(100, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		return result;
	}

	void Button::_notifyFocusEndEvent() {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->FadeTo(300, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		IFocusable::_notifyFocusEndEvent();
	}
}
}
