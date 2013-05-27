#include "Misc/stdafx.h"
#include "GUIManager.h"
#include "Base/D3DManager.h"
#include "Base/Detours.h"
#include "Components/Rectangle.h"
#include "Components/Button.h"
#include "Components/ListBox.h"
#include "Components/Texture.h"
#include "Components/TabControl.h"
#include "Components/EditBox.h"
#include "Base/Engine.h"

INIT_SINGLETON(UI::GUIManager);
UI::GUIManager sGUIMgr;

namespace UI {
	void GUIManager::Initialize() {
		std::shared_ptr<ComponentInterface> mainInterface = std::make_shared<ComponentInterface>();
		std::shared_ptr<ComponentInterface> backInterface = std::make_shared<ComponentInterface>();
		std::shared_ptr<Components::Frame> testFrame1 = CreateBasicFrame(L"[shark.dll] :: Test", 600, 600, 0xD0000000);
		std::shared_ptr<Components::Frame> testFrame2 = CreateBasicFrame(L"[shark.dll] :: Test", 140, 110, 0xD0000000);
		std::shared_ptr<Components::Frame> testFrame3 = CreateBasicFrame(L"[shark.dll] :: Test", 300, 300, 0xD0000000);
		std::shared_ptr<Components::Frame> testFrame4 = CreateBasicFrame(L"[shark.dll] :: Test", 140, 110, 0xD0000000);
		std::shared_ptr<Components::Frame> testFrame5 = CreateBasicFrame(L"[shark.dll] :: Test", 140, 110, 0xD0000000);
		mainInterface->PushControl(testFrame1);
		mainInterface->PushControl(testFrame5);
		//backInterface->PushControl(testFrame3);

		std::shared_ptr<Components::Button> innerButton1 =
			Components::Button::Create(L"Hide me!");
		innerButton1->SetPosition(Utils::Vector2(20.0f, 50.0f));

		std::shared_ptr<Components::Button> innerButton2 =
			Components::Button::Create(L"Shutdown!");
		innerButton2->SetPosition(Utils::Vector2(20.0f, 75.0f));
		innerButton2->OnClickEvent += [] (const Utils::Vector2&) {
			sEngine.InitializeShutdown();
		};
		
		std::shared_ptr<Components::Button> innerButton3 =
			Components::Button::Create(L"I'm a button!");
		innerButton3->SetPosition(Utils::Vector2(20.0f, 100.0f));
		
		std::shared_ptr<Components::Button> innerButton4 =
			Components::Button::Create(L"Show me!");
		innerButton4->SetPosition(Utils::Vector2(20.0f, 50.0f));

		std::shared_ptr<Components::Button> innerButton9 =
			Components::Button::Create(L"I'm a button!");
		innerButton9->SetPosition(Utils::Vector2(20.0f, 75.0f));

		std::shared_ptr<Components::Button> innerButton5 =
			Components::Button::Create(L"Container Button");
		innerButton5->SetPosition(Utils::Vector2(20.0f, 50.0f));

		std::shared_ptr<Components::Button> innerButton6 =
			Components::Button::Create(L"Container Button");
		innerButton6->SetPosition(Utils::Vector2(20.0f, 75.0f));

		std::shared_ptr<Components::Button> innerButton7 =
			Components::Button::Create(L"Container Button");
		innerButton7->SetPosition(Utils::Vector2(20.0f, 50.0f));

		std::shared_ptr<Components::Button> innerButton8 =
			Components::Button::Create(L"Container Button");
		innerButton8->SetPosition(Utils::Vector2(20.0f, 75.0f));

		std::shared_ptr<Components::Button> innerButton10 =
			Components::Button::Create(L"Container Button");
		innerButton10->SetPosition(Utils::Vector2(20.0f, 50.0f));
		
		std::shared_ptr<Components::Button> innerButton11 =
			Components::Button::Create(L"Container Button");
		innerButton11->SetPosition(Utils::Vector2(20.0f, 75.0f));

		std::shared_ptr<Components::Button> tabButton1 =
			Components::Button::Create(L"Tab Button");
		tabButton1->SetPosition(Utils::Vector2(10.0f, 10.0f));

		std::shared_ptr<Components::ListBox> innerListBox1 =
			Components::ListBox::Create();
		innerListBox1->SetPosition(Utils::Vector2(200.0f, 60.0f));
		innerListBox1->AddItem(UI::Components::ListBoxEntry::Create(L"Item #1"));
		innerListBox1->AddItem(UI::Components::ListBoxEntry::Create(L"Item #2"));
		innerListBox1->AddItem(UI::Components::ListBoxEntry::Create(L"Item #3"));
		innerListBox1->AddItem(UI::Components::ListBoxEntry::Create(L"Item #4"));
		innerListBox1->AddItem(UI::Components::ListBoxEntry::Create(L"Item #5"));

		innerButton1->OnClickEvent += [testFrame1] (const Utils::Vector2&) {
			testFrame1->Hide();
		};

		innerButton4->OnClickEvent += [testFrame1] (const Utils::Vector2&) {
			testFrame1->Show();
		};

		auto tabControl = Components::TabControl::Create(304, 300);
		auto page1 = Components::TabPage::Create(L"Test Page #1", 100, 20);
		auto page2 = Components::TabPage::Create(L"Test Page #2", 100, 20);
		auto page3 = Components::TabPage::Create(L"Test Page #3", 100, 20);
		tabControl->AddTabPage(page1);
		tabControl->AddTabPage(page2);
		tabControl->AddTabPage(page3);
		tabControl->SetPosition(Utils::Vector2(500.0f, 60));

		page1->GetContent()->PushChild(tabButton1);
		page2->GetContent()->PushChild(testFrame2);

		auto editBox = Components::EditBox::Create();
		editBox->SetPosition(Utils::Vector2(500, 500));
		testFrame1->PushChild(editBox);

		tabButton1->OnClickEvent += [testFrame2, editBox] (const Utils::Vector2&) {
			float4 fVisib = {1.0f, 1.0f, 1.0f, 1.0f};
			testFrame2->SetColorMod(fVisib);
			testFrame2->SetVisibility(true);
			editBox->Focus();
		};

		editBox->OnFocusStartEvent += [] {
			//throw std::invalid_argument("Testing exceptions!");
			return false;
		};

		testFrame1->PushChild(tabControl);
		
		testFrame1->PushChild(innerListBox1);
		testFrame1->PushChild(innerButton1);
		testFrame1->PushChild(innerButton2);
		testFrame1->PushChild(innerButton3);
		testFrame5->PushChild(innerButton4);
		//testFrame1->PushChild(testFrame2);

		testFrame1->PushChild(testFrame3);
		testFrame3->PushChild(innerButton5);
		testFrame3->PushChild(innerButton6);
		testFrame3->PushChild(testFrame4);
		testFrame4->PushChild(innerButton7);
		testFrame4->PushChild(innerButton8);
		testFrame5->PushChild(innerButton9);

		testFrame2->PushChild(innerButton10);
		testFrame2->PushChild(innerButton11);

		sD3DMgr.PushInterface(backInterface);
		sD3DMgr.PushInterface(mainInterface);
	}

