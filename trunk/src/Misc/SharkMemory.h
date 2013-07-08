#pragma once
#include "Singleton.h"
#include "ThreadGrabber.h"

namespace Utils {
	struct SHookInformation {
		void* function;
		byte* trampoline;
		ByteBuffer bytes;
	};

	class SharkMemory {
		SINGLETON_CLASS(SharkMemory) :
			m_trampolineHeap(HEAP_CREATE_ENABLE_EXECUTE) {}

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
		bool DetourFunction(void **ppDelegate, void *pRedirect);
		bool RemoveDetour(void **ppDelegate);
		bool RemoveAllDetours();

	protected:
		Heap m_trampolineHeap;
		std::map<void*, SHookInformation> m_hooks;

		static bool _dataCompare(const byte *pbData, const byte *pbMask, const char *pszMask);
		void _detourResumeThread(const std::shared_ptr<Thread> &thread) const;
		
		void _detourUpdateThread(const std::shared_ptr<Thread> &thread,
								 const SHookInformation &hookInfo) const;
	};
}

#define sMemory ::Utils::SharkMemory::Instance()
