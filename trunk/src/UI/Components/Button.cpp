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
		float4 fInvisible = {0.0f, 0.0f, 0.0f, 0.0f};
		pBorder->SetColor(Utils::Color(0xFF, 0x40, 0xB0, 0xFF));
		pBorder->SetColorMod(fInvisible);
		pBorder->SetHorizontalRounding(fHorizontalRounding);
		pBorder->SetVerticalRounding(fVerticalRounding);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		pButton->SetBorder(std::move(pBorder));
		pButton->SetWidth(fWidth);
		pButton->SetHeight(fHeight);
		pButton->SetHorizontalRounding(fHorizontalRounding);
		pButton->SetVerticalRounding(fVerticalRounding);

		std::array<Utils::Color, 4> gradient;
		gradient[0] = Utils::Color(0xFF, 0x40, 0x40, 0x40);
		gradient[1] = Utils::Color(0xFF, 0x40, 0x40, 0x40);
		gradient[2] = Utils::Color(0xFF, 0x10, 0x10, 0x10);
		gradient[3] = Utils::Color(0xFF, 0x10, 0x10, 0x10);
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
					OnClickEventNotify(nullptr);
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

	void Button::OnPushEventNotify(Utils::Vector2 *pvPosition) {
		float4 fPressed = {1.4f, 0.80f, 0.80f, 0.80f};
		FadeTo(75, fPressed);
		Focus();

		IPushable::OnPushEventNotify(pvPosition);
	}

	void Button::OnClickEventNotify(Utils::Vector2 *pvPosition) {
		if (IsHovered())
			FadeTo(100, GetHoverColor());

		IPushable::OnClickEventNotify(pvPosition);
	}

	void Button::OnReleaseEventNotify(Utils::Vector2 *pvPosition) {
		float4 fOrig = {1.0f, 1.0f, 1.0f, 1.0f};
		FadeTo(100, fOrig);

		IPushable::OnReleaseEventNotify(pvPosition);
	}

	bool Button::OnFocusStartEventNotify() {
		const auto pBorder = GetBorder();
		bool result = IFocusable::OnFocusStartEventNotify();

		if (!result && pBorder != nullptr) {
			float4 fFocused = {1.0f, 1.0f, 1.0f, 1.0f};
			pBorder->FadeTo(100, fFocused);
		}

		return result;
	}

	void Button::OnFocusEndEventNotify() {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr) {
			float4 fOrig = {0.0f, 0.0f, 0.0f, 0.0f};
			pBorder->FadeTo(300, fOrig);
		}

		IFocusable::OnFocusEndEventNotify();
	}
}
}
