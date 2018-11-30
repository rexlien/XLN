#pragma once

#include <XLN/Core/DebugHelper/MemoryMgr.h>


#if _MSC_VER && !XLN_SHIPPING  
/*
inline void* operator new(size_t size, const char *file, const char* func, int line)
{
    return XCr::MemoryMgr::GetInst().Allocate(size, 0, file, func, line);
}

inline void* operator new[](size_t size, const char *file, const char* func, int line)
{
    return XCr::MemoryMgr::GetInst().Allocate(size, 0, file, func, line);
}

inline void* operator new(size_t size)
{
    return XCr::MemoryMgr::GetInst().Allocate(size, 0);
}

inline void* operator new[](size_t size)
{
    return XCr::MemoryMgr::GetInst().Allocate(size, 0);
}

inline void operator delete(void* address)
{
    XCr::MemoryMgr::GetInst().DeAllocate(address);
}

inline void operator delete[](void* address)
{
    XCr::MemoryMgr::GetInst().DeAllocate(address);
}

inline void operator delete( void *address, const char *file, const char* func, int line )
{  
    XCr::MemoryMgr::GetInst().DeAllocate(address);
}

inline void operator delete[]( void *address, const char *file, const char* func, int line )
{ 
    XCr::MemoryMgr::GetInst().DeAllocate(address);
}


#define XLN_NEW new(__FILE__,__FUNCTION__, __LINE__)
#define XLN_DELETE delete
#define XLN_SAFE_DELETE(x) if(x) XLN_DELETE x;
*/
#define XLN_NEW new(std::nothrow)
#define XLN_DELETE delete
#define XLN_SAFE_DELETE(x) if(x) XLN_DELETE x;

#else

#define XLN_NEW new(std::nothrow)
#define XLN_DELETE delete
#define XLN_SAFE_DELETE(x) if(x) XLN_DELETE x;

#endif