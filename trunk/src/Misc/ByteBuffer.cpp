#include "Misc/stdafx.h"
#include "ByteBuffer.h"

namespace Utils {
	void ByteBuffer::put_array(const byte *pbData, size_t uSize, uint32 uPos) {
		if (m_storage.size() < uPos + uSize)
			m_storage.resize(uPos + uSize);
		memcpy(&m_storage[uPos], pbData, uSize);
	}

	void ByteBuffer::erase(uint32 uPos, size_t uSize) {
		auto pos = m_storage.begin() + uPos;
		m_storage.erase(pos, pos + uSize);
		if (m_rpos > uPos + uSize)
			m_rpos -= uSize;
		if (m_wpos > uPos + uSize)
			m_wpos -= uSize;
	}
}