	std::shared_ptr<Components::Frame> GUIManager::CreateBasicFrame(std::wstring swTitle, float fWidth, float fHeight, const Utils::Color &color) const {
		const auto pFrame = Components::Frame::Create(fWidth, fHeight);
		pFrame->SetMinSize(Utils::Vector2(140.0f, 50.0f));
		std::weak_ptr<Components::Frame> pWeakFrame = pFrame;

		float4 verticalRoundings = {0};
		verticalRoundings._1 = 14.0f;
		verticalRoundings._2 = 14.0f;

		float4 horizontalRoundings = {0};
		horizontalRoundings._1 = 14.0f;
		horizontalRoundings._2 = 14.0f;

		const auto pBackground = Components::Rectangle::Create(fWidth, fHeight);
		pBackground->SetDropShadow(true);
		pBackground->SetColor(color);
		pBackground->SetVerticalRoundings(verticalRoundings);
		pBackground->SetHorizontalRoundings(horizontalRoundings);
		pFrame->PushChild(pBackground);

		const auto pHeaderBar = Components::Rectangle::Create(fWidth, 25.0f);
		std::array<Utils::Color, 4> gradient;
		gradient[0] = 0xFF505050;
		gradient[1] = 0xFF505050;
		gradient[2] = 0x50101010;
		gradient[3] = 0x50101010;

		pHeaderBar->SetGradientColors(gradient);
		pHeaderBar->SetVerticalRoundings(verticalRoundings);
		pHeaderBar->SetHorizontalRoundings(horizontalRoundings);
		pFrame->PushChild(pHeaderBar);

		const auto pWindowTitle = Components::Label::Create(std::move(swTitle), DT_CENTER | DT_VCENTER, fWidth);
		pWindowTitle->SetPosition(Utils::Vector2(0.0f, 5.0f));
		pWindowTitle->SetColor(0x90FFFFFF);
		pWindowTitle->SetFont(sD3DMgr.GetFont(L"Corbel", 15, 0, FW_BOLD));
		pFrame->PushChild(pWindowTitle);

		const auto pBackgroundLineTop = Components::Rectangle::Create(fWidth, 1.0f);
		pBackgroundLineTop->SetPosition(Utils::Vector2(0.0f, 29.0f));
		pBackgroundLineTop->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineTop);

