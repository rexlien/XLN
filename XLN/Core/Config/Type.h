#ifndef __XTYPE_H__
#define __XTYPE_H__



namespace XCr
{
    
    typedef unsigned int UInt;
    typedef int Int;
#ifdef _MSC_VER
    typedef __int8 Int8;
    typedef unsigned __int8 UInt8;
    typedef __int16 Int16;
    typedef unsigned __int16 UInt16;
    typedef __int32 Int32;
    typedef unsigned __int32 UInt32;
    typedef __int64 Int64;
    typedef unsigned __int64 UInt64;
#else
    typedef int8_t Int8;
    typedef uint8_t UInt8;
    typedef int16_t Int16;
    typedef uint16_t UInt16;
    typedef int32_t Int32;
    typedef uint32_t UInt32;
    typedef int64_t Int64;
    typedef uint64_t UInt64;
#endif
    
    typedef bool Bool;
    typedef char  Char;
    typedef unsigned char UChar;
    
    typedef float Float32;
    typedef double Float64;


    
    typedef std::string String;
    typedef std::wstring WString;
    


    typedef wchar_t WChar;
    typedef std::ostringstream Ostringstream;
    typedef std::wostringstream WOstringstream;

    typedef std::ofstream Ofstream;
    typedef std::wofstream WOfstream;   
    
#ifdef X_FLOAT_64
    typedef double FloatNative;
#else
    typedef float FloatNative;
#endif

#ifdef _MSC_VER
#ifdef _WIN64
    typedef __int64 IntNative;
#else
    typedef std::ptrdiff_ IntNative;
#endif

#ifdef _WIN64
    typedef unsigned __int64 UIntNative;
#else
    typedef std::size_t UIntNative;
#endif
#else
    typedef std::ptrdiff_t IntNative;
    typedef std::size_t UIntNative;
#endif

#ifdef _UNICODE
    typedef std::wstring StringNative;
    typedef wchar_t CharNative;
#else
    typedef std::string StringNative;
    typedef char CharNative;
#endif 
    /*
    template <typename T>
    struct Shared_Ptr
    {	typedef boost::shared_ptr<T> type;
    
    };
    */

}

#	define XLN_WCHAR( x ) L ## x
#	define XLN_WIDEN(x) XLN_WCHAR(x)

#endif