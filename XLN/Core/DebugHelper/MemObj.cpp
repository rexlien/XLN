#include "MemObj.h"
#include <XLN/Core/DebugHelper/MemoryMgr.h>

using namespace XCr;

#ifdef MEM_OBJ_DEBUG_OPER
void* MemObj::operator new(size_t size, MemHint hint, const char* fileName, const char* funcName, int line)
{
    return MemoryMgr::GetInst().Allocate(size, 0, fileName, funcName, line);
    
}
void* MemObj::operator new[](size_t size, MemHint hint, const char* fileName, const char* funcName, int line)
{
    return MemoryMgr::GetInst().Allocate(size, 0, fileName, funcName, line);
}
/*
void MemObj::delete(void* p, MemHint hint)
{
    MemoryMgr::GetInst().DeAllocate(p);
}

void MemObj::delete[](void* p, MemHint hint)
{
    MemoryMgr::GetInst().DeAllocate(p);
}
*/
#else
void* MemObj::operator new(size_t size, MemHint hint)
{
    return ::operator new(size);

}
void* MemObj::operator new[](size_t size, MemHint hint)
{
    return ::operator new(size);
}
#endif

void MemObj::operator delete(void *p, size_t stElementSize)
{
     MemoryMgr::GetInst().DeAllocate(p);
}
void MemObj::operator delete[](void *p, size_t stElementSize)
{
     MemoryMgr::GetInst().DeAllocate(p);
}

void* MemObj::operator new(size_t size)
{   
    return ::operator new(size);
}

void* MemObj::operator new[](size_t size)
{
    return ::operator new(size);
}