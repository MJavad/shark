#include "Misc/stdafx.h"
#include "../resource.h"
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
#include "Base/FileManager.h"

using namespace UI::Components;

namespace UI {
	void GUIManager::Initialize() {
		const auto mainInterface = std::make_shared<ComponentInterface>();
		const auto backInterface = std::make_shared<ComponentInterface>();

		const auto testFrame1 = CreateBasicFrame(L"[shark.dll] :: Test", 600, 600, 0xD0000000);
		const auto testFrame2 = CreateBasicFrame(L"[shark.dll] :: Test", 140, 110, 0xD0000000);
		const auto testFrame3 = CreateBasicFrame(L"[shark.dll] :: Test", 300, 300, 0xD0000000);
		const auto testFrame4 = CreateBasicFrame(L"[shark.dll] :: Test", 140, 110, 0xD0000000);
		const auto testFrame5 = CreateBasicFrame(L"[shark.dll] :: Test", 140, 110, 0xD0000000);
		mainInterface->PushControl(testFrame1);
		mainInterface->PushControl(testFrame5);
		//backInterface->PushControl(testFrame3);

		const auto innerButton1 = Button::Create(L"Hide me!");
		innerButton1->SetPosition(Utils::Vector2(20.0f, 50.0f));

		const auto innerButton2 = Button::Create(L"Shutdown!");
		innerButton2->SetPosition(Utils::Vector2(20.0f, 75.0f));

		innerButton2->OnClickEvent += []
			(const std::shared_ptr<IPushable>&, Utils::Vector2*) {
				sEngine.InitializeShutdown();
			};
		
		const auto innerButton3 = Button::Create(L"I'm a button!");
		innerButton3->SetPosition(Utils::Vector2(20.0f, 100.0f));
		
		const auto innerButton4 = Button::Create(L"Show me!");
		innerButton4->SetPosition(Utils::Vector2(20.0f, 50.0f));

		const auto innerButton9 = Button::Create(L"I'm a button!");
		innerButton9->SetPosition(Utils::Vector2(20.0f, 75.0f));

		const auto innerButton5 = Button::Create(L"Container Button");
		innerButton5->SetPosition(Utils::Vector2(20.0f, 50.0f));

		const auto innerButton6 = Button::Create(L"Container Button");
		innerButton6->SetPosition(Utils::Vector2(20.0f, 75.0f));

		const auto innerButton7 = Button::Create(L"Container Button");
		innerButton7->SetPosition(Utils::Vector2(20.0f, 50.0f));

		const auto innerButton8 = Button::Create(L"Container Button");
		innerButton8->SetPosition(Utils::Vector2(20.0f, 75.0f));

		const auto innerButton10 = Button::Create(L"Container Button");
		innerButton10->SetPosition(Utils::Vector2(20.0f, 50.0f));
		
		const auto innerButton11 = Button::Create(L"Container Button");
		innerButton11->SetPosition(Utils::Vector2(20.0f, 75.0f));

		const auto tabButton1 = Button::Create(L"Tab Button");
		tabButton1->SetPosition(Utils::Vector2(10.0f, 10.0f));

		const auto innerListBox1 = ListBox::Create();
		innerListBox1->SetPosition(Utils::Vector2(200.0f, 60.0f));
		innerListBox1->AddItem(ListBoxEntry::Create(L"Item #1"));
		innerListBox1->AddItem(ListBoxEntry::Create(L"Item #2"));
		innerListBox1->AddItem(ListBoxEntry::Create(L"Item #3"));
		innerListBox1->AddItem(ListBoxEntry::Create(L"Item #4"));
		innerListBox1->AddItem(ListBoxEntry::Create(L"Item #5"));

		innerButton1->OnClickEvent += []
			(const std::shared_ptr<IPushable> &pSender, Utils::Vector2*) {
				pSender->GetUIParent()->Hide();
			};

		innerButton4->OnClickEvent += [testFrame1]
			(const std::shared_ptr<IPushable> &, Utils::Vector2*) {
				testFrame1->Show();
			};

		const auto tabControl = TabControl::Create(304, 300);
		const auto page1 = TabPage::Create(L"Test Page #1", 100, 20);
		const auto page2 = TabPage::Create(L"Test Page #2", 100, 20);
		const auto page3 = TabPage::Create(L"Test Page #3", 100, 20);
		tabControl->AddTabPage(page1);
		tabControl->AddTabPage(page2);
		tabControl->AddTabPage(page3);
		tabControl->SetPosition(Utils::Vector2(500.0f, 60));

		page1->GetContent()->PushChild(tabButton1);
		page2->GetContent()->PushChild(testFrame2);

		const auto editBox1 = EditBox::Create(true);
		editBox1->SetPosition(Utils::Vector2(500, 465));
		testFrame1->PushChild(editBox1);

		const auto editBox2 = EditBox::Create();
		editBox2->SetPosition(Utils::Vector2(500, 500));
		testFrame1->PushChild(editBox2);

		tabButton1->OnClickEvent += [testFrame2, editBox2]
			(const std::shared_ptr<IPushable>&, Utils::Vector2*) {
				testFrame2->SetVisibility(true);
				testFrame2->SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				editBox2->Focus();
				throw std::runtime_error("Testing...");
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

	std::shared_ptr<Frame> GUIManager::CreateBasicFrame(std::wstring swTitle, float width, float height, const D3DXCOLOR &color) const {
		const auto pFrame = Frame::Create(width, height);
		pFrame->SetMinSize(Utils::Vector2(140.0f, 50.0f));
		pFrame->SetMaxSize(Utils::Vector2(900.0f, 700.0f));

		float4 frameRoundings = {0};
		frameRoundings._1 = 14.0f;
		frameRoundings._2 = 14.0f;

		const auto pBackground = Rectangle::Create(width, height);
		pBackground->SetDropShadow(true);
		pBackground->SetColor(color);
		pBackground->SetVerticalRoundings(frameRoundings);
		pBackground->SetHorizontalRoundings(frameRoundings);
		pFrame->PushChild(pBackground);

		std::array<D3DXCOLOR, 4> gradient;
		gradient[0] = 0xFF505050;
		gradient[1] = 0xFF505050;
		gradient[2] = 0x50101010;
		gradient[3] = 0x50101010;

		const auto pHeaderBar = Rectangle::Create(width, 25.0f);
		pHeaderBar->SetGradientColors(std::move(gradient));
		pHeaderBar->SetVerticalRoundings(frameRoundings);
		pHeaderBar->SetHorizontalRoundings(frameRoundings);
		pFrame->PushChild(pHeaderBar);

		const auto pWindowTitle = Label::Create(std::move(swTitle), DT_CENTER | DT_VCENTER, width);
		pWindowTitle->SetPosition(Utils::Vector2(0.0f, 5.0f));
		pWindowTitle->SetColor(0x90FFFFFF);
		pWindowTitle->SetFont(sD3DMgr.GetFont(L"Corbel", 15, 0, FW_BOLD));
		pFrame->PushChild(pWindowTitle);

		const auto pBackgroundLineTop = Rectangle::Create(width, 1.0f);
		pBackgroundLineTop->SetPosition(Utils::Vector2(0.0f, 29.0f));
		pBackgroundLineTop->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineTop);

		const auto pBackgroundLineLeft = Rectangle::Create(1.0f, height - 31.0f);
		pBackgroundLineLeft->SetPosition(Utils::Vector2(0.0f, 30.0f));
		pBackgroundLineLeft->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineLeft);

		const auto pBackgroundLineBottom = Rectangle::Create(width - 1.0f, 1.0f);
		pBackgroundLineBottom->SetPosition(Utils::Vector2(0.0f, height - 1.0f));
		pBackgroundLineBottom->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineBottom);

