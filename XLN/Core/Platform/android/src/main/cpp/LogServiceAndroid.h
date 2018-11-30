//
// Created by Rex Lien on 2/16/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>

#include <XLN/Core/Foundation/Service/LogService.h>
#include <XLN/Core/Foundation/Log.h>

namespace XCr
{

class XLN_CLASS_DECL LogServiceAndroid : public XCr::LogService
{
    XLN_DECLARE_SERVICE_CLASS(LogServiceAndroid);
    public:
        LogServiceAndroid();
        virtual ~LogServiceAndroid();

        virtual bool Log(XCr::LogLevel level, const XCr::String& msg) override;

};

}