#pragma once
#include "Components/Label.h"
#include "Components/Rectangle.h"
#include "ComponentInterface.h"

namespace UI {
	class ConsoleWindow {
		SINGLETON_CLASS(ConsoleWindow) {}

	public:
		void Initialize();
		void UpdateWindow();
		void AddLine(const std::wstring &line) const;
		void RemoveLine(uint32 lineIndex) const;

	private:
		std::shared_ptr<ComponentInterface> m_interface;
		std::shared_ptr<Components::Label> m_windowContent;
		std::shared_ptr<Components::Rectangle> m_windowBackground;
	};
}

#define sConsoleWindow ::UI::ConsoleWindow::Instance()
