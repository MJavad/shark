#include "Misc/stdafx.h"
#include "SharkMemory.h"

// This is MLDE32. It can be used to get any x86 instruction's length.
// Must be in .text or we will get an access violation on execution.
#pragma code_seg(".text")
__declspec(allocate(".text")) const byte MLDE32[] = {
	0x60, 0xFC, 0x33, 0xD2, 0x8B, 0x74, 0x24, 0x24,
	0x8B, 0xEC, 0x68, 0x1C, 0xF7, 0x97, 0x10, 0x68,
	0x80, 0x67, 0x1C, 0xF7, 0x68, 0x18, 0x97, 0x38,
	0x17, 0x68, 0x18, 0xB7, 0x1C, 0x10, 0x68, 0x17,
	0x2C, 0x30, 0x17, 0x68, 0x17, 0x30, 0x17, 0x18,
	0x68, 0x47, 0xF5, 0x15, 0xF7, 0x68, 0x48, 0x37,
	0x10, 0x4C, 0x68, 0xF7, 0xE7, 0x2C, 0x27, 0x68,
	0x87, 0x60, 0xAC, 0xF7, 0x68, 0x52, 0x1C, 0x12,
	0x1C, 0x68, 0x1C, 0x87, 0x10, 0x7C, 0x68, 0x1C,
	0x70, 0x1C, 0x20, 0x68, 0x2B, 0x60, 0x67, 0x47,
	0x68, 0x11, 0x10, 0x21, 0x20, 0x68, 0x25, 0x16,
	0x12, 0x40, 0x68, 0x22, 0x20, 0x87, 0x82, 0x68,
	0x20, 0x12, 0x20, 0x47, 0x68, 0x19, 0x14, 0x10,
	0x13, 0x68, 0x13, 0x10, 0x27, 0x18, 0x68, 0x60,
	0x82, 0x85, 0x28, 0x68, 0x45, 0x40, 0x12, 0x15,
	0x68, 0xC7, 0xA0, 0x16, 0x50, 0x68, 0x12, 0x18,
	0x19, 0x28, 0x68, 0x12, 0x18, 0x40, 0xF2, 0x68,
	0x27, 0x41, 0x15, 0x19, 0x68, 0x11, 0xF0, 0xF0,
	0x50, 0xB9, 0x10, 0x47, 0x12, 0x15, 0x51, 0x68,
	0x47, 0x12, 0x15, 0x11, 0x68, 0x12, 0x15, 0x11,
	0x10, 0x68, 0x15, 0x11, 0x10, 0x47, 0xB8, 0x15,
	0x20, 0x47, 0x12, 0x50, 0x50, 0x68, 0x10, 0x1A,
	0x47, 0x12, 0x80, 0xC1, 0x10, 0x51, 0x80, 0xE9,
	0x20, 0x51, 0x33, 0xC9, 0x49, 0x41, 0x8B, 0xFC,
	0xAC, 0x8A, 0xF8, 0x8A, 0x27, 0x47, 0xC0, 0xEC,
	0x04, 0x2A, 0xC4, 0x73, 0xF6, 0x8A, 0x47, 0xFF,
	0x24, 0x0F, 0x3C, 0x0C, 0x75, 0x03, 0x5A, 0xF7,
	0xD2, 0x42, 0x3C, 0x00, 0x74, 0x42, 0x3C, 0x01,
	0x74, 0xDB, 0x83, 0xC7, 0x51, 0x3C, 0x0A, 0x74,
	0xD7, 0x8B, 0x7D, 0x24, 0x42, 0x3C, 0x02, 0x74,
	0x2F, 0x3C, 0x07, 0x74, 0x33, 0x3C, 0x0B, 0x0F,
	0x84, 0x7E, 0x00, 0x00, 0x00, 0x42, 0x3C, 0x03,
	0x74, 0x1E, 0x3C, 0x08, 0x74, 0x22, 0x42, 0x3C,
	0x04, 0x74, 0x15, 0x42, 0x42, 0x60, 0xB0, 0x66,
	0xF2, 0xAE, 0x61, 0x75, 0x02, 0x4A, 0x4A, 0x3C,
	0x09, 0x74, 0x0D, 0x2C, 0x05, 0x74, 0x6C, 0x42,
	0x8B, 0xE5, 0x89, 0x54, 0x24, 0x1C, 0x61, 0xC3,
	0xAC, 0x8A, 0xE0, 0xC0, 0xE8, 0x07, 0x72, 0x12,
	0x74, 0x14, 0x80, 0xC2, 0x04, 0x60, 0xB0, 0x67,
	0xF2, 0xAE, 0x61, 0x75, 0x09, 0x80, 0xEA, 0x03,
	0xFE, 0xC8, 0x75, 0xDC, 0x42, 0x40, 0x80, 0xE4,
	0x07, 0x60, 0xB0, 0x67, 0xF2, 0xAE, 0x61, 0x74,
	0x13, 0x80, 0xFC, 0x04, 0x74, 0x17, 0x80, 0xFC,
	0x05, 0x75, 0xC5, 0xFE, 0xC8, 0x74, 0xC1, 0x80,
	0xC2, 0x04, 0xEB, 0xBC, 0x66, 0x3D, 0x00, 0x06,
	0x75, 0xB6, 0x42, 0xEB, 0xB2, 0x3C, 0x00, 0x75,
	0xAE, 0xAC, 0x24, 0x07, 0x2C, 0x05, 0x75, 0xA7,
	0x42, 0xEB, 0xE4, 0xF6, 0x06, 0x38, 0x75, 0xA8,
	0xB0, 0x08, 0xD0, 0xEF, 0x14, 0x00, 0xE9, 0x72,
	0xFF, 0xFF, 0xFF, 0x80, 0xEF, 0xA0, 0x80, 0xFF,
	0x04, 0x73, 0x82, 0x60, 0xB0, 0x67, 0xF2, 0xAE,
	0x61, 0x75, 0x02, 0x4A, 0x4A, 0x60, 0xB0, 0x66,
	0xF2, 0xAE, 0x61, 0x0F, 0x84, 0x76, 0xFF, 0xFF,
	0xFF, 0x0F, 0x85, 0x66, 0xFF, 0xFF, 0xFF
};

