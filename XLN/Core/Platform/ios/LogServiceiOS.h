#pragma once

#include <XLN/Core/Config/Config.h>

#include <XLN/Core/Foundation/Service/LogService.h>
#include <XLN/Core/Foundation/Log.h>

namespace XCr
{
    class XLN_CLASS_DECL LogServiceiOS : public XCr::LogService
    {
        XLN_DECLARE_SERVICE_CLASS(LogServiceiOS);
        public:
            LogServiceiOS();
            virtual ~LogServiceiOS();
            
            virtual bool Log(XCr::LogLevel level, const XCr::String& msg) override;
            
        protected:
        

    };
    
}