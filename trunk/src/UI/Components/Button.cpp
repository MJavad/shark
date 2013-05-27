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
		pBorder->SetColor(0xFF40B0FF);
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
					OnClickRequest(Utils::Vector2());
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

	void Button::OnPushRequest(const Utils::Vector2 &vPosition) {
		float4 fPressed = {1.4f, 0.80f, 0.80f, 0.80f};
		FadeTo(75, fPressed);
		Focus();

		IPushable::OnPushRequest(vPosition);
	}

	void Button::OnClickRequest(const Utils::Vector2 &vPosition) {
		if (IsHovered())
			FadeTo(100, GetHoverColor());

		IPushable::OnClickRequest(vPosition);
	}

	void Button::OnReleaseRequest(const Utils::Vector2 &vPosition) {
		float4 fOrig = {1.0f, 1.0f, 1.0f, 1.0f};
		FadeTo(100, fOrig);

		IPushable::OnReleaseRequest(vPosition);
	}

	bool Button::OnFocusStartRequest() {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr) {
			float4 fFocused = {1.0f, 1.0f, 1.0f, 1.0f};
			pBorder->FadeTo(100, fFocused);
		}

		return IFocusable::OnFocusStartRequest();
	}

	void Button::OnFocusEndRequest() {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr) {
			float4 fOrig = {0.0f, 0.0f, 0.0f, 0.0f};
			pBorder->FadeTo(300, fOrig);
		}

		IFocusable::OnFocusEndRequest();
	}
}
}
