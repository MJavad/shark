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

namespace Utils {
	template <bool C>
	struct Not : std::integral_constant<bool, !C> {};



	template <bool, bool,
			  bool = false, bool = false, bool = false, bool = false,
			  bool = false, bool = false, bool = false, bool = false,
			  bool = false, bool = false, bool = false, bool = false>
	struct Or : std::true_type {};

	template <>
	struct Or<false, false, false, false, false, false, false,
			  false, false, false, false, false, false, false> : std::false_type {};



	template <bool, bool,
			  bool = true, bool = true, bool = true, bool = true,
			  bool = true, bool = true, bool = true, bool = true,
			  bool = true, bool = true, bool = true, bool = true>
	struct And : std::false_type {};

	template <>
	struct And<true, true, true, true, true, true, true,
			   true, true, true, true, true, true, true> : std::true_type {};



	template <class>
	struct IsNumberHelper : std::false_type {};

	template <>
	struct IsNumberHelper<int8> : std::true_type {};

	template <>
	struct IsNumberHelper<int16> : std::true_type {};

	template <>
	struct IsNumberHelper<int32> : std::true_type {};

	template <>
	struct IsNumberHelper<int64> : std::true_type {};

	template <>
	struct IsNumberHelper<uint8> : std::true_type {};

	template <>
	struct IsNumberHelper<uint16> : std::true_type {};

	template <>
	struct IsNumberHelper<uint32> : std::true_type {};

	template <>
	struct IsNumberHelper<uint64> : std::true_type {};

	template <>
	struct IsNumberHelper<float> : std::true_type {};

	template <>
	struct IsNumberHelper<double> : std::true_type {};

	template <class T>
	struct IsNumber : IsNumberHelper<typename std::remove_cv<T>::type> {};



	template <class>
	struct IsStringHelper : std::false_type {};

	template <>
	struct IsStringHelper<char*> : std::true_type {};

	template <>
	struct IsStringHelper<wchar_t*> : std::true_type {};

	template <>
	struct IsStringHelper<std::string> : std::true_type {};

	template <>
	struct IsStringHelper<std::wstring> : std::true_type {};

	template <class T>
	struct IsString : IsStringHelper<typename std::remove_cv<T>::type> {};



	template <class>
	struct IsPointerHelper : std::false_type {};

	template <class T>
	struct IsPointerHelper<T*> : std::true_type {};

	template <class T>
	struct IsPointerHelper<std::shared_ptr<T>> : std::true_type {};

	template <class T>
	struct IsPointerHelper<std::weak_ptr<T>> : std::true_type {};

	template <class T>
	struct IsPointerHelper<std::auto_ptr<T>> : std::true_type {};

	template <class T>
	struct IsPointerHelper<std::unique_ptr<T>> : std::true_type {};

	template <class T>
	struct IsPointerHelper<boost::shared_ptr<T>> : std::true_type {};

	template <class T>
	struct IsPointerHelper<boost::weak_ptr<T>> : std::true_type {};

	template <class T>
	struct IsPointerHelper<CComPtr<T>> : std::true_type {};

	template <class T>
	struct IsPointer : IsPointerHelper<typename std::remove_cv<T>::type> {};



	template <class T>
	struct RemovePointerHelper {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<T*> {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<std::shared_ptr<T>> {
		typedef T type;
	};
	template <class T>
	struct RemovePointerHelper<std::weak_ptr<T>> {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<std::auto_ptr<T>> {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<std::unique_ptr<T>> {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<boost::shared_ptr<T>> {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<boost::weak_ptr<T>> {
		typedef T type;
	};

	template <class T>
	struct RemovePointerHelper<CComPtr<T>> {
		typedef T type;
	};

	template <class T>
	struct RemovePointer : RemovePointerHelper<typename std::remove_cv<T>::type> {};
}
