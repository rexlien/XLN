//#include <windows.h>
#include "MemTrackAllocator.h"
//#include <XLN/Core/DebugHelper/Log.h>
#include <XLN/Core/DebugHelper/Assert.h>

using namespace XCr;

MemTrackAllocator::DumpAllocCB MemTrackAllocator::ms_DumpAllocCB = 0;

MemTrackAllocator::MemTrackAllocator( MemAllocator* trackedAllcator )
{   
    m_TrackedAllcator = trackedAllcator;
    memset(&m_AllocHT, 0, sizeof(AllocHashTable));
    m_AllocHT.m_Size = 1 << 12;
    m_AllocHT.m_AllocInfo = (AllocateInfo**)(XLN_ExternalMalloc(sizeof(AllocateInfo*) * m_AllocHT.m_Size));
    memset(m_AllocHT.m_AllocInfo, 0, sizeof(AllocateInfo*) * m_AllocHT.m_Size);
    m_FreeInfo = 0;
	m_lAllocatedSize = 0;
	m_lAllocatedCount = 0;
	m_lPeakSize = 0;
	m_lPeakCount = 0;
}

void* MemTrackAllocator::Allocate(size_t stSizeInBytes, size_t stAlignment, const char *file, const char* func, int line)
{
    void* allocatedAddr = m_TrackedAllcator->Allocate(stSizeInBytes, stAlignment, file, func, line);
    
    if(allocatedAddr)
    {   //add allocate info
        AllocateInfo* newInfo = GetNextFreeInfo();
        if(!newInfo)
        {
            newInfo = XLN_ExternalAlloc(AllocateInfo, 1);
        }

		if (newInfo)
		{
			newInfo->m_Addr = allocatedAddr;
			strcpy(newInfo->m_Srcfile, file);
			strcpy(newInfo->m_SrcFunc, func);
			newInfo->m_SrcLine = line;
			newInfo->m_AllocSize = ( XCr::Int32)stSizeInBytes;
			newInfo->m_Next = 0;
			newInfo->m_Prev = 0;

            m_lAllocatedSize.fetch_add(stSizeInBytes);
            m_lAllocatedCount++;
            m_lPeakSize = std::max(m_lPeakSize.load(), m_lAllocatedSize.load());
            m_lPeakCount = std::max(m_lPeakCount.load(), m_lAllocatedCount.load());

			m_AllocHT.Insert(newInfo);
		}
    }
    return allocatedAddr;
}

void* MemTrackAllocator::Allocate(size_t stSizeInBytes, size_t stAlignment)
{
   return m_TrackedAllcator->Allocate(stSizeInBytes, stAlignment);
}
void MemTrackAllocator::DeAllocate( void* p )
{
    m_TrackedAllcator->DeAllocate(p);
    if(p)
    {   //remove allocate info
        AllocateInfo* freeInfo = m_AllocHT.Remove(p);
        if(freeInfo)
        {
            m_lAllocatedSize.fetch_sub(freeInfo->m_AllocSize);
            m_lAllocatedCount--;

            AddFreeInfo(freeInfo);
        }
    }
}

MemTrackAllocator::AllocateInfo* MemTrackAllocator::GetNextFreeInfo()
{   boost::mutex::scoped_lock lock( m_TrackMutex );
    if(m_FreeInfo)
    {
        AllocateInfo* ret = m_FreeInfo;
        m_FreeInfo = m_FreeInfo->m_Next;
        return ret;
    }
    return 0;
}

void MemTrackAllocator::AddFreeInfo( AllocateInfo* info )
{
    boost::mutex::scoped_lock lock( m_TrackMutex );
    info->m_Next = m_FreeInfo;
    info->m_Prev = 0;
    m_FreeInfo = info;
}

void* MemTrackAllocator::ReAllocate( void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment )
{
    return 0;
}

void MemTrackAllocator::ShutDown()
{   
    //remaining allocate info indicate leaked memory
    //DumpMemStatus();
    m_AllocHT.Clear();
   
}

MemTrackAllocator::~MemTrackAllocator()
{
    if(m_AllocHT.m_AllocInfo)
        XLN_ExternalFree(m_AllocHT.m_AllocInfo);
    
    m_TrackedAllcator->~MemAllocator();
    XLN_ExternalFree(m_TrackedAllcator);
    
}

