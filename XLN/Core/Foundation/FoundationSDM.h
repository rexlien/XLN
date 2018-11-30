#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Fwd.h>

namespace XCr
{

    class XLN_CLASS_DECL FoundationSDM
    {
        public:
            static void Init();
            static void ShutDown();
            
            //static void RunOnce();
            static ServiceMgr* GetCoreServiceMgr();
        
        private:
            static ServiceMgr* ms_CoreServiceMgr;
        
    }; 
    
}