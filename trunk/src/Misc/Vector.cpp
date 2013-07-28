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
#include "Vector.h"

std::ostream& operator << (std::ostream &o, const Utils::Vector2 &v) {
	o << "{ X: " << v.x << ", Y: " << v.y << " }";
	return o;
}

std::ostream& operator << (std::ostream &o, const Utils::Vector3 &v) {
	o << "{ X: " << v.x << ", Y: " << v.y << ", Z: " << v.z << " }";
	return o;
}

namespace Utils {
	void Vector2::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<Vector2>("Vector2")
				.def(luabind::constructor<>())
				.def(luabind::constructor<Vector2>())
				.def(luabind::constructor<Vector3>())
				.def(luabind::constructor<float>())
				.def(luabind::constructor<float, float>())
				.def_readwrite("x", &Vector2::x)
				.def_readwrite("y", &Vector2::y)
				.def("angle", &Vector2::angle)
				.def("dot", &Vector2::dot)
				.def("normalize", &Vector2::normalize)
				.def("length", &Vector2::length)
				.def("distanceTo", &Vector2::distance_to)
				.def(luabind::const_self + Vector2())
				.def(luabind::const_self - Vector2())
				.def(luabind::const_self * Vector2())
				.def(luabind::const_self / Vector2())
				.def(luabind::const_self == Vector2())
				.def(luabind::const_self < Vector2())
				.def(luabind::const_self <= Vector2())
				.def(luabind::tostring(luabind::self))
		];
	}

	void Vector3::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<Vector3>("Vector3")
				.def(luabind::constructor<>())
				.def(luabind::constructor<Vector2>())
				.def(luabind::constructor<Vector3>())
				.def(luabind::constructor<float>())
				.def(luabind::constructor<float, float>())
				.def(luabind::constructor<float, float, float>())
				.def_readwrite("x", &Vector3::x)
				.def_readwrite("y", &Vector3::y)
				.def_readwrite("z", &Vector3::z)
				.def("angle", &Vector3::angle)
				.def("dot", &Vector3::dot)
				.def("cross", &Vector3::cross)
				.def("normalize", &Vector3::normalize)
				.def("length", &Vector3::length)
				.def("length2d", &Vector3::length2D)
				.def("distanceTo", &Vector3::distance_to)
				.def(luabind::const_self + Vector3())
				.def(luabind::const_self - Vector3())
				.def(luabind::const_self * Vector3())
				.def(luabind::const_self / Vector3())
				.def(luabind::const_self == Vector3())
				.def(luabind::const_self < Vector3())
				.def(luabind::const_self <= Vector3())
				.def(luabind::tostring(luabind::self))
		];
	}
}
