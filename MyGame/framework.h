#pragma once

#include "targetver.h"


#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES	// for M_PI
#define NOMINMAX			// I prefer use the `std::min` and `std::max` insted the `#define min / max ...`


#ifdef _DEBUG
// check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__) // TODO: use `std::uniqe_ptr` / `std::shared_ptr`
#else
#define DBG_NEW new // do not check for memory leaks
#define CATCH_EXCEPTIONS // in debug mode we want to see where we have error and handle it
#endif

#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")


#include "json.h"
#include "SettingManager.h"
#include "LogFile.h"


using namespace std;


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


// throw `std::exception` of the last winapi error (if it exists)
#define THROW_WINAPI_EXCEPTION { \
	int errId = GetLastError(); \
	if (errId != NO_ERROR) { \
		char errMsg[256], buf[512]; \
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errMsg, 255, nullptr); \
		sprintf_s(buf, "function: %s\nfile: %s\nline: %d\n\nerror message: %s\nerror id: %d", __FUNCTION__, __FILE__, __LINE__, errMsg, errId); \
		throw std::exception(buf); \
	} \
}

// throw the last winapi error if `cond` is `true`
#define THROW_IF(cond)		if (cond) THROW_WINAPI_EXCEPTION

// throw the last winapi error if `x` is `nullptr`
#define THROW_IF_NULL(x)	THROW_IF((x) == nullptr)

// throw the last winapi error if `x` faild. `x` should be HRESULT object or function with HRESULT return type; 
#define THROW_IF_FAILED(x)	THROW_IF(FAILED(x))
// TODO: recompile
// #define THROW_IF_FAILED(x)	if (FAILED(x)) throw std::exception("file: " __FILE__ ":\n" #x " faild");


// check if `arr` contains `val`
#define FIND_IN_ARRAY(arr,val) (std::find(std::begin(arr), std::end(arr), val) != std::end(arr))


// the real images size is 70px, and with that we can save the images ratio
#define REAL_IMAGE_SIZE 70.0f
// order the size of images
#define ORDER_SIZE(x) (TILE_SIZE / REAL_IMAGE_SIZE * (x))



enum class MouseButtons
{
	Left,
	Middle,
	Right
};


/* COM inheritance helpers */

// Releases a COM object and nullifies pointer.
template <typename InterfaceType>
inline void SafeRelease(InterfaceType** currentObject)
{
	if (*currentObject != nullptr)
	{
		(*currentObject)->Release();
		*currentObject = nullptr;
	}
}



/* Math Functions */

// check if `val` is between `a` and `b` (include `a` and `b`)
template <class T>
inline bool InRange(T val, T a, T b)
{
	if (a > b)
		return b <= val && val <= a;
	return a <= val && val <= b;
}

// return the distance between `a` and `b`
inline double GetDistance(D2D1_POINT_2F a, D2D1_POINT_2F b)
{
	return hypot(a.x - b.x, a.y - b.y);
}
// return the distance between `a` and `b` (by the center)
inline double GetDistance(D2D1_RECT_F a, D2D1_RECT_F b)
{
	return GetDistance(
		D2D1::Point2F((a.left + a.right) / 2, (a.top + a.bottom) / 2),
		D2D1::Point2F((b.left + b.right) / 2, (b.top + b.bottom) / 2)
	);
}

inline D2D1_SIZE_F GetRectSize(D2D1_RECT_F rc)
{
	return D2D1::SizeF(ORDER_SIZE(rc.right - rc.left), ORDER_SIZE(rc.bottom - rc.top));
}

inline void ChangePointZoomRatio(D2D1_POINT_2F& point, int oldTileSize, int newTileSize)
{
	// we convert the point to be relative to the tile size, and then return to the new tile size.
	point.x = point.x / oldTileSize * newTileSize;
	point.y = point.y / oldTileSize * newTileSize;
}

// return point from json (list with two elements)
inline D2D1_POINT_2F GetPointF(const json& j, float n = 1)
{
	return D2D1::Point2F((float)j[0] * n, (float)j[1] * n);
}
