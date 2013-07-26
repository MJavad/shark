#include "Misc/stdafx.h"
#include "Button.h"
#include "Base/WndProc.h"
#include "UI/GUIManager.h"

namespace UI {
namespace Components {
	boost::shared_ptr<Button> Button::Create(std::wstring textString,
											 float width,
											 float height,
											 float horizontalRounding,
											 float verticalRounding)
	{
		const auto pButton = boost::make_shared<Button>();
		pButton->SetCaption(Label::Create(std::move(textString), DT_CENTER | DT_VCENTER));

		const auto pBorder = Rectangle::Create(width, height);
		pBorder->SetColor(0xFF40B0FF);
		pBorder->SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		pBorder->SetHorizontalRounding(horizontalRounding);
		pBorder->SetVerticalRounding(verticalRounding);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		pButton->SetBorder(std::move(pBorder));
		pButton->SetWidth(width);
		pButton->SetHeight(height);
		pButton->SetHorizontalRounding(horizontalRounding);
		pButton->SetVerticalRounding(verticalRounding);

		std::array<D3DXCOLOR, 4> gradient;
		gradient[0] = 0xFF404040;
		gradient[1] = 0xFF404040;
		gradient[2] = 0xFF101010;
		gradient[3] = 0xFF101010;
		pButton->SetGradientColors(std::move(gradient));
		return pButton;
	}

	void Button::OnRender(uint32 timePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(timePassed);

		if (GetRenderBackground())
			Rectangle::OnRender(timePassed);
		
		const auto pCaption = GetCaption();
		if (pCaption != nullptr && pCaption->GetVisibility())
			pCaption->OnRender(timePassed);

		for (const auto &pTexture: GetTextures()) {
			if (pTexture->GetVisibility())
				pTexture->OnRender(timePassed);
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

	void Button::SetWidth(float width) {
		Rectangle::SetWidth(width);
			
		const auto pCaption = GetCaption();
		if (pCaption != nullptr)
			pCaption->SetWidth(width);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(width + 2);
	}

	void Button::SetHeight(float height) {
		Rectangle::SetHeight(height);
			
		const auto pCaption = GetCaption();
		if (pCaption != nullptr)
			pCaption->SetHeight(height);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(height + 2);
	}

	void Button::_notifyPushEvent(Utils::Vector2 *pPosition) {
		FadeTo(75, D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.4f));
		Focus();

		IPushable::_notifyPushEvent(pPosition);
	}

	void Button::_notifyClickEvent(Utils::Vector2 *pPosition) {
		if (IsHovered())
			FadeTo(100, GetHoverColor());

		IPushable::_notifyClickEvent(pPosition);
	}

	void Button::_notifyReleaseEvent(Utils::Vector2 *pPosition) {
		FadeTo(100, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		IPushable::_notifyReleaseEvent(pPosition);
	}

	bool Button::_notifyFocusBeginEvent() {
		const auto pBorder = GetBorder();
		bool result = IFocusable::_notifyFocusBeginEvent();

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

	void Button::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<Button,
							luabind::bases<Rectangle, IHoverable, IFocusable, IPushable>,
							boost::shared_ptr<IComponent>>("Button")
				.scope [ luabind::def("Create", &Button::CreateDefault) ]
				.def("AddTexture", &Button::AddTexture)
				.def("RemoveTexture", &Button::RemoveTexture)
				.property("caption", &Button::GetCaption, &Button::SetCaption)
				.property("border", &Button::GetBorder, &Button::SetBorder)
				.property("renderBackground", &Button::GetRenderBackground, &Button::SetRenderBackground)
		];
	}
}
}
