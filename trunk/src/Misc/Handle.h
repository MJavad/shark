#pragma once

namespace Utils
{
	class Handle
	{
	public:
		Handle() : m_handle(nullptr) {}
		Handle(HANDLE hObject) : m_handle(hObject) {}

		Handle(const Handle &other) : m_handle(nullptr) {
			HANDLE hProcess = GetCurrentProcess();
			BOOL bSuccess = DuplicateHandle(hProcess, other,
				hProcess, &m_handle, 0, FALSE, DUPLICATE_SAME_ACCESS);
			CloseHandle(hProcess);

			if (bSuccess == FALSE)
				m_handle = other.m_handle;
		}

		Handle(Handle &&other) :
			m_handle(other.m_handle) {
			other.m_handle = nullptr;
		}

		~Handle() {
			if (valid())
				CloseHandle(m_handle);
		}

		Handle& operator=(HANDLE hObject) {
			close(); // close previous handle...
			m_handle = hObject;
			return *this;
		}

		Handle& operator=(const Handle &other) {
			if (this != std::addressof(other)) {
				close(); // close previous handle...
				HANDLE hProcess = GetCurrentProcess();
				BOOL bSuccess = DuplicateHandle(hProcess, other,
					hProcess, &m_handle, 0, FALSE, DUPLICATE_SAME_ACCESS);
				CloseHandle(hProcess);

				if (bSuccess == FALSE)
					m_handle = other.m_handle;
			}
			
			return *this;
		}

		Handle& operator=(Handle &&other) {
			close(); // close previous handle...
			m_handle = other.m_handle;
			other.m_handle = nullptr;
			return *this;
		}

		HANDLE* operator&() {
			return &m_handle;
		}

		const HANDLE* operator&() const {
			return &m_handle;
		}

		operator HANDLE() const {
			return m_handle;
		}

		operator bool() const {
			return m_handle != nullptr;
		}

		bool valid() const {
			return m_handle != nullptr &&
				m_handle != INVALID_HANDLE_VALUE;
		}

		bool is_null() const {
			return m_handle == nullptr;
		}

		void close() {
			if (valid())
				CloseHandle(m_handle);
			m_handle = nullptr;
		}

	protected:
		HANDLE m_handle;
	};
}