		const auto pBackgroundLineRight = Rectangle::Create(1.0f, height - 31.0f);
		pBackgroundLineRight->SetPosition(Utils::Vector2(width - 1.0f, 30.0f));
		pBackgroundLineRight->SetColor(0x90303030);
		pFrame->PushChild(pBackgroundLineRight);

		const auto pCloseButtonTexture = Texture::Create(sD3DMgr.GetTextureFromResource(IDB_CLOSE_PNG));
		const auto pCloseButtonTextureHover = Texture::Create(sD3DMgr.GetTextureFromResource(IDB_CLOSE_HOVER_PNG));

		pCloseButtonTextureHover->SetVisibility(false);
		pCloseButtonTextureHover->SetColorMod(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		const auto pCloseButton = std::make_shared<Button>();
		pCloseButton->SetPosition(Utils::Vector2(width - 27.0f, 2.0f));
		pCloseButton->SetRenderRect(false);
		pCloseButton->SetWidth(23);
		pCloseButton->SetHeight(23);
		pCloseButton->AddTexture(pCloseButtonTexture);
		pCloseButton->AddTexture(pCloseButtonTextureHover);
		pCloseButton->SetHoverColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pFrame->PushChild(pCloseButton);

		// Register default events for frames...

		pCloseButton->OnHoverStartEvent +=
			[pCloseButtonTexture, pCloseButtonTextureHover]
			(const std::shared_ptr<IHoverable>&) {
				pCloseButtonTexture->Hide();
				pCloseButtonTextureHover->Show();
				return false;
			};

		pCloseButton->OnHoverEndEvent +=
			[pCloseButtonTexture, pCloseButtonTextureHover]
			(const std::shared_ptr<IHoverable>&) {
				pCloseButtonTextureHover->Hide();
				pCloseButtonTexture->Show();
				return false;
			};

		pCloseButton->OnClickEvent += []
			(const std::shared_ptr<IPushable> &pSender, Utils::Vector2*) {
				pSender->GetUIParent()->Hide();
			};

		pCloseButton->OnFocusStartEvent += []
			(const std::shared_ptr<IFocusable>&) {
				return true;
			};

		pFrame->OnResizeEvent += [pBackground, pWindowTitle, pHeaderBar, pBackgroundLineTop,
			pBackgroundLineLeft, pBackgroundLineBottom, pBackgroundLineRight, pCloseButton]
			(const std::shared_ptr<ISizable>&, float width, float height) {
				pBackground->SetWidth(width);
				pBackground->SetHeight(height);
				pWindowTitle->SetWidth(width);
				pHeaderBar->SetWidth(width);
				pBackgroundLineTop->SetWidth(width);
				pBackgroundLineLeft->SetHeight(height - 31.0f);
				pBackgroundLineBottom->SetWidth(width - 1.0f);
				pBackgroundLineRight->SetHeight(height - 31.0f);
				pBackgroundLineRight->SetPosition(Utils::Vector2(width - 1.0f, 30.0f));
				pBackgroundLineBottom->SetPosition(Utils::Vector2(0.0f, height - 1.0f));
				pCloseButton->SetPosition(Utils::Vector2(width - 27.0f, 2.0f));

				RECT screenRect = {0};
				const auto pRenderTarget = sD3DMgr.GetRenderTarget();
				if (pRenderTarget != nullptr &&
					pRenderTarget->GetSurfaceRect(&screenRect)) {
					Utils::Vector2 position = pBackground->GetScreenPosition();

					Utils::Vector2 vNormalized;
					vNormalized.x = (((position.x + pBackground->GetWidth() / 2.0f) * 2.0f) / screenRect.right - 1.0f) * -12.0f;
					vNormalized.y = (((position.y + pBackground->GetHeight() / 2.0f) * 2.0f) / screenRect.bottom - 1.0f) * -12.0f;
					pBackground->SetShadowDirection(vNormalized);
				}

				return false;
			};

		pFrame->OnSetPositionEvent += [pBackground]
			(const std::shared_ptr<Frame>&, const Utils::Vector2&) {
				RECT screenRect = {0};
				const auto pRenderTarget = sD3DMgr.GetRenderTarget();
				if (pRenderTarget != nullptr &&
					pRenderTarget->GetSurfaceRect(&screenRect)) {
					Utils::Vector2 position = pBackground->GetScreenPosition();

					Utils::Vector2 vNormalized;
					vNormalized.x = (((position.x + pBackground->GetWidth() / 2.0f) * 2.0f) / screenRect.right - 1.0f) * -12.0f;
					vNormalized.y = (((position.y + pBackground->GetHeight() / 2.0f) * 2.0f) / screenRect.bottom - 1.0f) * -12.0f;
					pBackground->SetShadowDirection(vNormalized);
				}
			};

		pFrame->OnFrameHighlightStartEvent += [pHeaderBar, pBackgroundLineTop,
			pBackgroundLineLeft, pBackgroundLineBottom, pBackgroundLineRight]
			(const std::shared_ptr<Frame>&) {
				D3DXCOLOR fadeIn(0.6f, 1.3f, 1.6f, 1.0f);
				pHeaderBar->FadeTo(200, fadeIn);
				pBackgroundLineTop->FadeTo(200, fadeIn);
				pBackgroundLineLeft->FadeTo(200, fadeIn);
				pBackgroundLineBottom->FadeTo(200, fadeIn);
				pBackgroundLineRight->FadeTo(200, fadeIn);
			};

		pFrame->OnFrameHighlightEndEvent += [pHeaderBar, pBackgroundLineTop,
			pBackgroundLineLeft, pBackgroundLineBottom, pBackgroundLineRight]
			(const std::shared_ptr<Frame>&) {
				D3DXCOLOR fadeOut(1.0f, 1.0f, 1.0f, 1.0f);
				pHeaderBar->FadeTo(200, fadeOut);
				pBackgroundLineTop->FadeTo(200, fadeOut);
				pBackgroundLineLeft->FadeTo(200, fadeOut);
				pBackgroundLineBottom->FadeTo(200, fadeOut);
				pBackgroundLineRight->FadeTo(200, fadeOut);
			};

		return pFrame;
	}
}
