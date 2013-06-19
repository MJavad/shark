#include "Misc/stdafx.h"
#include "EditBox.h"
#include "Base/WndProc.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	bool EditBox::s_renderCaret = false;
	uint32 EditBox::s_caretPosition = 3;
	uint32 EditBox::s_caretTimer = 0;
	bool EditBox::s_activeSelection = false;
	bool EditBox::s_swapSelection = false;

	std::shared_ptr<EditBox> EditBox::Create(float fWidth, float fHeight,
		float fHorizontalRounding, float fVerticalRounding) {
		const auto pEditBox = std::make_shared<EditBox>();
		const auto pLabel = Label::Create(L"", DT_CENTER | DT_VCENTER | DT_SINGLELINE, fWidth, fHeight);
		//pLabel->SetDropShadow(false);

		const auto pBorder = Rectangle::Create(fWidth, fHeight);
		pBorder->SetColor(Utils::Color(0x90, 0xB0, 0xB0, 0xB0));
		pBorder->SetHorizontalRounding(fHorizontalRounding);
		pBorder->SetVerticalRounding(fVerticalRounding);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		pEditBox->SetContent(std::move(pLabel));
		pEditBox->SetBorder(std::move(pBorder));
		pEditBox->SetWidth(fWidth);
		pEditBox->SetHeight(fHeight);
		pEditBox->SetHorizontalRounding(fHorizontalRounding);
		pEditBox->SetVerticalRounding(fVerticalRounding);

		std::array<Utils::Color, 4> gradient;
		gradient[0] = Utils::Color(0xFF, 0x25, 0x25, 0x25);
		gradient[1] = Utils::Color(0xFF, 0x25, 0x25, 0x25);
		gradient[2] = Utils::Color(0xFF, 0x10, 0x10, 0x10);
		gradient[3] = Utils::Color(0xFF, 0x10, 0x10, 0x10);
		pEditBox->SetGradientColors(std::move(gradient));
		return pEditBox;
	}

	void EditBox::OnRender(uint32 uTimePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(uTimePassed);

		Rectangle::OnRender(uTimePassed);

		float textHeight = 0;
		const auto pContent = GetContent();
		if (pContent != nullptr) {
			pContent->OnRender(uTimePassed);

			const auto pFont = pContent->GetFont();
			if (pFont != nullptr)
				textHeight = static_cast<float>(pFont->GetDescription().Height);
		}

		// caret specific stuff...
		if (!IsFocused())
			return;

		uint32 timeNow = timeGetTime();
		if (timeNow - s_caretTimer > 600) {
			s_caretTimer = timeNow;
			s_renderCaret = !s_renderCaret;
		}

		Utils::Vector2 vScreen = GetScreenPosition();
		const auto pRenderTarget = sD3DMgr.GetRenderTarget();

		if (s_renderCaret) {
			Utils::Vector2 vCaret;
			vCaret.x = float(pContent->CPToX(s_caretPosition));
			vCaret.y = vScreen.y + (GetHeight() / 2.0f - textHeight / 2.0f);

			std::array<Utils::Color, 4> gradient;
			gradient.fill(pContent->GetColor());
			pRenderTarget->FillRectangle(vCaret, IRenderTarget::MakeDimension(3.0f, textHeight), gradient);
		}
	}

	void EditBox::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!sWndProc.LastMessageHandled &&
			GetVisibility() && IsFocused()) {
			switch (uMsg)
			{
			case WM_CHAR:
				_onChar(wParam & 0xFFFF);
				break;

			case WM_KEYDOWN:
				_onKeyDown(wParam);
				break;

			case WM_MOUSEMOVE:
				_onMouseMove(Utils::Vector2(lParam));
				break;
			}
		}

		IPushable::OnMessageReceived(uMsg, wParam, lParam);
		IFocusable::OnMessageReceived(uMsg, wParam, lParam);
	}

	void EditBox::SetWidth(float fWidth) {
		Rectangle::SetWidth(fWidth);

		const auto pContent = GetContent();
		if (pContent != nullptr)
			pContent->SetWidth(max(fWidth - 10.0f, 0.0f));

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(fWidth + 2);
	}

	void EditBox::SetHeight(float fHeight) {
		Rectangle::SetHeight(fHeight);

		const auto pContent = GetContent();
		if (pContent != nullptr)
			pContent->SetHeight(fHeight);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(fHeight + 2);
	}

	void EditBox::_onChar(wchar_t c) {
		std::wstring swText;
		swText += c;
		InsertText(std::move(swText));
	}

	void EditBox::_onKeyDown(int key) {

	}

	void EditBox::_onMouseMove(const Utils::Vector2 &vPosition) {

	}

	void EditBox::InsertText(std::wstring swText) {
		for (auto itr = swText.begin(), end = swText.end(); itr != end; ) {
			if (!isprint(*itr))
				swText.erase(itr++);
			else
				++itr;
		}

		const auto pContent = GetContent();
		if (pContent != nullptr)
			pContent->SetText(pContent->GetText() + swText);
	}

	void EditBox::EraseText(uint32 numChars) {

	}

	void EditBox::OnPushEventNotify(Utils::Vector2 *pvPosition) {
		const auto pContent = GetContent();
		if (pContent != nullptr && pvPosition != nullptr)
			s_caretPosition = pContent->XToCP(static_cast<int32>(pvPosition->x));

		if (IsFocused()) {
			// handle double click
		}
		else
			Focus();

		IPushable::OnPushEventNotify(pvPosition);
	}

	void EditBox::OnClickEventNotify(Utils::Vector2 *pvPosition) {

		IPushable::OnClickEventNotify(pvPosition);
	}

	void EditBox::OnReleaseEventNotify(Utils::Vector2 *pvPosition) {
		s_activeSelection = false;
		IPushable::OnReleaseEventNotify(pvPosition);
	}

	bool EditBox::OnFocusStartEventNotify() {
		const auto pBorder = GetBorder();
		bool result = IFocusable::OnFocusStartEventNotify();

		if (!result && pBorder != nullptr) {
			float4 fFocused = {1.77f, 0.36f, 1.0f, 1.45f};
			pBorder->FadeTo(100, fFocused);
		}

		return result;
	}

	void EditBox::OnFocusEndEventNotify() {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr) {
			float4 fOrig = {1.0f, 1.0f, 1.0f, 1.0f};
			pBorder->FadeTo(300, fOrig);
		}

		return IFocusable::OnFocusEndEventNotify();
	}
}
}
