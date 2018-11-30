#pragma once

/* Initial platform/compiler-related stuff to set.
*/
#define XLN_PLATFORM_WIN32 1
#define XLN_PLATFORM_LINUX 2
#define XLN_PLATFORM_APPLE_IOS 3
#define XLN_PLATFORM_APPLE_OSX 4
#define XLN_PLATFORM_ANDROID 5

#define XLN_COMPILER_MSVC 1
#define XLN_COMPILER_GNUC 2
#define XLN_COMPILER_BORL 3
#define XLN_COMPILER_CLANG 4

#define XLN_ENDIAN_LITTLE 0
#define XLN_ENDIAN_BIG 1

#define XLN_ARCHITECTURE_32 1
#define XLN_ARCHITECTURE_64 2


/* Finds the compiler type and version.
*/
#if defined( _MSC_VER )
#   define XLN_COMPILER XLN_COMPILER_MSVC
#   define XLN_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
#   define XLN_COMPILER COMPILER_GNUC
#   define XLN_COMP_VER (((__GNUC__)*100) + \
    (__GNUC_MINOR__*10) + \
    __GNUC_PATCHLEVEL__)
#elif defined(__clang__)
#   define XLN_COMP_VER    __clang_major__
#   define XLN_COMPILER XLN_COMPILER_CLANG
/*
#elif defined( __BORLANDC__ )
#   define XLN_COMPILER XLN_COMPILER_BORL
#   define XLN_COMP_VER __BCPLUSPLUS__
*/
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead */
#if XLN_COMPILER == XLN_COMPILER_MSVC
#   if XLN_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#else
#   define FORCEINLINE __inline
#endif

/* Finds the current platform */

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define XLN_PLATFORM XLN_PLATFORM_WIN32
//#	define GRAPHIC_SDK GRAPHIC_DIRECTX

#elif defined(__APPLE__)

#include <TargetConditionals.h>
    #if (TARGET_OS_IPHONE)
        #define XLN_PLATFORM XLN_PLATFORM_APPLE_IOS
    #elif (TARGET_OS_MAC)
        #define XLN_PLATFORM XLN_PLATFORM_APPLE_OSX
    #else
        #error Unknown Apple platform
    #endif

#elif defined(__ANDROID__)
#   define XLN_PLATFORM XLN_PLATFORM_ANDROID
#else
#   define XLN_PLATFORM XLN_PLATFORM_LINUX
#endif

/* Find the arch type */
#if defined(__x86_64__)
#   define XLN_ARCH_TYPE XLN_ARCHITECTURE_64
#else
#   define XLN_ARCH_TYPE XLN_ARCHITECTURE_32
#endif


#if XLN_PLATFORM == XLN_PLATFORM_WIN32
#ifdef _USRDLL
#   define XLN_CLASS_DECL __declspec(dllimport)
#else
#	define XLN_CLASS_DECL
#endif
#else
#   define XLN_CLASS_DECL __attribute__ ((visibility ("default")))
#endif

//#   define XLN_CLASS_DECL __declspec(dllexport)
//#   define XLN_IMPORT __attribute__ ((visibility ("default")))

#define XLN_PLATFORM_APPLE (XLN_PLATFORM == XLN_PLATFORM_APPLE_IOS || XLN_PLATFORM == XLN_PLATFORM_APPLE_OSX )
//----------------------------------------------------------------------------
// Windows Settings
#if XLN_PLATFORM == XLN_PLATFORM_WIN32


#   if defined( __MINGW32__ )
#       define EXT_HASH
#   else
//#       define snprintf _snprintf
//#       define vsnprintf vsnprintf
#   endif

#ifdef _DLL
#	ifndef _USEDLL
#		define _USEDLL
#	endif
#endif


#elif XLN_PLATFORM == XLN_PLATFORM_LINUX || XLN_PLATFORM_APPLE


// A quick define to overcome different names for the same function
#   define stricmp strcasecmp


#endif

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Endian Settings
#ifdef XLN_CONFIG_BIG_ENDIAN
#    define XLN_ARCH_ENDIAN XLN_ENDIAN_BIG
#else
#    define XLN_ARCH_ENDIAN XLN_ENDIAN_LITTLE
#endif



#if XLN_COMPILER == XLN_COMPILER_GNUC && COMP_VER >= 310
#   define HashMap ::__gnu_cxx::hash_map
#else
#   if XLN_COMPILER == XLN_COMPILER_MSVC
#       if COMP_VER > 1300 && !defined(_STLP_MSVC)
#           define HashMap ::stdext::hash_map
#       else
#           define HashMap ::std::hash_map
#       endif
#   else
#       define HashMap ::std::hash_map
#   endif
#endif





//module related
#if XLN_PLATFORM == XLN_PLATFORM_WIN32
#    define DYNLIB_HANDLE HINSTANCE
#    define DYNLIB_LOAD( a ) LoadLibrary( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) FreeLibrary( a )

//struct HINSTANCE__;
//typedef struct HINSTANCE__* hInstance;

#elif XLN_PLATFORM == XLN_PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif XLN_PLATFORM == XLN_PLATFORM_APPLE_OSX
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif
///////////////////////////

//unicode related


//function declaration define
#ifdef _WIN32
#define  STDCALL				__stdcall
#define  FASTCALL			   __fastcall
//#define  FORCEINLINE		   __forceinline	
#else
#define  STDCALL
#define  FASTCALL			   
//#define  FORCEINLINE		   inline
#endif

#define DONT_INLINE(a) (((int)(a)+1)?(a):(a))

#ifndef XLN_MAX_PATH
#define XLN_MAX_PATH  260
#endif


//optimization setting
#if XLN_COMPILER == XLN_COMPILER_MSVC

#include <memory.h>
#include <string.h>
#include <math.h>

#pragma intrinsic (memset, memcpy, memcmp)
#pragma intrinsic (strcpy, strcmp, strlen, strcat)
#pragma intrinsic (abs, fabs)


#endif