INIT_SINGLETON(Utils::SharkMemory);

namespace Utils {
	void SharkMemory::Initialize() {

	}

	inline DWORD_PTR SharkMemory::Allocate(uint32 uSize, DWORD dwProtection) {
		return reinterpret_cast<DWORD_PTR>(VirtualAlloc(nullptr, uSize, MEM_COMMIT | MEM_RESERVE, dwProtection));
	}

	inline bool SharkMemory::Free(DWORD_PTR dwAddress) {
		HRESULT hResult = VirtualFree(reinterpret_cast<LPVOID>(dwAddress), 0, MEM_RELEASE);
		return hResult != FALSE;
	}

	inline bool SharkMemory::SetMemoryProtection(DWORD_PTR dwAddress, uint32 uSize,
												 DWORD dwNewProtect, DWORD *pOldProtect) {
		HRESULT hResult = VirtualProtect(reinterpret_cast<LPVOID>(dwAddress), uSize, dwNewProtect, pOldProtect);
		return hResult != FALSE;
	}

	inline bool SharkMemory::WriteMemory_Safe(DWORD_PTR dwAddress, const ByteBuffer &bytes) {
		SIZE_T dwSize = bytes.size();
		SIZE_T dwBytesWritten = 0;
		HRESULT hResult = WriteProcessMemory(GetCurrentProcess(),
			reinterpret_cast<LPVOID>(dwAddress), bytes.data(), dwSize, &dwBytesWritten);

		return (hResult != FALSE && dwBytesWritten == dwSize);
	}
	
	inline ByteBuffer SharkMemory::ReadMemory_Safe(DWORD_PTR dwAddress, uint32 uSize) {
		ByteBuffer buffer(uSize);
		SIZE_T dwBytesRead = 0;
		ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(dwAddress),
			buffer.data(), uSize, &dwBytesRead);

