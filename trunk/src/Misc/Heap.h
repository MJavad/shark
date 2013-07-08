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
