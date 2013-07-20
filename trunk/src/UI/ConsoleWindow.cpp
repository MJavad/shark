#include "Misc/stdafx.h"
#include "ConsoleWindow.h"
#include "Base/D3DManager.h"
#include "Base/WndProc.h"

namespace UI {
	void ConsoleWindow::Initialize() {
		m_isInitialized = true;
		m_consoleLabel = Components::Label::Create();
		m_consoleLabel->SetDropShadow(false);
		m_consoleLabel->SetFont(sD3DMgr.GetFont(L"Consolas", 16));
		m_consoleLabel->SetFormatFlags(DT_LEFT | DT_BOTTOM | DT_NOCLIP);
		m_consoleLabel->SetHeight(200.0f);

		m_windowBackground = Components::Rectangle::Create(0.0f, 0.0f);
		m_windowBackground->SetColor(0xC0000000);
		m_windowBackground->SetHeight(200.0f);

		m_controlGroup = Components::ItemsControl::Create();
		m_controlGroup->PushChild(m_windowBackground);
		m_controlGroup->PushChild(m_consoleLabel);
		m_controlGroup->SetVisibility(false);
		m_controlGroup->SetPosition(Utils::Vector2(0.0f, -200.0f));

		m_interface = std::make_shared<ComponentInterface>();
		m_interface->PushControl(m_controlGroup);
		sD3DMgr.PushInterface(m_interface);

		const auto updateDelegate = std::bind(&ConsoleWindow::UpdateWindow, this);
		sD3DMgr.OnDeviceResetEvent.connect(updateDelegate);
		sD3DMgr.OnDeviceChangedEvent.connect(updateDelegate);

		using namespace std::placeholders;
		sWndProc.OnMessageReceivedEvent.connect(std::bind(
			&ConsoleWindow::_onMessageReceived, this, _1, _2, _3));

		UpdateWindow();
	}

	void ConsoleWindow::UpdateWindow() {
		RECT screenDimensions = {0};
		const auto pRenderTarget = sD3DMgr.GetRenderTarget();

		if (m_isInitialized && pRenderTarget != nullptr &&
			pRenderTarget->GetSurfaceRect(&screenDimensions))
		{
			float screenWidth = static_cast<float>(
				screenDimensions.right - screenDimensions.left);

			m_consoleLabel->SetWidth(screenWidth);
			m_windowBackground->SetWidth(screenWidth);
		}
	}

	void ConsoleWindow::AddLine(const std::wstring &line) const {
		if (m_isInitialized) {
			std::wstring currentText = m_consoleLabel->GetText();
			m_consoleLabel->SetText(currentText + line + L"\r\n");

			size_t numLines = std::count(
				currentText.begin(),
				currentText.end(), L'\n');

			if (numLines >= 20)
				RemoveLine(0);
		}
	}

	void ConsoleWindow::RemoveLine(uint32 lineIndex) const {
		if (m_isInitialized) {
			std::wstring output, current;
			std::wistringstream in(m_consoleLabel->GetText());

			uint32 currentLine = 0;
			while (std::getline(in, current).good()) {
				if (currentLine++ != lineIndex)
					output += (current + L"\r\n");
			}

			m_consoleLabel->SetText(output);
		}
	}

	void ConsoleWindow::Show() const {
		if (m_isInitialized) {
			m_controlGroup->Show(150);
			m_controlGroup->MoveTo(150, Utils::Vector2(0.0f, 0.0f));
		}
	}

	void ConsoleWindow::Hide() const {
		if (m_isInitialized) {
			Utils::Vector2 hidePoint;
			hidePoint.y = -m_windowBackground->GetHeight();
			m_controlGroup->MoveTo(150, hidePoint);
			m_controlGroup->Hide(150);
		}
	}

	void ConsoleWindow::_onMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) const {
		switch (uMsg) {
		case WM_KEYDOWN:
			if (!sWndProc.LastMessageHandled)
				_onKeyDown(wParam);
			break;
		}
	}

	void ConsoleWindow::_onKeyDown(uint32 keyCode) const {
		switch (keyCode) {
		case VK_OEM_3: // The console trigger key ` (US)
			if (!IsShown())
				Show();
			else
				Hide();
			break;

		case VK_ESCAPE: // Escape closes the console
			if (IsShown())
				Hide();
			else
				return;
			break;

		default:
			return;
		}

		sWndProc.LastMessageHandled = true;
	}
}
