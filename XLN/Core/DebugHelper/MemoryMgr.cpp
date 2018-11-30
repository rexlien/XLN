#include "MemoryMgr.h"

#include <XLN/Core/DebugHelper/Assert.h>
#include <XLN/Core/DebugHelper/Memory/MemAllocator.h>
#include <XLN/Core/DebugHelper/Memory/ExternalMemAllocator.h>

using namespace XCr;

static bool s_UserAllocator = false;

MemoryMgr::MemoryMgr(void)
{   
    m_Allocator = &GetDefMemAllocator();
}

MemoryMgr::~MemoryMgr(void)
{
    if(s_UserAllocator && m_Allocator)
    {  
		MemAllocator *pkAllocator = m_Allocator;
        m_Allocator = &GetDefMemAllocator();
		pkAllocator->~MemAllocator();
        XLN_ExternalFree(pkAllocator);
    }
}

MemoryMgr& XCr::MemoryMgr::GetInst()
{
    static MemoryMgr inst;
    return inst;
}

void XCr::MemoryMgr::SetAllocator( MemAllocator* allocator )
{   
    if(s_UserAllocator && m_Allocator)
	{
		MemAllocator *pkAllocator = m_Allocator;
        m_Allocator = &GetDefMemAllocator();
		s_UserAllocator = false;
		pkAllocator->~MemAllocator();
        XLN_ExternalFree(pkAllocator);
	}

	if (allocator)
	{
		s_UserAllocator = true;
    	m_Allocator = allocator;
	}
}

void XCr::MemoryMgr::DeAllocate( void* p )
{   
    XLN_ASSERT(m_Allocator);
    m_Allocator->DeAllocate(p);
   
}

void* XCr::MemoryMgr::ReAllocate( void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment )
{
    return 0;
}

void* XCr::MemoryMgr::Allocate( size_t sizeInBytes, size_t alignment, const char* fileName, const char* funcName, int line )
{
    XLN_ASSERT(m_Allocator);
    return m_Allocator->Allocate(sizeInBytes, alignment, fileName, funcName, line);
}

void* XCr::MemoryMgr::Allocate( size_t sizeInBytes, size_t alignment)
{
    XLN_ASSERT(m_Allocator);
    return m_Allocator->Allocate(sizeInBytes, alignment);
}

void XCr::MemoryMgr::ShutDown()
{   
    if(m_Allocator)
    {   
		m_Allocator->ShutDown();
    }
}

void XCr::MemoryMgr::DumpMemStatus()
{
    m_Allocator->DumpMemStatus();
}

XCr::UInt64 XCr::MemoryMgr::GetAllocatedSize()
{   
    if(m_Allocator)
    {   
		return m_Allocator->GetAllocatedSize();
    }

	return 0;
}

XCr::UInt64 XCr::MemoryMgr::GetAllocatedCount()
{   
    if(m_Allocator)
    {   
		return m_Allocator->GetAllocatedCount();
    }

	return 0;
}

XCr::UInt64 XCr::MemoryMgr::GetPeakSize()
{   
    if(m_Allocator)
    {   
		return m_Allocator->GetPeakSize();
    }

	return 0;
}

XCr::UInt64 XCr::MemoryMgr::GetPeakCount()
{   
    if(m_Allocator)
    {   
		return m_Allocator->GetPeakCount();
    }

	return 0;
}

MemAllocator& XCr::MemoryMgr::GetDefMemAllocator()
{   //NOTE:: the defMemallocator will not be delete, this ensure defMemAllocator will be always exist, 
    //although cause expected fixed memory leak, it should be omit as it's been treated as system level class. 
    static ExternalMemAllocator* defMemAllocator = XLN_ExternalNew (XLN_ExternalMalloc(sizeof(ExternalMemAllocator))) ExternalMemAllocator();//inst;
    return *defMemAllocator;//inst;
}
