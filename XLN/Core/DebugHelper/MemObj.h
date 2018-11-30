#pragma once

#include <XLN/Core/Config/Config.h>

#ifndef XLN_SHIPPING
#define MEM_OBJ_DEBUG_OPER
#endif

namespace XCr
{
    class XLN_CLASS_DECL MemHint
    {
        
        
        
        public:
            
            enum Bits
            {   
                NONE = 0

            };
            
            MemHint() {m_Hint = NONE;};
            MemHint(Bits hint) {m_Hint = hint;};
         private:
            Bits m_Hint;
    };
    
    
    class XLN_CLASS_DECL MemObj
    {    
        
        public:
#ifdef MEM_OBJ_DEBUG_OPER        
            static void* operator new(size_t size, MemHint hint, const char* fileName, const char* funcName, int line);
            static void* operator new[](size_t size, MemHint hint, const char* fileName, const char* funcName, int line);
            
            static void operator delete(
                void*, 
                MemHint,
                const char*, 
                const char*, 
                int){}

            static void operator delete[](
                void*, 
                MemHint,
                const char*, 
                const char*, 
                int){}
            //static void delete(void* p, MemHint hint);
            //static void delete[](void* p, MemHint hint);
#else
    
            static void* operator new(size_t stSize, MemHint kHint);
            static void* operator new[](size_t stSize, MemHint kHint);

            // Required for exception handling in the compiler. These 
            // should not be used in practice.
            static void operator delete( void *, MemHint) {}
            static void operator delete[]( void *, MemHint) {}
#endif            

            static void operator delete(void *pvMem, size_t stElementSize);
            static void operator delete[](void *pvMem, size_t stElementSize);
            
            static void* operator new(size_t size);
            static void* operator new[](size_t size);
            
            //
            inline static void* operator new( size_t, void* p ) { return p; }
            inline static void* operator new[]( size_t, void* p ) { return p; }

            // Conversely we also then need a placement delete function:
            inline static void operator delete( void *, void* ) {}
            inline static void operator delete[]( void *, void* ) {}
    };
}

#define MEM_OBJ_DEFAULT_MEMHINT XCr::MemHint() 