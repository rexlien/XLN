#include "ExternalMemAllocator.h"

using namespace XCr;

void* ExternalMemAllocator::Allocate( size_t stSizeInBytes, size_t stAlignment, const char *file, const char* func, int line )
{
	return Allocate(stSizeInBytes, stAlignment);
}

void* ExternalMemAllocator::Allocate( size_t stSizeInBytes, size_t stAlignment)
{
//#if XLN_USEDLL
    return operator new(stSizeInBytes);
//#else
//	return malloc(stSizeInBytes);
//#endif
}

void ExternalMemAllocator::DeAllocate( void* p )
{
//#if XLN_USEDLL
    operator delete(p);
//#else
 //   free(p);
//#endif
}

void* ExternalMemAllocator::ReAllocate( void* pvMemory,size_t& stSizeInBytes,size_t& stAlignment )
{
    return 0;
}

ExternalMemAllocator::~ExternalMemAllocator()
{

}

ExternalMemAllocator::ExternalMemAllocator()
{

}