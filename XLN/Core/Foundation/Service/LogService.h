#pragma once

#include <XLN/Core/Config/Config.h>

#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Foundation/Log.h>

namespace XCr
{
    class XLN_CLASS_DECL LogService : public XCr::Service
    {
        XLN_DECLARE_SERVICE_CLASS(LogService);
        public:
            LogService();
            virtual ~LogService();
            
            virtual bool Log(XCr::LogLevel level, const XCr::String& msg);
            virtual bool OnInit() override;
            virtual bool OnDestroy() override;

            void EnablePrintStdOut(bool enable)
            {
                m_PrintStdOut = enable;
            }

            void GetDependency(std::vector<ServiceID>& dependencies) override;
        protected:
            
            bool m_PrintStdOut;

    };
    
}