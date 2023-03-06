#pragma once

#define NOMINMAX

#include "targetver.h"


#ifdef _DEBUG
// check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DBG_NEW new // do not check for memory leaks
#define CATCH_EXCEPTIONS // in debug mode we want to see where we have error and handle it
#endif

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <stdio.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")


#include "json.h"
#include "SettingManager.h"
#include "GlobalVariables.h"


using namespace std;


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


// throw `std::exception` of the last winapi error (if it exists)
#define THROW_WINAPI_EXCEPTION { \
	int errId=GetLastError(); \
	if(errId!=NO_ERROR) { \
		char errMsg[256],buf[512]; \
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,nullptr,errId,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),errMsg,255,nullptr); \
		sprintf_s(buf,"function: %s\nfile: %s\nline: %d\n\nerror message: %s  error id: %d",__FUNCTION__,__FILE__,__LINE__,errMsg,errId); \
		throw std::exception(buf); \
	} \
}

// throw the last winapi error if `cond` is `true`
#define THROW_IF(cond)		{if(cond)THROW_WINAPI_EXCEPTION}

// throw the last winapi error if `x` is `nullptr`
#define THROW_IF_NULL(x)	THROW_IF((x)==nullptr)

// throw the last winapi error if `x` faild. `x` should be HRESULT object or function with HRESULT return type; 
#define THROW_IF_FAILED(x)	THROW_IF(FAILED(x))
// TODO: recompile
// #define THROW_IF_FAILED(x)	if (FAILED(x)) throw std::exception("file: " __FILE__ ":\n" #x " faild");


// check if `arr` contains `val`
#define FIND_IN_ARRAY(arr,val) (std::find(std::begin(arr),std::end(arr),val)!=std::end(arr))


// the real images size is 70px, and with that we can save the images ratio
#define REAL_IMAGE_SIZE 70.0f
// order the size of images
#define ORDER_SIZE(x) (TILE_SIZE/REAL_IMAGE_SIZE*(x))
