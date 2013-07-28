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

#include "Misc/stdafx.h"
#include "ClipStack.h"
#include "Base/D3DManager.h"

namespace UI {
	void ClipStack::Apply() const {
		if (sD3DMgr.IsCreated()) 
			sD3DMgr.GetRenderTarget()->SetClippingArea(Top());
	}

	void ClipStack::Push(const RECT &rect) {
		if (m_stack.empty())
			return m_stack.push(rect);

		RECT newTop = {0};
		RECT curTop = m_stack.top();
		if (IntersectRect(&newTop, &curTop, &rect) == FALSE)
			newTop = curTop;

		m_stack.push(newTop);
	}

	const RECT* ClipStack::Top() const {
		if (!m_stack.empty())
			return &m_stack.top();
		return nullptr;
	}
}
