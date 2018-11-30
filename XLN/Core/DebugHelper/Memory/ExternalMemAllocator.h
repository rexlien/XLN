#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/DebugHelper/Memory/MemAllocator.h>

namespace XCr
{

    class XLN_CLASS_DECL ExternalMemAllocator : public MemAllocator
    {
    
        public:
            ExternalMemAllocator();
            virtual ~ExternalMemAllocator();
            
            virtual void* Allocate(size_t stSizeInBytes, size_t stAlignment, const char *file, const char* func, int line);
	        virtual void* Allocate(size_t stSizeInBytes, size_t stAlignment);
            virtual void DeAllocate(void* p);
            virtual void* ReAllocate(void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment);
            
            
    
    };


}