		const auto pBackgroundLineLeft = Components::Rectangle::Create(1.0f, fHeight - 31.0f);
		pBackgroundLineLeft->SetPosition(Utils::Vector2(0.0f, 30.0f));
		pBackgroundLineLeft->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineLeft);

		const auto pBackgroundLineBottom = Components::Rectangle::Create(fWidth - 1.0f, 1.0f);
		pBackgroundLineBottom->SetPosition(Utils::Vector2(0.0f, fHeight - 1.0f));
		pBackgroundLineBottom->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineBottom);

		const auto pBackgroundLineRight = Components::Rectangle::Create(1.0f, fHeight - 31.0f);
		pBackgroundLineRight->SetPosition(Utils::Vector2(fWidth - 1.0f, 30.0f));
		pBackgroundLineRight->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineRight);

		const auto pCloseButtonTexture = Components::Texture::Create(sD3DMgr.GetTextureFromFile(L"E:\\shark\\close.png"));
		const auto pCloseButtonTextureHover = Components::Texture::Create(sD3DMgr.GetTextureFromFile(L"E:\\shark\\close_hover.png"));

		float4 fInvisible = {0.0f, 1.0f, 1.0f, 1.0f};
		pCloseButtonTextureHover->SetVisibility(false);
		pCloseButtonTextureHover->SetColorMod(fInvisible);

		const auto pCloseButton = std::make_shared<Components::Button>();
		pCloseButton->SetPosition(Utils::Vector2(fWidth - 27.0f, 2.0f));
		pCloseButton->SetRenderRect(false);
		pCloseButton->SetWidth(23);
		pCloseButton->SetHeight(23);
		pCloseButton->AddTexture(pCloseButtonTexture);
		pCloseButton->AddTexture(pCloseButtonTextureHover);
		pFrame->PushChild(pCloseButton);

		float4 fHoverColor = {1.0f, 1.0f, 1.0f, 1.0f};
		pCloseButton->SetHoverColor(fHoverColor);

		pCloseButton->OnHoverStartEvent += [pCloseButtonTexture, pCloseButtonTextureHover] {
			pCloseButtonTexture->Hide();
			pCloseButtonTextureHover->Show();
			return false;
		};

		pCloseButton->OnHoverEndEvent += [pCloseButtonTexture, pCloseButtonTextureHover] {
			pCloseButtonTextureHover->Hide();
			pCloseButtonTexture->Show();
			return false;
		};

		pCloseButton->OnClickEvent += [pWeakFrame] (const Utils::Vector2&) {
			pWeakFrame.lock()->Hide();
		};

		pCloseButton->OnFocusStartEvent += [] { return true; };

		pFrame->OnResizeEvent += [pBackground, pWindowTitle, pHeaderBar, pBackgroundLineTop, pBackgroundLineLeft,
								  pBackgroundLineBottom, pBackgroundLineRight, pCloseButton] (float fWidth, float fHeight) {
			pBackground->SetWidth(fWidth);
			pBackground->SetHeight(fHeight);
			pWindowTitle->SetWidth(fWidth);
			pHeaderBar->SetWidth(fWidth);
			pBackgroundLineTop->SetWidth(fWidth);
			pBackgroundLineLeft->SetHeight(fHeight - 31.0f);
			pBackgroundLineBottom->SetWidth(fWidth - 1.0f);
			pBackgroundLineRight->SetHeight(fHeight - 31.0f);
			pBackgroundLineRight->SetPosition(Utils::Vector2(fWidth - 1.0f, 30.0f));
			pBackgroundLineBottom->SetPosition(Utils::Vector2(0.0f, fHeight - 1.0f));
			pCloseButton->SetPosition(Utils::Vector2(fWidth - 27.0f, 2.0f));

			RECT screenRect = {0};
			sD3DMgr.GetRenderTarget()->GetSurfaceRect(&screenRect);
			Utils::Vector2 vPosition = pBackground->GetScreenPosition();

			Utils::Vector2 vNormalized;
			vNormalized.x = (((vPosition.x + pBackground->GetWidth() / 2.0f) * 2.0f) / screenRect.right - 1.0f) * -12.0f;
			vNormalized.y = (((vPosition.y + pBackground->GetHeight() / 2.0f) * 2.0f) / screenRect.bottom - 1.0f) * -12.0f;
			pBackground->SetShadowDirection(vNormalized);
			return false;
		};

		pFrame->OnFocusStartEvent += [pHeaderBar, pBackgroundLineTop, pBackgroundLineLeft,
								      pBackgroundLineBottom, pBackgroundLineRight] {
			float4 fadeIn = {1.0f, 0.6f, 1.3f, 1.6f};
			pHeaderBar->FadeTo(200, fadeIn);
			pBackgroundLineTop->FadeTo(200, fadeIn);
			pBackgroundLineLeft->FadeTo(200, fadeIn);
			pBackgroundLineBottom->FadeTo(200, fadeIn);
			pBackgroundLineRight->FadeTo(200, fadeIn);
		};

		pFrame->OnFocusStopEvent += [pHeaderBar, pBackgroundLineTop, pBackgroundLineLeft,
								     pBackgroundLineBottom, pBackgroundLineRight] {
			float4 fadeOut = {1.0f, 1.0f, 1.0f, 1.0f};
			pHeaderBar->FadeTo(200, fadeOut);
			pBackgroundLineTop->FadeTo(200, fadeOut);
			pBackgroundLineLeft->FadeTo(200, fadeOut);
			pBackgroundLineBottom->FadeTo(200, fadeOut);
			pBackgroundLineRight->FadeTo(200, fadeOut);
		};

		pFrame->OnSetPositionEvent += [pBackground] (const Utils::Vector2&) {
			RECT screenRect = {0};
			sD3DMgr.GetRenderTarget()->GetSurfaceRect(&screenRect);
			Utils::Vector2 vPosition = pBackground->GetScreenPosition();

			Utils::Vector2 vNormalized;
			vNormalized.x = (((vPosition.x + pBackground->GetWidth() / 2.0f) * 2.0f) / screenRect.right - 1.0f) * -12.0f;
			vNormalized.y = (((vPosition.y + pBackground->GetHeight() / 2.0f) * 2.0f) / screenRect.bottom - 1.0f) * -12.0f;
			pBackground->SetShadowDirection(vNormalized);
		};

		return pFrame;
	}
}
