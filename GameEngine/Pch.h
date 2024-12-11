#pragma once

#define WIN32_LEAN_AND_MEAN   

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Engine.lib" )
#else
#pragma comment(lib, "Release\\Engine.lib")
#endif // _DEBUG


#include <CorePch.h>