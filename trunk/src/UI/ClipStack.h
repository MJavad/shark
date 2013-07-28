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

namespace UI {
	class ClipStack
	{
	public:
		ClipStack() : m_stack() {}

		void Apply() const;
		void Push(const RECT &rect);
		void Pop() { m_stack.pop(); }
		bool IsEmpty() const { return m_stack.empty(); }
		size_t Size() const { return m_stack.size(); }
		const RECT* Top() const;

		bool PtInClipArea(const POINT &pt) const {
			return m_stack.empty() || (PtInRect(&m_stack.top(), pt) != FALSE);
		}

		void SetRect(const RECT &rect, const boost::function<void ()> &func) {
			Push(rect);
			Apply();
			func();
			Pop();
			Apply();
		}

	protected:
		std::stack<RECT> m_stack;
	};
}
