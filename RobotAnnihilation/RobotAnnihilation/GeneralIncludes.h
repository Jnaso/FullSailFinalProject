#pragma once

#ifndef _GENERALINCLUDE_H_
#define _GENERALINCLUDE_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

template <class T> inline void saferelease(T& t)
{
	if (t)
	{ 
		t->Release();
		t = 0;
	}

}

//Internal Files
#include <windows.h>
#include <memory>
#include <dxgi.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <SimpleMath.h>
#include <vector>
#include <iostream>
#include <functional>
#include <assert.h>

//External Files
#include "MathDefines.h"
#include "DDSTextureLoader.h"

#endif