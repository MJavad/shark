/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
