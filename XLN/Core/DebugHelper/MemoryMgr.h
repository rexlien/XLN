#pragma once

#include <XLN/Core/Config/Platform.h>
#include <cstdlib>
#include <string>
#include <XLN/Core/Config/Type.h>
#include <XLN/Core/DebugHelper/Fwd.h>


namespace XCr
{


class XLN_CLASS_DECL MemoryMgr
{
    public:
      
		inline MemAllocator *GetAllocator() { return m_Allocator; }
        void SetAllocator(MemAllocator* allocator);
        void* Allocate(size_t sizeInBytes, size_t alignment, const char* fileName, const char* funcName, int line);
        void* Allocate(size_t sizeInBytes, size_t alignment);
        void DeAllocate(void* p);
        void* ReAllocate(void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment);
        static MemoryMgr& GetInst();
        void ShutDown();
        void DumpMemStatus();
        XCr::UInt64 GetAllocatedSize();
		XCr::UInt64 GetAllocatedCount();
		XCr::UInt64 GetPeakSize();
		XCr::UInt64 GetPeakCount();
        
    private:
        static MemAllocator& GetDefMemAllocator();
        
        MemoryMgr(void);
        ~MemoryMgr(void);
        MemAllocator* m_Allocator;
};

}

