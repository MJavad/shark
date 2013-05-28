#pragma once
#include "Singleton.h"
#include "ThreadGrabber.h"

namespace Utils {
	struct SHookInformation {
		DWORD_PTR function;
		byte* trampoline;
		uint32 size;
	};

	class SharkMemory : public Singleton
	{
		SINGLETON_OBJECT
		SharkMemory() : m_trampolineHeap(HEAP_CREATE_ENABLE_EXECUTE) {}

	public:
		~SharkMemory() {
			RemoveAllDetours();
		}

		void Initialize();
		static DWORD_PTR Allocate(uint32 uSize, DWORD dwProtection = PAGE_EXECUTE_READWRITE);
		static bool Free(DWORD_PTR dwAddress);

		static bool SetMemoryProtection(DWORD_PTR dwAddress, uint32 uSize,
			DWORD dwNewProtect, DWORD *pOldProtect = nullptr);

		static bool WriteMemory_Safe(DWORD_PTR dwAddress, const ByteBuffer &bytes);
		static ByteBuffer ReadMemory_Safe(DWORD_PTR dwAddress, uint32 uSize);

		template <typename T>
		static T& GetMemory(DWORD_PTR dwAddress) {
			return *reinterpret_cast<T*>(dwAddress);
		}

		static DWORD_PTR FindMemoryPattern(DWORD_PTR dwStartAddress,
			DWORD dwLength, const byte *pbMask, const char *pszMask);

		static uint32 GetInstructionSize(DWORD_PTR dwAddress);
		bool DetourFunction(void **ppDelegate, const void *pRedirect);
		bool RemoveDetour(void **ppDelegate);
		bool RemoveAllDetours();

	protected:
		Heap m_trampolineHeap;
		std::map<DWORD_PTR, SHookInformation> m_hooks;

		static bool _dataCompare(const byte *pbData, const byte *pbMask, const char *pszMask);
		void _detourSuspendThreads(const std::list<std::shared_ptr<Thread>> &threads) const;
		void _detourResumeThreads(const std::list<std::shared_ptr<Thread>> &threads) const;
		
		std::list<std::shared_ptr<Thread>> _threadExecutingInstruction(
					const std::list<std::shared_ptr<Thread>> &threads,
					DWORD_PTR dwAddress, DWORD_PTR dwLength) const;
	};
}

#define sMemory GET_INSTANCE(Utils::SharkMemory)