		buffer.resize(dwBytesRead);
		return buffer;
	}

	inline uint32 SharkMemory::GetInstructionSize(DWORD_PTR dwAddress) {
		return reinterpret_cast<uint32 (__cdecl*) (DWORD_PTR)>(&MLDE32[0]) (dwAddress);
	}

	bool SharkMemory::DetourFunction(void **ppDelegate, const void *pRedirect) {
		ThreadGrabber threadGrabber;
		if (!threadGrabber.update(GetCurrentProcessId())) {
			LOG_DEBUG("Failed to get threads!");
			return false;
		}

		DWORD dwOldProtect = 0;
		void *pFunction = *ppDelegate;
		DWORD_PTR dwAddress = reinterpret_cast<DWORD_PTR>(pFunction);
		bool bSuccess = SetMemoryProtection(dwAddress, 0x20, PAGE_EXECUTE_READWRITE, &dwOldProtect);

		if (bSuccess) {
			uint32 uSize = 0;
			while (uSize < 5)
				uSize += GetInstructionSize(dwAddress + uSize);

			// allocate a trampoline...
			byte *pTrampoline = reinterpret_cast<byte*>(m_trampolineHeap.allocate(uSize + 5));
			DWORD_PTR dwTrampoline = reinterpret_cast<DWORD_PTR>(pTrampoline);
			bSuccess = (pTrampoline != nullptr);

			if (bSuccess) {
				memcpy(pTrampoline, pFunction, uSize);
				pTrampoline[uSize] = 0xE9;
				GetMemory<DWORD_PTR>(dwTrampoline + uSize + 1) = dwAddress - (dwTrampoline + 5);

				*ppDelegate = pTrampoline;
				SHookInformation hookInfo = {0};
				hookInfo.function = dwAddress;
				hookInfo.trampoline = pTrampoline;
				hookInfo.size = uSize;
				m_hooks[dwTrampoline] = hookInfo;

				ByteBuffer jump(uSize, 0xCC);
				jump << byte(0xE9);
				jump << reinterpret_cast<DWORD_PTR>(pRedirect) - (dwAddress + 5);

				const auto &lstThreads = threadGrabber.threads();
				std::list<std::shared_ptr<Thread>> lstCriticalThreads(lstThreads);
				_detourSuspendThreads(lstThreads);

				do {
					lstCriticalThreads = _threadExecutingInstruction(
						lstCriticalThreads, dwAddress, uSize);

					for (const auto& thread: lstCriticalThreads) {
						if (thread->open(thread->access() | THREAD_SUSPEND_RESUME)) {
							thread->resume();
							Sleep(rand() % 2); // give him some time to move along... :D
							thread->suspend();
						}
					}
				}
				while (!lstCriticalThreads.empty());

				WriteMemory_Safe(dwAddress, jump);
				_detourResumeThreads(lstThreads);
			}
			else
				LOG_DEBUG("Trampoline heap alloc failed!");

			SetMemoryProtection(dwAddress, 0x20, dwOldProtect);
		}
		else
			LOG_DEBUG("Cannot set code protection!");

		return bSuccess;
	}

	bool SharkMemory::RemoveDetour(void **ppDelegate) {
		DWORD_PTR dwTrampoline = reinterpret_cast<DWORD_PTR>(*ppDelegate);
		auto itr = m_hooks.find(dwTrampoline);
		if (itr == m_hooks.end())
			return false;

		SHookInformation &hookInfo = itr->second;
		ByteBuffer bytes(hookInfo.size);
		bytes.put_array(hookInfo.trampoline, hookInfo.size);

		DWORD dwOldProtect = 0;
		SetMemoryProtection(hookInfo.function, hookInfo.size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		bool bSuccess = WriteMemory_Safe(hookInfo.function, bytes);
		SetMemoryProtection(hookInfo.function, hookInfo.size, dwOldProtect);

		if (bSuccess) {
			*ppDelegate = reinterpret_cast<void*>(hookInfo.function);
			m_trampolineHeap.free(hookInfo.trampoline);
			m_hooks.erase(itr);
		}

		return bSuccess;
	}

	bool SharkMemory::RemoveAllDetours() {
		bool bSuccess = true;
		auto mHooks = m_hooks;
		for (auto& hook: mHooks) {
			void *pTrampoline = hook.second.trampoline;
			bSuccess &= RemoveDetour(&pTrampoline);
		}

		return bSuccess;
	}

	void SharkMemory::_detourSuspendThreads(const std::list<std::shared_ptr<Thread>> &threads) const {
		uint32 curThreadId = GetCurrentThreadId();
		for (const auto& thread: threads) {
			if (thread->id() != curThreadId &&
				thread->open(thread->access() | THREAD_SUSPEND_RESUME))
				thread->suspend();
		}
	}

	void SharkMemory::_detourResumeThreads(const std::list<std::shared_ptr<Thread>> &threads) const {
		uint32 curThreadId = GetCurrentThreadId();
		for (const auto& thread: threads) {
			if (thread->id() != curThreadId &&
				thread->open(thread->access() | THREAD_SUSPEND_RESUME))
				thread->resume();
		}
	}

	std::list<std::shared_ptr<Thread>> SharkMemory::_threadExecutingInstruction(
							const std::list<std::shared_ptr<Thread>> &threads,
							DWORD_PTR dwAddress, DWORD_PTR dwLength) const {
		uint32 curThreadId = GetCurrentThreadId();
		std::list<std::shared_ptr<Thread>> lstThreads;
		for (const auto& thread: threads) {
			if (thread->id() != curThreadId &&
				thread->open(thread->access() | THREAD_GET_CONTEXT)) {
				CONTEXT ctx = {0};
				if (GetThreadContext(thread->handle(), &ctx) == FALSE)
					continue;

				if (ctx.Eip >= dwAddress && ctx.Eip < dwAddress + dwLength)
					lstThreads.push_back(thread);
			}
		}

		return lstThreads;
	}
}
