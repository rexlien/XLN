#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/DebugHelper/Memory/MemAllocator.h>
#include <boost/thread/mutex.hpp>

#define XLN_ALLOCINFO_FILENAME_LEN 255
#define XLN_ALLOCINFO_FUNCNAME_LEN 255

namespace XCr
{

class XLN_CLASS_DECL MemTrackAllocator : public MemAllocator
{

    public:
        
        struct AllocateInfo
        {
            public:
                void* m_Addr;

                XCr::Char m_Srcfile[XLN_ALLOCINFO_FILENAME_LEN];
                XCr::Char m_SrcFunc[XLN_ALLOCINFO_FUNCNAME_LEN];
                XCr::Int32 m_SrcLine;
                XCr::Int32 m_AllocSize;

                AllocateInfo* m_Next;
                AllocateInfo* m_Prev;
        };
        
        typedef bool (*DumpAllocCB)(XCr::MemTrackAllocator::AllocateInfo* info);
        static DumpAllocCB RegisterDumpAllocCB(DumpAllocCB cb);
        
        MemTrackAllocator(MemAllocator* trackedAllcator);
        virtual ~MemTrackAllocator();
        virtual void* Allocate(size_t stSizeInBytes, size_t stAlignment, const char *file, const char* func, int line);
        virtual void* Allocate(size_t stSizeInBytes, size_t stAlignment);
        virtual void DeAllocate(void* p);
        virtual void* ReAllocate(void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment);
        virtual void ShutDown();
        virtual void DumpMemStatus();
        virtual XCr::UInt64 GetAllocatedSize();
		virtual XCr::UInt64 GetAllocatedCount();
		virtual XCr::UInt64 GetPeakSize();
		virtual XCr::UInt64 GetPeakCount();
         
        
    private:
        
        
        
        //template<class T>
        struct AllocHashTable
        {
            public:
                typedef bool (*ForAllAllocInfoCB)(AllocateInfo* info);
                //AllocHashTable(XCr::UInt32 fixSize);
                void Insert(AllocateInfo* allocInfo);
                AllocateInfo* Find(void* addr);
                void Remove(AllocateInfo* allocInfo);
                AllocateInfo* Remove(void* addr);
                void Clear();
                void ForAllAllocInfo(ForAllAllocInfoCB cb);
            
                XCr::UInt32 GetHash(void* addr);
                AllocateInfo** m_AllocInfo;
                XCr::UInt32 m_Size; //got be power of 2;
            private:
                boost::mutex m_HTMutex;
        };
        static bool DefDumpAllocCB(AllocateInfo* info); 
        static DumpAllocCB ms_DumpAllocCB;
        
        AllocateInfo* GetNextFreeInfo();
        void AddFreeInfo(AllocateInfo* info);
       
        
        //XCr::UInt32 m_AllocHTSize;
        AllocHashTable m_AllocHT;
        AllocateInfo* m_FreeInfo;
        MemAllocator* m_TrackedAllcator;
        
        
        boost::mutex m_TrackMutex;

		std::atomic<XCr::UInt64> m_lAllocatedSize;
		std::atomic<XCr::UInt64> m_lAllocatedCount;
		std::atomic<XCr::UInt64>	m_lPeakSize;
		std::atomic<XCr::UInt64>	m_lPeakCount;
};



}