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

namespace Utils
{
	class Heap
	{
	public:
		Heap(DWORD flOptions = 0,
			 SIZE_T dwInitialSize = 0,
			 SIZE_T dwMaximumSize = 0) {
			m_heap = HeapCreate(flOptions, dwInitialSize, dwMaximumSize);
			if (m_heap == nullptr)
				throw std::runtime_error("HeapCreate failed!");
		}

		~Heap() {
			HeapDestroy(m_heap);
		}

		byte* allocate(size_t dwSize) const {
			return reinterpret_cast<byte*>(HeapAlloc(m_heap, 0, dwSize));
		}

		bool free(byte *pMemory) const {
			return HeapFree(m_heap, 0, pMemory) != FALSE;
		}

	protected:
		HANDLE m_heap;

		Heap(const Heap&);
		Heap& operator=(const Heap&);
	};
}
