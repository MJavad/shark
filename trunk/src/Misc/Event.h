#pragma once
#include "TokenManager.h"

namespace Utils
{
	struct Minimum {
		template <typename T, typename InputIterator>
		static T Comp(InputIterator first, InputIterator last) {
			if (first == last)
				return T();

			T min_value = *first++;
			while (first != last) {
				if (min_value > *first)
					min_value = *first;
				++first;
			}

			return min_value;
		}
	};

	struct Maximum {
		template <typename T, typename InputIterator>
		static T Comp(InputIterator first, InputIterator last) {
			if (first == last)
				return T();

			T max_value = *first++;
			while (first != last) {
				if (max_value < *first)
					max_value = *first;
				++first;
			}

			return max_value;
		}
	};

	template <typename FuncT>
	struct SEventDelegate {
		uint64 token;
		std::function<FuncT> callback;
	};

	template <typename FuncT>
	class Event {};

#ifdef VARIADIC_TEMPLATES_SUPPORTED
	// Threadsafe
	template <typename R, typename... Args>
	class Event<R (Args...)>
	{
	public:
		typedef R fnconv_t(Args...);
		Event() : m_tokenMgr(), m_mutex(), m_delegates() {}

		Event(std::function<fnconv_t> func) :
			m_mutex(), m_tokenMgr(), m_delegates() {
			connect(std::move(func));
		}

		Event(Event<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_mutex(std::move(other.m_mutex)),
			m_delegates(std::move(other.m_delegates)) {}

		Event<fnconv_t>& operator=(Event<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
			m_mutex = std::move(other.m_mutex);
			m_delegates = std::move(other.m_delegates);
			return *this;
		}

		R operator()(const Args&... args) const {
			return _invoke_impl<R, Maximum>(args...);
		}

		template <typename CompT>
		R invoke(const Args&... args) const {
			return _invoke_impl<R, CompT>(args...);
		}

		std::vector<R> invoke_getall(const Args&... args) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<R> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(args...));
			return results;
		}

		uint32 size() const {
			LockGuard g(m_mutex);
			return m_delegates.size();
		}

		SEventDelegate<fnconv_t> operator+=(std::function<fnconv_t> func) {
			return connect(std::move(func));
		}

		Event<fnconv_t>& operator-=(const SEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SEventDelegate<fnconv_t> connect(std::function<fnconv_t> func) {
			LockGuard g(m_mutex);
			SEventDelegate<fnconv_t> deleg = { m_tokenMgr.get(), std::move(func) };
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove(const uint64 &token) {
			LockGuard g(m_mutex);
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_mutex.acquire();
			m_delegates.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

	protected:
		TokenManager m_tokenMgr;
		mutable Mutex m_mutex;
		std::map<uint64, SEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const Args&... args) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			for (auto& pair: mDelegates)
				pair.second.callback(args...);
		}

		template <typename T, typename CompT>
		typename std::enable_if<!std::is_same<void, T>::value, T>::type
			_invoke_impl(const Args&... args) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<T> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(args...));
			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};
#else


