#pragma once
#include "Components/Label.h"
#include "Components/Rectangle.h"
#include "Components/ItemsControl.h"
#include "ComponentInterface.h"

namespace UI {
	class ConsoleWindow {
		SINGLETON_CLASS(ConsoleWindow) : m_isInitialized(false) {}

	public:
		void Initialize();
		void UpdateWindow();
		void AddLine(const std::wstring &line) const;
		void RemoveLine(uint32 lineIndex) const;

		void Show() const;
		void Hide() const;

		bool IsShown() const {
			return m_controlGroup->GetVisibility();
		}

	private:
		bool m_isInitialized;
		boost::shared_ptr<ComponentInterface> m_interface;
		boost::shared_ptr<Components::ItemsControl> m_controlGroup;
		boost::shared_ptr<Components::Label> m_consoleLabel;
		boost::shared_ptr<Components::Rectangle> m_windowBackground;

		void _onMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		void _onKeyDown(UINT keyCode) const;
	};
}

#define sConsoleWindow ::UI::ConsoleWindow::Instance()
