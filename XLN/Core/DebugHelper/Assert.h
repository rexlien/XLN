#pragma once

#include <XLN/Core/Config/Config.h>

namespace XCr
{

typedef bool (*AssertFailProcFuncPtr)(
        const char* pcExpression,
        const char* pcFile,
        const char* pcFunction,
        const int iLine);

class XLN_CLASS_DECL Assert
{
    
    public:
        static void RegisterAssertFailCB(AssertFailProcFuncPtr cb);
        // This assertion is the generic platform-specific assert handler
        /*
        static bool DefaultAssertFail(
            const char* pcExpression,
            const char* pcFile,
            const char* pcFunction,
            const int iLine);

        // This assertion routine will simply write out a debug string. 
        // It will not trigger a breakpoint. This is useful for running 
        // automated tests.
        static bool SimpleAssertFail(
            const char* pcExpression,
            const char* pcFile,
            const char* pcFunction,
            const int iLine);
*/
        static AssertFailProcFuncPtr ms_pfnNiAssertFailProc;
};

}

// NIASSERTINLINE results in a boolean type, 
// to enable nesting into statement calls, such as NiVerifyStaticCast
#ifdef _DEBUG
#if defined(_PS3) && defined(__SPU__)
#define XLN_ASSERT(boolean_expresion) \
    ((boolean_expresion) ? true : XLN_DEBUGBREAK())
#else
#define XLN_ASSERT(boolean_expresion) \
    ((boolean_expresion) ? true : \
    XCr::Assert::ms_pfnNiAssertFailProc( \
#boolean_expresion, __FILE__, __FUNCTION__, __LINE__))
#endif
#else
#define XLN_ASSERT(value) false
// Defaults to false to avoid misuse of form: ClientFunc(NIASSERT(...))
// Use NIVERIFY instead in those situations.
#endif

//#define NIASSERT(boolean_expresion) ((void)NIASSERTINLINE(boolean_expresion));