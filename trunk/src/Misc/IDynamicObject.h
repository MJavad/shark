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
	class IDynamicObject abstract : 
		public boost::enable_shared_from_this<IDynamicObject>
	{
	protected:
		IDynamicObject() {}

		template <typename T>
		boost::shared_ptr<T> getThis() {
			return boost::dynamic_pointer_cast<T>(shared_from_this());
		}

		template <typename T>
		boost::shared_ptr<const T> getThis() const {
			return boost::dynamic_pointer_cast<const T>(shared_from_this());
		}

		template <>
		boost::shared_ptr<IDynamicObject> getThis() {
			return shared_from_this();
		}

		template <>
		boost::shared_ptr<const IDynamicObject> getThis() const {
			return shared_from_this();
		}

	public:
		virtual ~IDynamicObject() {}
	};
}
