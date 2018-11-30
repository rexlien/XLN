#pragma once

#include <XLN/Core/Config/Config.h>
/**
A platform specific Memory allocator
*/
namespace XCr
{

class XLN_CLASS_DECL MemAllocator
{
    public:
        virtual ~MemAllocator(){};
        virtual void* Allocate(size_t stSizeInBytes, size_t stAlignment, const char *file, const char* func, int line) = 0;
        virtual void* Allocate(size_t stSizeInBytes, size_t stAlignment) = 0;
        virtual void DeAllocate(void* p) = 0;
        virtual void* ReAllocate(void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment) = 0;
        virtual void ShutDown() {};
        virtual void DumpMemStatus() {};
		virtual XCr::UInt64 GetAllocatedSize() {return 0;}
		virtual XCr::UInt64 GetAllocatedCount() {return 0;}
		virtual XCr::UInt64 GetPeakSize() {return 0;}
		virtual XCr::UInt64 GetPeakCount() {return 0;}
};

}