#ifndef __XCONFIG_H__
#define __XCONFIG_H__

#include <XLN/Core/Config/Platform.h>
#include <XLN/Core/Config/Module.h>

#define XLN_UNUSED
#define XLN_UNUSED_ARG(arg) ( (void)(arg) )
#define XLN_FORCEINLINE __forceinline
#define XLN_NOINLINE __declspec(noinline)



#if XLN_PLATFORM==XLN_PLATFORM_WIN32
#include <winsock2.h>
#include <windows.h>
	#define NOMINMAX 1
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif 

//=================== C++ header ============================//
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <chrono>
#include <cstring>
#include <cstdarg>
#include <cmath>

#if XLN_PLATFORM == XLN_PLATFORM_ANDROID
#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>
#endif

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <stack>
//autoptr
#include <memory>
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

//thread
#include <thread>
#include <mutex>

//Type headers
#include <XLN/Core/Config/Type.h>

//Misc Macro

#include <XLN/Core/Config/Math.h>

#include <XLN/Core/Config/Memory.h>
#include <XLN/Core/Config/New.h>



#endif