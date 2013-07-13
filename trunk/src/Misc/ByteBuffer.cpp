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
