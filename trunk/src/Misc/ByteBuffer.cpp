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
#include "ByteBuffer.h"

namespace Utils {
	void ByteBuffer::put_array(const byte *data, size_t size, uint32 pos) {
		if (m_storage.size() < pos + size)
			m_storage.resize(pos + size);
		memcpy(&m_storage[pos], data, size);
	}

	void ByteBuffer::erase(uint32 pos, size_t size) {
		auto posIter = m_storage.begin() + pos;
		m_storage.erase(posIter, posIter + size);
		if (m_rpos > pos + size)
			m_rpos -= size;
		if (m_wpos > pos + size)
			m_wpos -= size;
	}
}
