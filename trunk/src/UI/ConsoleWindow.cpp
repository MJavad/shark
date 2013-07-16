#include "Misc/stdafx.h"
#include "ConsoleWindow.h"
#include "Base/D3DManager.h"

namespace UI {
	void ConsoleWindow::Initialize() {
		m_windowContent = Components::Label::Create();
		m_windowContent->SetDropShadow(false);
		m_windowContent->SetFont(sD3DMgr.GetFont(L"Consolas", 16));
		m_windowContent->SetFormatFlags(DT_LEFT | DT_BOTTOM);
		m_windowContent->SetHeight(200.0f);

		m_windowBackground = Components::Rectangle::Create(0.0f, 0.0f);
		m_windowBackground->SetColor(0xC0000000);
		m_windowBackground->SetHeight(200.0f);

		m_interface = std::make_shared<ComponentInterface>();
		m_interface->PushControl(m_windowBackground);
		m_interface->PushControl(m_windowContent);
		sD3DMgr.PushInterface(m_interface);

		const auto updateDelegate = std::bind(&ConsoleWindow::UpdateWindow, this);
		sD3DMgr.OnDeviceResetEvent.connect(updateDelegate);
		sD3DMgr.OnDeviceChangedEvent.connect(updateDelegate);

		UpdateWindow();
	}

	void ConsoleWindow::UpdateWindow() {
		RECT screenDimensions = {0};
		const auto pRenderTarget = sD3DMgr.GetRenderTarget();
		if (pRenderTarget == nullptr ||
			!pRenderTarget->GetSurfaceRect(&screenDimensions))
			return;

		float screenWidth = static_cast<float>(
			screenDimensions.right - screenDimensions.left);
		m_windowContent->SetWidth(screenWidth);
		m_windowBackground->SetWidth(screenWidth);
	}

	void ConsoleWindow::AddLine(const std::wstring &line) const {
		if (m_windowContent != nullptr) {
			std::wstring currentText = m_windowContent->GetText();
			m_windowContent->SetText(currentText + line + L"\r\n");

			size_t numLines = std::count(
				currentText.begin(),
				currentText.end(), L'\n');

			if (numLines >= 20)
				RemoveLine(0);
		}
	}

	void ConsoleWindow::RemoveLine(uint32 lineIndex) const {
		if (m_windowContent != nullptr) {
			std::wstring output, current;
			std::wistringstream in(m_windowContent->GetText());

			uint32 currentLine = 0;
			while (std::getline(in, current).good()) {
				if (currentLine++ != lineIndex)
					output += (current + L"\r\n");
			}

			m_windowContent->SetText(output);
		}
	}
}