//////////// EVENT P0 //////////////


	// Threadsafe
	template <typename R>
	class Event<R (void)>
	{
	public:
		typedef R fnconv_t(void);
		Event() : m_tokenMgr(), m_mutex(), m_delegates() {}

		Event(std::function<fnconv_t> func) :
			m_mutex(), m_tokenMgr(), m_delegates() {
			connect(std::move(func));
		}

		Event(Event<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_mutex(std::move(other.m_mutex)),
			m_delegates(std::move(other.m_delegates)) {}

		Event<fnconv_t>& operator=(Event<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
			m_mutex = std::move(other.m_mutex);
			m_delegates = std::move(other.m_delegates);
			return *this;
		}

		R operator()() const {
			return _invoke_impl<R, Maximum>();
		}

		template <typename CompT>
		R invoke() const {
			return _invoke_impl<R, CompT>();
		}

		std::vector<R> invoke_getall() const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<R> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback());
			return results;
		}

		uint32 size() const {
			LockGuard g(m_mutex);
			return m_delegates.size();
		}

		SEventDelegate<fnconv_t> operator+=(std::function<fnconv_t> func) {
			return connect(std::move(func));
		}

		Event<fnconv_t>& operator-=(const SEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SEventDelegate<fnconv_t> connect(std::function<fnconv_t> func) {
			LockGuard g(m_mutex);
			SEventDelegate<fnconv_t> deleg = { m_tokenMgr.get(), std::move(func) };
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove(const uint64 &token) {
			LockGuard g(m_mutex);
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_mutex.acquire();
			m_delegates.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

	protected:
		TokenManager m_tokenMgr;
		mutable Mutex m_mutex;
		std::map<uint64, SEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl() const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			for (auto& pair: mDelegates)
				pair.second.callback();
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl() const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<T> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback());
			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P1 //////////////


	// Threadsafe
	template <typename R, typename A0>
	class Event<R (A0)>
	{
	public:
		typedef R fnconv_t(A0);
		Event() : m_tokenMgr(), m_mutex(), m_delegates() {}

		Event(std::function<fnconv_t> func) :
			m_mutex(), m_tokenMgr(), m_delegates() {
			connect(std::move(func));
		}

		Event(Event<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_mutex(std::move(other.m_mutex)),
			m_delegates(std::move(other.m_delegates)) {}

		Event<fnconv_t>& operator=(Event<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
			m_mutex = std::move(other.m_mutex);
			m_delegates = std::move(other.m_delegates);
			return *this;
		}

		R operator()(const A0 &a0) const {
			return _invoke_impl<R, Maximum>(a0);
		}

		template <typename CompT>
		R invoke(const A0 &a0) const {
			return _invoke_impl<R, CompT>(a0);
		}

		std::vector<R> invoke_getall(const A0 &a0) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<R> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0));
			return results;
		}

		uint32 size() const {
			LockGuard g(m_mutex);
			return m_delegates.size();
		}

		SEventDelegate<fnconv_t> operator+=(std::function<fnconv_t> func) {
			return connect(std::move(func));
		}

		Event<fnconv_t>& operator-=(const SEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SEventDelegate<fnconv_t> connect(std::function<fnconv_t> func) {
			LockGuard g(m_mutex);
			SEventDelegate<fnconv_t> deleg = { m_tokenMgr.get(), std::move(func) };
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove(const uint64 &token) {
			LockGuard g(m_mutex);
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_mutex.acquire();
			m_delegates.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

	protected:
		TokenManager m_tokenMgr;
		mutable Mutex m_mutex;
		std::map<uint64, SEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			for (auto& pair: mDelegates)
				pair.second.callback(a0);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<T> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0));
			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P2 //////////////


	// Threadsafe
	template <typename R, typename A0, typename A1>
	class Event<R (A0, A1)>
	{
	public:
		typedef R fnconv_t(A0, A1);
		Event() : m_tokenMgr(), m_mutex(), m_delegates() {}

		Event(std::function<fnconv_t> func) :
			m_mutex(), m_tokenMgr(), m_delegates() {
			connect(std::move(func));
		}

		Event(Event<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_mutex(std::move(other.m_mutex)),
			m_delegates(std::move(other.m_delegates)) {}

		Event<fnconv_t>& operator=(Event<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
			m_mutex = std::move(other.m_mutex);
			m_delegates = std::move(other.m_delegates);
			return *this;
		}

		R operator()(const A0 &a0, const A1 &a1) const {
			return _invoke_impl<R, Maximum>(a0, a1);
		}

		template <typename CompT>
		R invoke(const A0 &a0, const A1 &a1) const {
			return _invoke_impl<R, CompT>(a0, a1);
		}

		std::vector<R> invoke_getall(const A0 &a0, const A1 &a1) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<R> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0, a1));
			return results;
		}

		uint32 size() const {
			LockGuard g(m_mutex);
			return m_delegates.size();
		}

		SEventDelegate<fnconv_t> operator+=(std::function<fnconv_t> func) {
			return connect(std::move(func));
		}

		Event<fnconv_t>& operator-=(const SEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SEventDelegate<fnconv_t> connect(std::function<fnconv_t> func) {
			LockGuard g(m_mutex);
			SEventDelegate<fnconv_t> deleg = { m_tokenMgr.get(), std::move(func) };
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove(const uint64 &token) {
			LockGuard g(m_mutex);
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_mutex.acquire();
			m_delegates.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

	protected:
		TokenManager m_tokenMgr;
		mutable Mutex m_mutex;
		std::map<uint64, SEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			for (auto& pair: mDelegates)
				pair.second.callback(a0, a1);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<T> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0, a1));
			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P3 //////////////


	// Threadsafe
	template <typename R, typename A0, typename A1, typename A2>
	class Event<R (A0, A1, A2)>
	{
	public:
		typedef R fnconv_t(A0, A1, A2);
		Event() : m_tokenMgr(), m_mutex(), m_delegates() {}

		Event(std::function<fnconv_t> func) :
			m_mutex(), m_tokenMgr(), m_delegates() {
			connect(std::move(func));
		}

		Event(Event<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_mutex(std::move(other.m_mutex)),
			m_delegates(std::move(other.m_delegates)) {}

		Event<fnconv_t>& operator=(Event<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
			m_mutex = std::move(other.m_mutex);
			m_delegates = std::move(other.m_delegates);
			return *this;
		}

		R operator()(const A0 &a0, const A1 &a1, const A2 &a2) const {
			return _invoke_impl<R, Maximum>(a0, a1, a2);
		}

		template <typename CompT>
		R invoke(const A0 &a0, const A1 &a1, const A2 &a2) const {
			return _invoke_impl<R, CompT>(a0, a1, a2);
		}

		std::vector<R> invoke_getall(const A0 &a0, const A1 &a1, const A2 &a2) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<R> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0, a1, a2));
			return results;
		}

		uint32 size() const {
			LockGuard g(m_mutex);
			return m_delegates.size();
		}

		SEventDelegate<fnconv_t> operator+=(std::function<fnconv_t> func) {
			return connect(std::move(func));
		}

		Event<fnconv_t>& operator-=(const SEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SEventDelegate<fnconv_t> connect(std::function<fnconv_t> func) {
			LockGuard g(m_mutex);
			SEventDelegate<fnconv_t> deleg = { m_tokenMgr.get(), std::move(func) };
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove(const uint64 &token) {
			LockGuard g(m_mutex);
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_mutex.acquire();
			m_delegates.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

	protected:
		TokenManager m_tokenMgr;
		mutable Mutex m_mutex;
		std::map<uint64, SEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			for (auto& pair: mDelegates)
				pair.second.callback(a0, a1, a2);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<T> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0, a1, a2));
			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P4 //////////////


	// Threadsafe
	template <typename R, typename A0, typename A1, typename A2, typename A3>
	class Event<R (A0, A1, A2, A3)>
	{
	public:
		typedef R fnconv_t(A0, A1, A2, A3);
		Event() : m_tokenMgr(), m_mutex(), m_delegates() {}

		Event(std::function<fnconv_t> func) :
			m_mutex(), m_tokenMgr(), m_delegates() {
			connect(std::move(func));
		}

		Event(Event<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_mutex(std::move(other.m_mutex)),
			m_delegates(std::move(other.m_delegates)) {}

		Event<fnconv_t>& operator=(Event<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
			m_mutex = std::move(other.m_mutex);
			m_delegates = std::move(other.m_delegates);
			return *this;
		}

		R operator()(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			return _invoke_impl<R, Maximum>(a0, a1, a2, a3);
		}

		template <typename CompT>
		R invoke(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			return _invoke_impl<R, CompT>(a0, a1, a2, a3);
		}

		std::vector<R> invoke_getall(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<R> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0, a1, a2, a3));
			return results;
		}

		uint32 size() const {
			LockGuard g(m_mutex);
			return m_delegates.size();
		}

		SEventDelegate<fnconv_t> operator+=(std::function<fnconv_t> func) {
			return connect(std::move(func));
		}

		Event<fnconv_t>& operator-=(const SEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SEventDelegate<fnconv_t> connect(std::function<fnconv_t> func) {
			LockGuard g(m_mutex);
			SEventDelegate<fnconv_t> deleg = { m_tokenMgr.get(), std::move(func) };
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove(const uint64 &token) {
			LockGuard g(m_mutex);
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_mutex.acquire();
			m_delegates.clear();
			m_tokenMgr.reset();
			m_mutex.release();
		}

	protected:
		TokenManager m_tokenMgr;
		mutable Mutex m_mutex;
		std::map<uint64, SEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			for (auto& pair: mDelegates)
				pair.second.callback(a0, a1, a2, a3);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			m_mutex.acquire();
			auto mDelegates = m_delegates;
			m_mutex.release();

			std::vector<T> results;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(pair.second.callback(a0, a1, a2, a3));
			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};
#endif
}
