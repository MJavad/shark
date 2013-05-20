#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "d3d11.lib") Not present in Windows XP...
#pragma comment(lib, "Winmm.lib")
#pragma warning(disable: 4100)

//#define VARIADIC_TEMPLATES_SUPPORTED
#define DEBUG_USE_FUNCTIONNAMES

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
#include <codecvt>
#include <atlbase.h>
#include "Utility.h"
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
#include "Base/Log.h"
#include "SharkMemory.h"

#undef DrawText
#undef GetObject

#define RADIAN(deg) (deg*(M_PI_F/180.0f))
#define DEGREE(rad) (rad/(M_PI_F/180.0f))
#define ROUND(number) (floor(number+0.5f))
#define MIN_LIMIT(var,limit) if(var<limit) var=limit
#define MAX_LIMIT(var,limit) if(var>limit) var=limit

struct float2 {
	float _1, _2;
};

struct float3 {
	float _1, _2, _3;
};

struct float4 {
	float _1, _2, _3, _4;
};

