//
// Created by Rex Lien on 4/12/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Message/BaseMessage.h>

namespace XGf
{

class XLN_CLASS_DECL WebViewMessage : public XCr::KeyValuePairMessage
{
    XLN_DECLARE_BASE_MSG_CLASS(WebViewMessage);

    public:
        enum Command
        {
            LOAD_URL,
            INJECT_JSCRIPT
        };
        WebViewMessage();
        WebViewMessage(const XCr::String& webViewID, Command command);
        const XCr::String& GetWebViewID() const;
        Command GetCommand() const;
    private:
        Command m_Command;
        XCr::String m_WebViewID;
};

}