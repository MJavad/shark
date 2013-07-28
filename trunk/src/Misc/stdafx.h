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
#define _CRT_SECURE_NO_WARNINGS
//#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "d3d11.lib") Not present in Windows XP...
#pragma comment(lib, "Winmm.lib")
#pragma warning(disable: 4100 4121)

//#define VARIADIC_TEMPLATES_SUPPORTED

// Logging...
#define DEBUG_USE_LOG
#define DEBUG_USE_FUNCTIONNAMES
#define DEBUG_USE_FILENAMES

typedef unsigned long long uint64;
typedef signed long long int64;

typedef unsigned int uint32;
typedef signed int int32;

typedef unsigned short uint16;
typedef signed short int16;

typedef unsigned char uint8;
typedef signed char int8;

#ifndef M_PI
#define M_PI 3.141592865358979
#endif

#ifndef M_PI_F
#define M_PI_F 3.141592865358979f
#endif

#ifndef M_TWO_PI
#define M_TWO_PI 6.283185730717958
#endif

#ifndef M_TWO_PI_F
#define M_TWO_PI_F 6.283185730717958f
#endif

#include <Windows.h>
#include <windowsx.h>
#include <TlHelp32.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <stack>
#include <type_traits>
#include <functional>
#include <thread>
#include <unordered_map>
#include <boost/format.hpp>
#include <boost/cast.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <codecvt>
#include <atlbase.h>
#include <iostream>

// lua & luabind
#include "lua/lua.hpp"
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"
#include "luabind/shared_ptr_converter.hpp"

#include "TypeTraits.h"
#include "Mutex.h"
#include "Color.h"
#include "Vector.h"
#include "Singleton.h"
#include "ByteBuffer.h"
#include "Event.h"
#include "Handle.h"
#include "Signal.h"
#include "Heap.h"
#include "TimerDispatcher.h"
#include "ThreadGrabber.h"
#include "Helpers.h"
#include "Base/Log.h"
#include "SharkMemory.h"

#undef DrawText
#undef GetObject

#define RADIAN(deg) (deg*(M_PI_F/180.0f))
#define DEGREE(rad) (rad/(M_PI_F/180.0f))
#define ROUND(number) (floor(number+0.5f))
#define MIN_LIMIT(var,limit) if(var<limit) var=limit
#define MAX_LIMIT(var,limit) if(var>limit) var=limit
#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

struct float2 {
	float _1, _2;
};

struct float3 {
	float _1, _2, _3;
};

struct float4 {
	float _1, _2, _3, _4;
};
