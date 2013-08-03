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

#pragma once
#include "Event.h"
#include "Base/LuaHandler.h"

namespace Utils
{
	template <typename FuncT>
	struct SLuaEventDelegate {
		uint64 token;
		luabind::object function;
	};

	template <typename FuncT>
	class LuaEvent {};

#ifdef VARIADIC_TEMPLATES_SUPPORTED
	// Threadsafe
	template <typename R, typename... Args>
	class LuaEvent<R (Args...)>
	{
	public:
		typedef R fnconv_t(Args...);
		LuaEvent() : m_tokenMgr(), m_delegates() {}

		LuaEvent(LuaEvent<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_delegates(std::move(other.m_delegates)) {}

		LuaEvent<fnconv_t>& operator=(LuaEvent<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
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
			std::vector<R> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(sLuaHandler.ExecuteFunction<R>(pair.second.function, args...));
			return results;
		}

		uint32 size() const {
			return m_delegates.size();
		}

		LuaEvent<fnconv_t>& operator-=(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SLuaEventDelegate<fnconv_t> connect_lua(const luabind::object &o) {
			if (luabind::type(o) != LUA_TFUNCTION)
				throw std::runtime_error("Invalid argument: Object is not a function!");

			SLuaEventDelegate<fnconv_t> deleg;
			deleg.token = m_tokenMgr.get();
			deleg.function = o;
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove_deleg(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
		}

		void remove(const uint64 &token) {
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_delegates.clear();
			m_tokenMgr.reset();
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState) {
			luabind::module(luaState.get()) [
				luabind::class_<SLuaEventDelegate<fnconv_t>>(),
				luabind::class_<LuaEvent<fnconv_t>>()
					.def("AddCallback", &LuaEvent<fnconv_t>::connect_lua)
					.def("RemoveCallback", &LuaEvent<fnconv_t>::remove_deleg)
					.def("ClearAll", &LuaEvent<fnconv_t>::clear)
					.property("numCallbacks", &LuaEvent<fnconv_t>::size)
			];
		}

	private:
		TokenManager m_tokenMgr;
		std::map<uint64, SLuaEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const Args&... args) const {
			auto mDelegates = m_delegates;
			for (auto& pair: mDelegates)
				sLuaHandler.ExecuteFunction<void>(pair.second.function, args...);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const Args&... args) const {
			std::vector<T> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(sLuaHandler.ExecuteFunction<T>(pair.second.function, args...));

			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};
#else


//////////// EVENT P0 //////////////


	// Threadsafe
	template <typename R>
	class LuaEvent<R (void)>
	{
	public:
		typedef R fnconv_t(void);
		LuaEvent() : m_tokenMgr(), m_delegates() {}

		LuaEvent(LuaEvent<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_delegates(std::move(other.m_delegates)) {}

		LuaEvent<fnconv_t>& operator=(LuaEvent<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
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
			std::vector<R> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(sLuaHandler.ExecuteFunction<R>(pair.second.function));
			return results;
		}

		uint32 size() const {
			return m_delegates.size();
		}

		LuaEvent<fnconv_t>& operator-=(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SLuaEventDelegate<fnconv_t> connect_lua(const luabind::object &o) {
			if (luabind::type(o) != LUA_TFUNCTION)
				throw std::runtime_error("Invalid argument: Object is not a function!");

			SLuaEventDelegate<fnconv_t> deleg;
			deleg.token = m_tokenMgr.get();
			deleg.function = o;
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove_deleg(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
		}

		void remove(const uint64 &token) {
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_delegates.clear();
			m_tokenMgr.reset();
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState) {
			luabind::module(luaState.get()) [
				luabind::class_<SLuaEventDelegate<fnconv_t>>(),
				luabind::class_<LuaEvent<fnconv_t>>()
					.def("AddCallback", &LuaEvent<fnconv_t>::connect_lua)
					.def("RemoveCallback", &LuaEvent<fnconv_t>::remove_deleg)
					.def("ClearAll", &LuaEvent<fnconv_t>::clear)
					.property("numCallbacks", &LuaEvent<fnconv_t>::size)
			];
		}

	private:
		TokenManager m_tokenMgr;
		std::map<uint64, SLuaEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl() const {
			auto mDelegates = m_delegates;
			for (auto& pair: mDelegates)
				sLuaHandler.ExecuteFunction<void>(pair.second.function);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl() const {
			std::vector<T> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(sLuaHandler.ExecuteFunction<T>(pair.second.function));

			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P1 //////////////


	// Threadsafe
	template <typename R, typename A0>
	class LuaEvent<R (A0)>
	{
	public:
		typedef R fnconv_t(A0);
		LuaEvent() : m_tokenMgr(), m_delegates() {}

		LuaEvent(LuaEvent<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_delegates(std::move(other.m_delegates)) {}

		LuaEvent<fnconv_t>& operator=(LuaEvent<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
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
			std::vector<R> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			//for (auto& pair: mDelegates)
			//	results.push_back(sLuaHandler.ExecuteFunction<R>(pair.second.function, a0));
			return results;
		}

		uint32 size() const {
			return m_delegates.size();
		}

		LuaEvent<fnconv_t>& operator-=(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SLuaEventDelegate<fnconv_t> connect_lua(const luabind::object &o) {
			if (luabind::type(o) != LUA_TFUNCTION)
				throw std::runtime_error("Invalid argument: Object is not a function!");

			SLuaEventDelegate<fnconv_t> deleg;
			deleg.token = m_tokenMgr.get();
			deleg.function = o;
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove_deleg(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
		}

		void remove(const uint64 &token) {
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_delegates.clear();
			m_tokenMgr.reset();
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState) {
			luabind::module(luaState.get()) [
				luabind::class_<SLuaEventDelegate<fnconv_t>>(),
				luabind::class_<LuaEvent<fnconv_t>>()
					.def("AddCallback", &LuaEvent<fnconv_t>::connect_lua)
					.def("RemoveCallback", &LuaEvent<fnconv_t>::remove_deleg)
					.def("ClearAll", &LuaEvent<fnconv_t>::clear)
					.property("numCallbacks", &LuaEvent<fnconv_t>::size)
			];
		}

	private:
		TokenManager m_tokenMgr;
		std::map<uint64, SLuaEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0) const {
			auto mDelegates = m_delegates;
			for (auto& pair: mDelegates)
				luabind::call_function<void>(pair.second.function, a0);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0) const {
			std::vector<T> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(luabind::call_function<T>(pair.second.function, a0));

			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P2 //////////////


	// Threadsafe
	template <typename R, typename A0, typename A1>
	class LuaEvent<R (A0, A1)>
	{
	public:
		typedef R fnconv_t(A0, A1);
		LuaEvent() : m_tokenMgr(), m_delegates() {}

		LuaEvent(LuaEvent<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_delegates(std::move(other.m_delegates)) {}

		LuaEvent<fnconv_t>& operator=(LuaEvent<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
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
			std::vector<R> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			//for (auto& pair: mDelegates)
			//	results.push_back(pair.second.callback(a0, a1));
			return results;
		}

		uint32 size() const {
			return m_delegates.size();
		}

		LuaEvent<fnconv_t>& operator-=(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SLuaEventDelegate<fnconv_t> connect_lua(const luabind::object &o) {
			if (luabind::type(o) != LUA_TFUNCTION)
				throw std::runtime_error("Invalid argument: Object is not a function!");

			SLuaEventDelegate<fnconv_t> deleg;
			deleg.token = m_tokenMgr.get();
			deleg.function = o;
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove_deleg(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
		}

		void remove(const uint64 &token) {
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_delegates.clear();
			m_tokenMgr.reset();
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState) {
			luabind::module(luaState.get()) [
				luabind::class_<SLuaEventDelegate<fnconv_t>>(),
				luabind::class_<LuaEvent<fnconv_t>>()
					.def("AddCallback", &LuaEvent<fnconv_t>::connect_lua)
					.def("RemoveCallback", &LuaEvent<fnconv_t>::remove_deleg)
					.def("ClearAll", &LuaEvent<fnconv_t>::clear)
					.property("numCallbacks", &LuaEvent<fnconv_t>::size)
			];
		}

	private:
		TokenManager m_tokenMgr;
		std::map<uint64, SLuaEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1) const {
			auto mDelegates = m_delegates;
			for (auto& pair: mDelegates)
				luabind::call_function<void>(pair.second.function, a0, a1);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1) const {
			std::vector<T> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(luabind::call_function<T>(pair.second.function, a0, a1));

			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P3 //////////////


	// Threadsafe
	template <typename R, typename A0, typename A1, typename A2>
	class LuaEvent<R (A0, A1, A2)>
	{
	public:
		typedef R fnconv_t(A0, A1, A2);
		LuaEvent() : m_tokenMgr(), m_delegates() {}

		LuaEvent(LuaEvent<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_delegates(std::move(other.m_delegates)) {}

		LuaEvent<fnconv_t>& operator=(LuaEvent<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
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
			std::vector<R> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			//for (auto& pair: mDelegates)
			//	results.push_back(pair.second.callback(a0, a1, a2));
			return results;
		}

		uint32 size() const {
			return m_delegates.size();
		}

		LuaEvent<fnconv_t>& operator-=(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SLuaEventDelegate<fnconv_t> connect_lua(const luabind::object &o) {
			if (luabind::type(o) != LUA_TFUNCTION)
				throw std::runtime_error("Invalid argument: Object is not a function!");

			SLuaEventDelegate<fnconv_t> deleg;
			deleg.token = m_tokenMgr.get();
			deleg.function = o;
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove_deleg(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
		}

		void remove(const uint64 &token) {
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_delegates.clear();
			m_tokenMgr.reset();
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState) {
			luabind::module(luaState.get()) [
				luabind::class_<SLuaEventDelegate<fnconv_t>>(),
				luabind::class_<LuaEvent<fnconv_t>>()
					.def("AddCallback", &LuaEvent<fnconv_t>::connect_lua)
					.def("RemoveCallback", &LuaEvent<fnconv_t>::remove_deleg)
					.def("ClearAll", &LuaEvent<fnconv_t>::clear)
					.property("numCallbacks", &LuaEvent<fnconv_t>::size)
			];
		}

	private:
		TokenManager m_tokenMgr;
		std::map<uint64, SLuaEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2) const {
			auto mDelegates = m_delegates;
			for (auto& pair: mDelegates)
				luabind::call_function<void>(pair.second.function, a0, a1, a2);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2) const {
			std::vector<T> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(luabind::call_function<T>(pair.second.function, a0, a1, a2));

			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};


//////////// EVENT P4 //////////////


	// Threadsafe
	template <typename R, typename A0, typename A1, typename A2, typename A3>
	class LuaEvent<R (A0, A1, A2, A3)>
	{
	public:
		typedef R fnconv_t(A0, A1, A2, A3);
		LuaEvent() : m_tokenMgr(), m_delegates() {}

		LuaEvent(LuaEvent<fnconv_t> &&other) :
			m_tokenMgr(std::move(other.m_tokenMgr)),
			m_delegates(std::move(other.m_delegates)) {}

		LuaEvent<fnconv_t>& operator=(LuaEvent<fnconv_t> &&other) {
			m_tokenMgr = std::move(other.m_tokenMgr);
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
			std::vector<R> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			//for (auto& pair: mDelegates)
			//	results.push_back(pair.second.callback(a0, a1, a2, a3));
			return results;
		}

		uint32 size() const {
			return m_delegates.size();
		}

		LuaEvent<fnconv_t>& operator-=(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
			return *this;
		}

		SLuaEventDelegate<fnconv_t> connect_lua(const luabind::object &o) {
			if (luabind::type(o) != LUA_TFUNCTION)
				throw std::runtime_error("Invalid argument: Object is not a function!");

			SLuaEventDelegate<fnconv_t> deleg;
			deleg.token = m_tokenMgr.get();
			deleg.function = o;
			m_delegates[deleg.token] = deleg;
			return deleg;
		}

		void remove_deleg(const SLuaEventDelegate<fnconv_t> &deleg) {
			remove(deleg.token);
		}

		void remove(const uint64 &token) {
			auto itr = m_delegates.find(token);
			if (itr != m_delegates.end()) {
				m_tokenMgr.remove(token);
				m_delegates.erase(itr);
			}
		}

		void clear() {
			m_delegates.clear();
			m_tokenMgr.reset();
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState) {
			luabind::module(luaState.get()) [
				luabind::class_<SLuaEventDelegate<fnconv_t>>(),
				luabind::class_<LuaEvent<fnconv_t>>()
					.def("AddCallback", &LuaEvent<fnconv_t>::connect_lua)
					.def("RemoveCallback", &LuaEvent<fnconv_t>::remove_deleg)
					.def("ClearAll", &LuaEvent<fnconv_t>::clear)
					.property("numCallbacks", &LuaEvent<fnconv_t>::size)
			];
		}

	private:
		TokenManager m_tokenMgr;
		std::map<uint64, SLuaEventDelegate<fnconv_t>> m_delegates;

		template <typename T, typename CompT>
		typename std::enable_if<std::is_same<void, T>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			auto mDelegates = m_delegates;
			for (auto& pair: mDelegates)
				luabind::call_function<void>(pair.second.function, a0, a1, a2, a3);
		}

		template <typename T, typename CompT>
		typename std::enable_if<Not<std::is_same<void, T>::value>::value, T>::type
			_invoke_impl(const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3) const {
			std::vector<T> results;
			auto mDelegates = m_delegates;
			results.reserve(mDelegates.size());
			for (auto& pair: mDelegates)
				results.push_back(luabind::call_function<T>(pair.second.function, a0, a1, a2, a3));

			return CompT::Comp<T>(results.cbegin(), results.cend());
		}
	};
#endif
}
