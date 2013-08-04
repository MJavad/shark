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
#include "Components/IComponent.h"
#include "Components/Frame.h"

namespace UI {
	class GUIManager {
		SINGLETON_CLASS(GUIManager) {}

	public:
		void Initialize();

		boost::shared_ptr<Components::Frame> CreateBasicFrame(
			std::wstring windowTitle, float width, float height, const D3DXCOLOR &color) const;

		boost::shared_ptr<ComponentInterface> GetInterface() const { return m_interface; }
		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		boost::shared_ptr<ComponentInterface> m_interface;

		boost::shared_ptr<Components::Frame> _createBasicLuaFrame(
			const std::string &windowTitle, float width, float height, const D3DXCOLOR &color) const;
	};
}

#define sGUIMgr ::UI::GUIManager::Instance()
