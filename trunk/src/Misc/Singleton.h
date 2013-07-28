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

namespace Utils
{
	class Singleton
	{
	public:
		template <class T>
		static typename std::enable_if<
			std::is_base_of<Singleton, T>::value, T&>::type
			Instance() {
			static T sInstance;
			return sInstance;
		}

	protected:
		Singleton() {}

	private:
		Singleton(const Singleton&);
		Singleton& operator=(const Singleton&);
	};
}

#define SINGLETON_OBJECT friend class ::Utils::Singleton;
#define GET_INSTANCE(T) ::Utils::Singleton::Instance<::T>()

#define SINGLETON_CLASS(T) \
public:		static T& Instance() { \
				static T sInstance; \
				return sInstance; \
			} \
private:	T(const T&); \
			T& operator=(const T&); \
			T()
