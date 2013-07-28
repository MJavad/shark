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

namespace Utils {
	void SharkMemory::Initialize() {

	}

	inline DWORD_PTR SharkMemory::Allocate(uint32 size, DWORD dwProtection) {
		return reinterpret_cast<DWORD_PTR>(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, dwProtection));
	}

	inline bool SharkMemory::Free(DWORD_PTR address) {
		HRESULT result = VirtualFree(reinterpret_cast<LPVOID>(address), 0, MEM_RELEASE);
		return result != FALSE;
	}

	inline bool SharkMemory::SetMemoryProtection(DWORD_PTR address, uint32 size,
												 DWORD dwNewProtect, DWORD *pOldProtect) {
		HRESULT result = VirtualProtect(reinterpret_cast<LPVOID>(address), size, dwNewProtect, pOldProtect);
		return result != FALSE;
	}

	inline bool SharkMemory::WriteMemory_Safe(DWORD_PTR address, const ByteBuffer &bytes) {
		SIZE_T dwSize = bytes.size();
		SIZE_T bytesWritten = 0;
		HRESULT result = WriteProcessMemory(GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address), bytes.data(), dwSize, &bytesWritten);

		return (result != FALSE && bytesWritten == dwSize);
	}
	
	inline ByteBuffer SharkMemory::ReadMemory_Safe(DWORD_PTR address, uint32 size) {
		ByteBuffer buffer(size);
		SIZE_T bytesRead = 0;
		ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(address),
			buffer.data(), size, &bytesRead);

		buffer.resize(bytesRead);
		return buffer;
	}

	inline uint32 SharkMemory::GetInstructionSize(DWORD_PTR address) {
		return reinterpret_cast<uint32 (__cdecl*) (DWORD_PTR)>(&MLDE32[0]) (address);
	}

	bool SharkMemory::_dataCompare(const byte *pbData, const byte *pbMask, const char *pszMask) {
		for (; *pszMask != 0; ++pszMask, ++pbData, ++pbMask) {
			if (*pszMask == 'x' && *pbData != *pbMask)
				return false;
		}
		return true;
	}

	DWORD_PTR SharkMemory::FindMemoryPattern(DWORD_PTR dwStartAddress,
			DWORD dwLength, const byte *pbMask, const char *pszMask) {
		for (dwLength += dwStartAddress; dwStartAddress < dwLength; ++dwStartAddress) {
			if (_dataCompare(reinterpret_cast<const byte*>(dwStartAddress), pbMask, pszMask))
				return dwStartAddress;
		}

		return 0;
	}

	bool SharkMemory::DetourFunction(void **ppDelegate, void *pRedirect) {
		ThreadGrabber threadGrabber;
		if (!threadGrabber.update(GetCurrentProcessId())) {
			LOG_DEBUG("Detour transaction failed: Could not take thread snapshot!");
			return false;
		}

		bool success = false;
		void *function = *ppDelegate;
		DWORD_PTR functionOffset = reinterpret_cast<DWORD_PTR>(function);

		DWORD oldProtection = 0;
		if (SetMemoryProtection(functionOffset, 0x20, PAGE_EXECUTE_READWRITE, &oldProtection)) {
			uint32 trampolineSize = 0;
			uint32 jumpSize = sizeof(DWORD_PTR) + 1;
			while (trampolineSize < jumpSize)
				trampolineSize += GetInstructionSize(functionOffset + trampolineSize);

			// allocate a trampoline...
			byte *trampoline = m_trampolineHeap.allocate(trampolineSize + jumpSize);

			if (trampoline != nullptr) {
				memcpy(trampoline, function, trampolineSize);
				trampoline[trampolineSize] = 0xE9;

				DWORD_PTR trampolineOffset = reinterpret_cast<DWORD_PTR>(trampoline);
				GetMemory<DWORD_PTR>(trampolineOffset + trampolineSize + 1) = functionOffset - (trampolineOffset + jumpSize);

				SHookInformation hookInfo = {0};
				hookInfo.function = function;
				hookInfo.trampoline = trampoline;
				hookInfo.bytes.put_array(trampoline, trampolineSize);
				m_hooks[trampoline] = hookInfo;

				// hook chain support - relocate jump far, call far
				if (trampolineSize > sizeof(DWORD_PTR) &&
					(trampoline[0] == 0xE8 || trampoline[0] == 0xE9))
				{
					DWORD_PTR& relocate = GetMemory<DWORD_PTR>(trampolineOffset + 1);
					relocate += functionOffset;
					relocate -= trampolineOffset;
				}

				ByteBuffer jump(trampolineSize, 0xCC);
				jump << byte(0xE9);
				jump << reinterpret_cast<DWORD_PTR>(pRedirect) - (functionOffset + jumpSize);

				const auto &threads = threadGrabber.threads();
				for (const auto& thread: threads)
					_detourUpdateThread(thread, hookInfo);

				*ppDelegate = trampoline;
				success = WriteMemory_Safe(functionOffset, jump);

				for (const auto& thread: threads)
					_detourResumeThread(thread);
			}
			else
				LOG_DEBUG("Detour transaction failed: Trampoline heap alloc failed!");

			SetMemoryProtection(functionOffset, 0x20, oldProtection);
		}
		else
			LOG_DEBUG("Detour transaction failed: Cannot set code protection!");

		return success;
	}

	bool SharkMemory::RemoveDetour(void **ppDelegate) {
		auto itr = m_hooks.find(*ppDelegate);
		if (itr == m_hooks.end()) {
			LOG_DEBUG("Could not find a detour registered to this delegate.");
			return false;
		}

		DWORD oldProtection = 0;
		SHookInformation &hookInfo = itr->second;
		DWORD_PTR functionOffset = reinterpret_cast<DWORD_PTR>(hookInfo.function);
		SetMemoryProtection(functionOffset, hookInfo.bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtection);
		bool success = WriteMemory_Safe(functionOffset, hookInfo.bytes);
		SetMemoryProtection(functionOffset, hookInfo.bytes.size(), oldProtection);

		if (success) {
			*ppDelegate = hookInfo.function;
			m_trampolineHeap.free(hookInfo.trampoline);
			m_hooks.erase(itr);
		}
		else
			LOG_DEBUG("Detour transaction failed: Could not remove detour patch (Function: 0x%08X).", hookInfo.function);

		return success;
	}

	bool SharkMemory::RemoveAllDetours() {
		bool bSuccess = true;
		auto mHooks = m_hooks;
		for (auto& hook: mHooks) {
			void *trampolineDelegate = hook.second.trampoline;
			bSuccess &= RemoveDetour(&trampolineDelegate);
		}

		return bSuccess;
	}

	void SharkMemory::_detourResumeThread(const boost::shared_ptr<Thread> &thread) const {
		if (thread->id() != GetCurrentThreadId() &&
			thread->open(thread->access() | THREAD_SUSPEND_RESUME))
			thread->resume();
	}

	void SharkMemory::_detourUpdateThread(const boost::shared_ptr<Thread> &thread,
										  const SHookInformation &hookInfo) const
	{
		if (thread->id() != GetCurrentThreadId() &&
			thread->open(thread->access() | THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME)) {
			thread->suspend();
			CONTEXT ctx = {0};
			if (GetThreadContext(thread->handle(), &ctx) == FALSE)
				return;

			// do we need to relocate the threads instruction pointer to the trampoline?
			DWORD_PTR functionOffset = reinterpret_cast<DWORD_PTR>(hookInfo.function);
			if (ctx.Eip >= functionOffset &&
				ctx.Eip < (functionOffset + hookInfo.bytes.size()))
			{
				ctx.Eip -= functionOffset;
				ctx.Eip += reinterpret_cast<DWORD_PTR>(hookInfo.trampoline);
				SetThreadContext(thread->handle(), &ctx);
			}
		}
	}
}
