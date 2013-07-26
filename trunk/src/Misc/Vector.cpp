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
