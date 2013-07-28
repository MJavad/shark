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
#include "IComponent.h"

namespace UI {
namespace Components {
	class IRectComponent abstract : public virtual IComponent
	{
	public:
		virtual float GetWidth() const {
			return m_width;
		}

		virtual void SetWidth(float width) {
			m_width = width;
		}

		virtual float GetHeight() const {
			return m_height;
		}

		virtual void SetHeight(float height) {
			m_height = height;
		}

		bool PtInBoundingRect(const POINT &vPoint) const {
			RECT dispRect = GetDisplayRect();
			return PtInRect(&dispRect, vPoint) != FALSE;
		}

		RECT GetFullRect() const;
		RECT GetDisplayRect() const;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IRectComponent() : m_width(0), m_height(0) {}

	private:
		float m_width, m_height;
	};
}
}