void MemTrackAllocator::AllocHashTable::Insert( AllocateInfo* allocInfo )
{   
    XLN_ASSERT(allocInfo->m_Next == 0);
    XLN_ASSERT(allocInfo->m_Prev == 0);
     XCr::UInt32 index = GetHash(allocInfo->m_Addr);
     
     
     //check valid
     MemTrackAllocator::AllocateInfo* head = m_AllocInfo[index];
     while(head)
     {  XLN_ASSERT(head->m_Addr != allocInfo->m_Addr);
        head = head->m_Next;
     }
     
     boost::mutex::scoped_lock lock( m_HTMutex );
     MemTrackAllocator::AllocateInfo* bucket = m_AllocInfo[index];
     m_AllocInfo[index] = allocInfo;
     m_AllocInfo[index]->m_Next = bucket;
     if(bucket)
        bucket->m_Prev = allocInfo;
        
      
    
}


XCr::UInt32 MemTrackAllocator::AllocHashTable::GetHash( void* addr )
{   
    //TODO: this could be further optimized
     return (reinterpret_cast<XCr::IntNative>(addr) >> 4) & (m_Size - 1);
}

void MemTrackAllocator::AllocHashTable::Remove( AllocateInfo* allocInfo )
{
    Remove(allocInfo->m_Addr);
}

MemTrackAllocator::AllocateInfo* MemTrackAllocator::AllocHashTable::Remove( void* addr )
{   
    XCr::UInt32 index = GetHash(addr);
    boost::mutex::scoped_lock lock( m_HTMutex );
    MemTrackAllocator::AllocateInfo* bucket = m_AllocInfo[index];
    
    //if bucket head hit
    if(bucket)// && bucket->m_Next == 0)
    {   if(bucket->m_Addr == addr)
        {   
            m_AllocInfo[index] = bucket->m_Next;
            return bucket;
        }
        bucket = bucket->m_Next;
    }
    
    while(bucket)
    {   
       if(bucket->m_Addr == addr)
       {    
           
            if(bucket->m_Prev)
                bucket->m_Prev->m_Next = bucket->m_Next;
            if(bucket->m_Next)
                bucket->m_Next->m_Prev = bucket->m_Prev;
            return bucket;
       }
       
       bucket = bucket->m_Next;
    }
    return 0;
}

bool MemTrackAllocator::DefDumpAllocCB(XCr::MemTrackAllocator::AllocateInfo* info)
{   
  
    //XLN_LOGW("Remaining Mem in: %s, %s, %d\n", info->m_Srcfile, info->m_SrcFunc, info->m_SrcLine);
    return true;
}

void MemTrackAllocator::AllocHashTable::ForAllAllocInfo( ForAllAllocInfoCB cb )
{
    boost::mutex::scoped_lock lock( m_HTMutex );
    for(XCr::UInt32 i = 0; i < m_Size; i++)
    {   AllocateInfo* info = m_AllocInfo[i];
        while(info)
        {      
            //XLN_LOGW("Remaining Mem info index: %d\n", i);
            cb(info); 
            info = info->m_Next;
        }
    }
}

void MemTrackAllocator::AllocHashTable::Clear()
{   
    boost::mutex::scoped_lock lock( m_HTMutex );
    for(XCr::UInt32 i = 0; i < m_Size; i++)
    {   AllocateInfo* info = m_AllocInfo[i];
        while(info)
        {        
            AllocateInfo* curInfo = info;
            info = info->m_Next;
            XLN_ExternalFree(curInfo);
        }
        m_AllocInfo[i] = 0;
    }
    
}



void XCr::MemTrackAllocator::DumpMemStatus()
{   if(ms_DumpAllocCB)
    {
        m_AllocHT.ForAllAllocInfo(ms_DumpAllocCB);
    }
    //else
    //m_AllocHT.ForAllAllocInfo(DefDumpAllocCB);
}

XCr::MemTrackAllocator::DumpAllocCB XCr::MemTrackAllocator::RegisterDumpAllocCB( DumpAllocCB cb )
{
    ms_DumpAllocCB = cb;
    return 0;
}

XCr::UInt64 XCr::MemTrackAllocator::GetAllocatedSize()
{
	return m_lAllocatedSize;
}

XCr::UInt64 XCr::MemTrackAllocator::GetAllocatedCount()
{
	return m_lAllocatedCount;
}

XCr::UInt64 XCr::MemTrackAllocator::GetPeakSize()
{
	return m_lPeakSize;
}

XCr::UInt64 XCr::MemTrackAllocator::GetPeakCount()
{
	return m_lPeakCount;
}
