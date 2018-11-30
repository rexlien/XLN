#ifndef __XLN_MEMORYCONFIG_H__
#define __XLN_MEMORYCONFIG_H__


#include <XLN/Core/DebugHelper/MemObj.h>


#ifndef XLN_SHIPPING

#define XLN_OBJ_NEW new(MEM_OBJ_DEFAULT_MEMHINT, __FILE__,__FUNCTION__, __LINE__)
#define XLN_OBJ_DELETE delete
//#define XLN_NEWR(placement) new(placement, __FILE__, __FUNCTION__, __LINE__)
#else


#define XLN_OBJ_NEW new(XCr::MemHint(XCr::MemHint::NONE))
#define XLN_OBJ_DELETE delete

#endif

#define XLN_ExternalNew new
#define XLN_ExternalDelete delete
#define XLN_ExternalAlloc(T, count) ((T*)malloc(sizeof(T)*(count)))
#define XLN_ExternalMalloc malloc
#define XLN_ExternalRealloc realloc
#define XLN_ExternalFree free
#define XLN_ExternalCalloc calloc


#endif